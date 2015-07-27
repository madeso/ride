// Copyright (2015) Gustav

#include "ride/wx.h"
#include "ride/createnewprojectdlg.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include "ride/resources/icons.h"
#include "ride/wxutils.h"

//////////////////////////////////////////////////////////////////////////

#include "ride/generated/ui.h"

class CreateNewProjectDlg : public ui::CreateNewProject {
 public:
  CreateNewProjectDlg(wxWindow* parent);

  const wxString project_folder() const;
  const wxString project_name() const;

  wxString GetTarget() const;
  wxString GenerateCargoCommandline() const;

 protected:
  virtual void OnProjectNameEnter(wxCommandEvent& event);

  virtual void OnProjectNameChanged(wxCommandEvent& event);
  virtual void OnProjectFolderChanged(wxCommandEvent& event);
  virtual void OnBrowseProjectFolder(wxCommandEvent& event);
  virtual void OnCancel(wxCommandEvent& event);
  virtual void OnOk(wxCommandEvent& event);

 private:
  wxString GetVcsName() const;
  void UpdateTarget();
};

//////////////////////////////////////////////////////////////////////////

CreateNewProjectDlgHandler::CreateNewProjectDlgHandler(wxWindow* parent)
    : parent_(parent) {}

bool CreateNewProjectDlgHandler::ShowModal() {
  CreateNewProjectDlg dlg(parent_);
  const bool ret = wxID_OK == dlg.ShowModal();
  if (ret) {
    project_folder_ = dlg.project_folder();
    project_name_ = dlg.project_name();
    target_ = dlg.GetTarget();
    cargo_command_line_ = dlg.GenerateCargoCommandline();
  }
  return ret;
}

const wxString CreateNewProjectDlgHandler::project_folder() const {
  return project_folder_;
}

const wxString CreateNewProjectDlgHandler::project_name() const {
  return project_name_;
}

wxString CreateNewProjectDlgHandler::target() const { return target_; }

wxString CreateNewProjectDlgHandler::cargo_command_line() const {
  return cargo_command_line_;
}

//////////////////////////////////////////////////////////////////////////

enum ProjectTemplateType { PTT_UNKNOWN, PTT_BINARY, PTT_LIBRARY };

wxString GetValidDirectory(const wxString& dir) {
  if (dir.IsEmpty()) return "";
  wxDir filename(dir);
  const wxString full_path = filename.GetNameWithSep();
  return full_path;
}

ProjectTemplateType GetPtt(wxListCtrl* list) {
  const long selection =
      list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
  if (selection == -1) return PTT_UNKNOWN;
  wxUIntPtr data = list->GetItemData(selection);
  if (data == PTT_LIBRARY)
    return PTT_LIBRARY;
  else if (data == PTT_BINARY)
    return PTT_BINARY;
  else
    return PTT_UNKNOWN;
}

void AddItem(wxListCtrl* list, const wxString& text, int image,
             ProjectTemplateType type) {
  long i = list->InsertItem(0, "", image);
  list->SetItemColumnImage(i, 0, image);
  list->SetItem(i, 1, text);
  list->SetItemData(i, type);
}

CreateNewProjectDlg::CreateNewProjectDlg(wxWindow* parent)
    : ui::CreateNewProject(parent) {
  uiVcs->SetSelection(0);
  uiProjectfolder->SetValue(wxStandardPaths::Get().GetDocumentsDir());

  wxImageList* images = new wxImageList(16, 16);
  images->Add(wxIcon(create_app_xpm));
  images->Add(wxIcon(create_library_xpm));
  uiTemplates->AssignImageList(images, wxIMAGE_LIST_SMALL);

  uiTemplates->InsertColumn(0, "");
  uiTemplates->SetColumnWidth(0, 20);
  uiTemplates->InsertColumn(1, "Name", wxLIST_FORMAT_LEFT,
                            wxLIST_AUTOSIZE_USEHEADER);

  AddItem(uiTemplates, "Library", 1, PTT_LIBRARY);
  AddItem(uiTemplates, "Binary", 0, PTT_BINARY);

  uiTemplates->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
  uiProjectName->SetFocus();
}

void CreateNewProjectDlg::OnProjectNameEnter(wxCommandEvent& event) {
  OnOk(event);
}

void CreateNewProjectDlg::OnProjectNameChanged(wxCommandEvent& event) {
  UpdateTarget();
}

void CreateNewProjectDlg::OnProjectFolderChanged(wxCommandEvent& event) {
  UpdateTarget();
}

void CreateNewProjectDlg::OnBrowseProjectFolder(wxCommandEvent& event) {
  wxDirDialog dlg(NULL, "Choose project folder", uiProjectfolder->GetValue(),
                  wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
  if (wxID_OK != dlg.ShowModal()) return;
  uiProjectfolder->SetValue(dlg.GetPath());
}

wxString CreateNewProjectDlg::GetTarget() const {
  if (project_folder().IsEmpty()) return "Empty project folder";
  wxFileName target_cargo_file(project_folder());

  if (project_name().IsEmpty()) return "No valid project";
  target_cargo_file.AppendDir(project_name());
  target_cargo_file.SetName("Cargo");
  target_cargo_file.SetExt("toml");
  return target_cargo_file.GetFullPath();
}

void CreateNewProjectDlg::UpdateTarget() {
  uiTargetFolder->SetLabel(GetTarget());
}

void CreateNewProjectDlg::OnCancel(wxCommandEvent& event) {
  EndModal(wxID_CANCEL);
}

const wxString CreateNewProjectDlg::project_folder() const {
  return GetValidDirectory(uiProjectfolder->GetValue());
}

const wxString CreateNewProjectDlg::project_name() const {
  return uiProjectName->GetValue();
}

void CreateNewProjectDlg::OnOk(wxCommandEvent& event) {
  if (project_folder().IsEmpty() == false &&
      project_name().IsEmpty() == false && GetPtt(uiTemplates) != PTT_UNKNOWN) {
    EndModal(wxID_OK);
  } else {
    ShowError(this, "Please enter valid name and folder for the project",
              "Missing name and/or folder for project");
  }
}

wxString CreateNewProjectDlg::GetVcsName() const {
  const int selected_item = uiVcs->GetSelection();
  switch (selected_item) {
    case 0:
      return "none";
    case 1:
      return "git";
    case 2:
      return "hg";
    default:
      assert(0 && "Unknown selected VCS item");
      return "none";
  }
}

wxString CreateNewProjectDlg::GenerateCargoCommandline() const {
  const wxString template_cmd =
      GetPtt(uiTemplates) == PTT_BINARY ? "--bin " : "";
  const wxString travis_cmd = uiTravis->GetValue() ? "--travis " : "";
  const wxString vcs_name = GetVcsName();
  return wxString::Format("cargo new --vcs %s %s%s%s", vcs_name, template_cmd,
                          travis_cmd, project_name());
}
