#include "ride/deletefolderdlg.h"

namespace {
  static bool DefaultFull = true;
  static bool DefaultRecursive = true;
}

DeleteFolderDlg::DeleteFolderDlg(wxWindow* parent, const wxString& message, const wxString& title)
  : ui::DeleteFolder(parent, wxID_ANY, title)
{
  uiRemoveFileDescription->SetLabel(message);
  set_full(DefaultFull);
  set_recursive(DefaultRecursive);
}

bool DeleteFolderDlg::full() const {
  return uiFull->GetValue();
}

bool DeleteFolderDlg::recursive() const {
  return uiRecursive->GetValue();
}

void DeleteFolderDlg::set_full(bool new_full) const {
  uiFull->SetValue(new_full);
}

void DeleteFolderDlg::set_recursive(bool new_recursive) const {
  uiRecursive->SetValue(new_recursive);
}

void DeleteFolderDlg::OnNo(wxCommandEvent& event) {
  EndModal(wxID_NO);
}

void DeleteFolderDlg::OnYes(wxCommandEvent& event) {
  DefaultFull = full();
  DefaultRecursive = recursive();
  EndModal(wxID_YES);
}
