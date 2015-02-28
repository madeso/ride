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
  if (project_folder().IsEmpty() == false || project_name().IsEmpty() == false) {
    wxMessageBox("Please enter name and folder for the project", "Missing name and/or folder for project", wxICON_ERROR, this);
  }
  else {
    EndModal(wxID_OK);
  }
}
