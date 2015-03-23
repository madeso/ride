#include "ride/finddlg.h"

#include "ride/wx.h"
#include "ride/generated/ui.h"
#include "ride/wxutils.h"

#include "wx/stc/stc.h"

// based on http://docs.wholetomato.com/default.asp?W193

class FindDlg : public ui::Find {
public:
  FindDlg(wxWindow* parent, const wxString& find);

  const wxString GetText() const {
    return uiFindText->GetValue();
  }

  const int GetFlags();

  wxStyledTextCtrl* GetStc()           {
    return m_scintilla1;
  }


protected:
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);
};

FindDlg::FindDlg(wxWindow* parent, const wxString& find)
  : ui::Find(parent, wxID_ANY)
{
  uiFindText->SetValue(find);
  uiFindText->SelectAll();
  uiFindText->SetFocus();

  uiLookIn->Append("Current file");
  uiLookIn->Append("This project");
  uiLookIn->SetSelection(0);

  uiFindTarget->Append("Normal text");
  uiFindTarget->Append("Regex");
  uiFindTarget->Append("Posix");
  uiFindTarget->SetSelection(0);
}

void FindDlg::OnCancel(wxCommandEvent& event) {
  EndModal(wxID_CANCEL);
}

void FindDlg::OnOk(wxCommandEvent& event) {
  EndModal(wxID_OK);
}

const int FindDlg::GetFlags() {
  int ret = 0;

  if (uiMatchCase->GetValue()) {
    ret |= wxSTC_FIND_MATCHCASE;
  }

  if (uiMatchWholeWord->GetValue()) {
    ret |= wxSTC_FIND_WHOLEWORD;
  }

  if (uiFindWordStart->GetValue()) {
    ret |= wxSTC_FIND_WORDSTART;
  }

  switch (uiFindTarget->GetSelection()) {
  case 0:
    break;
  case 1:
    ret |= wxSTC_FIND_REGEXP;
    break;
  case 2:
    ret |= wxSTC_FIND_POSIX;
    break;
  default:
    assert(false && "invalid selection");
    break;
  }

  return ret;
}

struct FindResult {
  FindResult(const wxString& f, const wxString& co, const int l, const int c)
    : file(f)
    , content(co)
    , line_number(l)
    , column_number(c)
  {
  }

  wxString file;
  wxString content;
  int line_number;
  int column_number;
};

void FindInFiles(wxStyledTextCtrl* dlg, const wxString& file, const wxString& text, int flags, std::vector<FindResult>* res) {
  assert(res);
  dlg->LoadFile(file);
  int index = -1;
  while (true) {
    index = dlg->FindText(index+1, dlg->GetLength(), text, flags);
    if (index == -1) return;
    const int line = dlg->LineFromPosition(index);
    const int line_start = dlg->PositionFromLine(line);
    res->push_back(FindResult(file, dlg->GetLine(line), line, index-line_start));
  }
}

bool ShowFindDlg(wxWindow* parent, const wxString& current_selection, const wxString& current_file, const wxString root_folder) {
  FindDlg dlg(parent, current_selection);
  if (wxID_OK != dlg.ShowModal()) return false;

  std::vector<FindResult> res;

  // we can't create a styled ctrl so we cheat by having a 0x0 widget on the find dlg
  // and use that for searching...

  // todo: get the current stc from the main application so we can search in not saved files..

  FindInFiles(dlg.GetStc(), current_file, dlg.GetText(), dlg.GetFlags(), &res);

  // todo: output search result on a search result output pane

  return true;
}
