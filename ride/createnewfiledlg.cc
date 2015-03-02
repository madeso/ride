#include "ride/wx.h"
#include "ride/createnewfiledlg.h"
#include <wx/filename.h>
#include <wx/dir.h>
#include "ride/resources/icons.h"

void AddFileTemplate(wxListCtrl* list, const wxString& text, int image)
{
  long i = list->InsertItem(0, "", image);
  list->SetItemColumnImage(i, 0, image);
  list->SetItem(i, 1, text);
  list->SetItem(i, 2, "Rust");
  // list->SetItemData(i, type);
}

CreateNewFileDlg::CreateNewFileDlg(wxWindow* parent, const wxString& fodler_hint) : ui::CreateNewFile(parent) {
  wxImageList* images = new wxImageList(16,16);
  images->Add(wxIcon(file_normal_xpm));
  uiTemplates->AssignImageList(images, wxIMAGE_LIST_SMALL);
  
  const long icon_index = uiTemplates->InsertColumn(0, "Icon");
  const long language_index = uiTemplates->InsertColumn(2, "Language");
  const long name_index = uiTemplates->InsertColumn(1, "Name");
  uiTemplates->SetColumnWidth(icon_index, 20);
  uiTemplates->SetColumnWidth(language_index, 60);
  uiTemplates->SetColumnWidth(name_index, 200);

  AddFileTemplate(uiTemplates, "Empty Rust", 0);
  AddFileTemplate(uiTemplates, "Rust class", 0);

  uiPath->SetValue(fodler_hint);

  uiTemplates->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
  uiName->SetFocus();
}

void CreateNewFileDlg::OnNameEnter(wxCommandEvent& event) {
  OnOk(event);
}

void CreateNewFileDlg::OnCancel(wxCommandEvent& event) {
  EndModal(wxID_CANCEL);
}

void CreateNewFileDlg::OnOk(wxCommandEvent& event) {
  EndModal(wxID_OK);
}
