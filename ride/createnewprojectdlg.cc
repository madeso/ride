#include "ride/wx.h"
#include "ride/createnewprojectdlg.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>

CreateNewProjectDlg::CreateNewProjectDlg(wxWindow* parent) : ui::CreateNewProject(parent) {
  uiVcs->SetSelection(0);
  uiProjectfolder->SetValue(wxStandardPaths::Get().GetDocumentsDir());
}

void CreateNewProjectDlg::OnProjectNameChanged(wxCommandEvent& event) {
  UpdateTarget();
}

void CreateNewProjectDlg::OnProjectFolderChanged(wxCommandEvent& event) {
  UpdateTarget();
}

void CreateNewProjectDlg::OnBrowseProjectFolder(wxCommandEvent& event) {
  wxDirDialog dlg(NULL, "Choose project folder", uiProjectfolder->GetValue(), wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
  if (wxID_OK != dlg.ShowModal()) return;
  uiProjectfolder->SetValue(dlg.GetPath());
}

wxString CreateNewProjectDlg::GetTarget() const {
  if (project_folder().IsEmpty()) return "No valid folder";
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

const wxString CreateNewProjectDlg::project_folder() const { return uiProjectfolder->GetValue(); }
const wxString CreateNewProjectDlg::project_name() const { return uiProjectName->GetValue(); }

void CreateNewProjectDlg::OnOk(wxCommandEvent& event) {
  if (project_folder().IsEmpty() == false && project_name().IsEmpty() == false) {
    EndModal(wxID_OK);
  }
  else {
    wxMessageBox("Please enter name and folder for the project", "Missing name and/or folder for project", wxICON_ERROR, this);
  }
}

wxString CreateNewProjectDlg::GetVcsName() const {
  const int selected_item = uiVcs->GetSelection();
  switch (selected_item) {
  case 0: return "none";
  case 1: return "git";
  case 2: return "hg";
  default:
    assert(0 && "Unknown selected VCS item");
    return "none";
  }
}

wxString CreateNewProjectDlg::GenerateCargoCommandline() const {
  const wxString template_cmd = uiTemplate->GetSelection()==0 ? "--bin " : "";
  const wxString travis_cmd = uiTravis->GetValue() ? "--travis " : "";
  const wxString vcs_name = GetVcsName();
  return wxString::Format("cargo new --vcs %s %s%s%s", vcs_name, template_cmd, travis_cmd, project_name());
}
