#include "ride/wxutils.h"
#include "ride/wx.h"
#include <wx/listctrl.h>
#include <wx/stc/stc.h>
#include <wx/tokenzr.h>

void WriteLine(wxStyledTextCtrl* stc, const wxString& str) {
  stc->SetReadOnly(false);
  stc->AppendText(str);
  stc->AppendText("\n");
  stc->SetReadOnly(true);
}

void ClearOutput(wxStyledTextCtrl* stc) {
  stc->SetReadOnly(false);
  stc->SetText(wxEmptyString);
  stc->SetReadOnly(true);
}

wxPoint GetContextEventPosition(const wxContextMenuEvent& event) {
  wxPoint ret = event.GetPosition();
  // according to the documentation: http://docs.wxwidgets.org/trunk/classwx_context_menu_event.html#a291e3437b4bf913128ea14e511d161cb
  if (ret == wxDefaultPosition) {
    return wxGetMousePosition();
  }
  else {
    return ret;
  }
}

void AppendEnabled(wxMenu& menu, int id, const wxString& text, bool enabled) {
  menu.Append(id, text);
  menu.Enable(id, enabled);
}

std::vector<wxString> Split(const wxString& str, const wxString& c) {
  std::vector<wxString> ret;
  wxStringTokenizer tokenizer(str, c);
  while (tokenizer.HasMoreTokens()) {
    wxString token = tokenizer.GetNextToken();
    ret.push_back(token);
  }
  return ret;
}

std::vector<wxString> RemoveEmptyStrings(const std::vector<wxString>& v) {
  std::vector<wxString> ret;
  ret.reserve(v.size());
  for (const wxString& s : v) {
    if (s.IsEmpty()) continue;
    else ret.push_back(s);
  }
  return ret;
}

void SetSelection(wxListCtrl* list, long item, bool select){
  if (select) {
    list->SetItemState(item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
  }
  else {
    list->SetItemState(item, 0, wxLIST_STATE_SELECTED);
  }
}

std::vector<long> GetSelection(wxListCtrl* listctrl) {
  std::vector<long> ret;
  long item = -1;
  while (true)
  {
    item = listctrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item == -1)
      break;
    ret.push_back(item);
  }
  return ret;
}

void ClearSelection(wxListCtrl* listctrl) {
  long item = -1;
  while (true)
  {
    item = listctrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item == -1)
      break;
    SetSelection(listctrl, item, false);
  }
}

DialogResult CDL(int dl) {
  switch (dl) {
  case wxID_YES:
    return DialogResult::YES;
  case wxID_NO:
    return DialogResult::NO;
  case wxID_CANCEL:
    return DialogResult::CANCEL;
  default:
    assert(0 && "Invalid dialog result");
    return DialogResult::CANCEL;
  }
}

DialogResult ShowYesNo(wxWindow* self, const wxString& caption,
  const wxMessageDialogBase::ButtonLabel& yes_button,
  const wxMessageDialogBase::ButtonLabel& no_button,
  const wxString& title_ok, const wxString title_error) {

  wxMessageDialog dlg(self, _(""), caption, wxYES_NO | wxICON_QUESTION);
  const bool label_change_ok = dlg.SetYesNoLabels(yes_button, no_button);
  dlg.SetMessage(label_change_ok ? title_ok : title_error);
  return CDL(dlg.ShowModal());
}

DialogResult ShowYesNoCancel(wxWindow* self, const wxString& caption,
  const wxMessageDialogBase::ButtonLabel& yes_button,
  const wxMessageDialogBase::ButtonLabel& no_button,
  const wxMessageDialogBase::ButtonLabel& cancel_button,
  const wxString& title_ok, const wxString title_error) {

  wxMessageDialog dlg(self, _(""), caption, wxYES_NO | wxCANCEL | wxICON_QUESTION);
  const bool label_change_ok = dlg.SetYesNoCancelLabels(yes_button, no_button, cancel_button);
  dlg.SetMessage(label_change_ok ? title_ok : title_error);
  return CDL(dlg.ShowModal());
}

void ShowInfo(wxWindow* self, const wxString& message, const wxString& title) {
  wxMessageBox(message, title, wxICON_INFORMATION | wxOK | wxCENTRE, self);
}

void ShowWarning(wxWindow* self, const wxString& message, const wxString& title) {
  wxMessageBox(message, title, wxICON_WARNING | wxOK | wxCENTRE, self);
}

void ShowError(wxWindow* self, const wxString& message, const wxString& title) {
  wxMessageBox(message, title, wxICON_ERROR | wxOK | wxCENTRE, self);
}
