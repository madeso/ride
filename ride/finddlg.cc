#include "ride/finddlg.h"

#include "ride/wx.h"
#include "ride/generated/ui.h"
#include "ride/wxutils.h"

#include "wx/stc/stc.h"
#include "ride/settings.h"
#include <wx/dir.h>

class FindDlg : public ui::Find {
public:
  FindDlg(wxWindow* parent, const wxString& find, const ride::FindDlg& data);

  const wxString GetText() const {
    return uiFindText->GetValue();
  }

  const int GetFlags();

  void ToData(ride::FindDlg& data) const;

  wxStyledTextCtrl* GetStc() {
    return m_scintilla1;
  }

  bool LookInCurrentFile() const {
    return uiLookIn->GetSelection() == 0;
  }
  
protected:
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);
  void OnEnter(wxCommandEvent& event) {
    OnOk(event);
  }
};

FindDlg::FindDlg(wxWindow* parent, const wxString& find, const ride::FindDlg& data)
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


  uiIncludeSubFolders->SetValue(data.sub_folders());
  uiMatchCase->SetValue(data.match_case());
  uiMatchWholeWord->SetValue(data.match_whole_word());
  uiFindWordStart->SetValue(data.match_start());
  uiFileTypes->SetValue(data.file_types().c_str());
  uiFindTarget->SetSelection(data.target());
}

void FindDlg::ToData(ride::FindDlg& data) const {
  data.set_sub_folders(uiIncludeSubFolders->GetValue());
  data.set_match_case(uiMatchCase->GetValue());
  data.set_match_whole_word(uiMatchWholeWord->GetValue());
  data.set_match_start(uiFindWordStart->GetValue());
  data.set_file_types(uiFileTypes->GetValue());
  data.set_target(static_cast<::ride::FindDlgTarget>(uiFindTarget->GetSelection()));
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
  FindResult(const wxString& f, const wxString& co, const int sl, const int sc, const int el, const int ec)
    : file(f)
    , content(co)
    , start_line(sl)
    , start_col(sc)
    , end_line(el)
    , end_col(ec)
  {
  }

  wxString file;
  wxString content;
  int start_line;
  int start_col;
  int end_line;
  int end_col;
};

void FindInFiles(wxStyledTextCtrl* dlg, const wxString& file, const wxString& text, int flags, std::vector<FindResult>* res) {
  assert(res);
  dlg->LoadFile(file);
  int start_index = -1;
  while (true) {
    int end_index = 0;
    start_index = dlg->FindText(start_index+1, dlg->GetLength(), text, &end_index, flags);
    if (start_index == -1) return;
    const int start_line = dlg->LineFromPosition(start_index);
    const int start_col = start_index - dlg->PositionFromLine(start_line);
    const int end_line = dlg->LineFromPosition(end_index);
    const int end_col = end_index - dlg->PositionFromLine(end_line);
    res->push_back(FindResult(file, dlg->GetLine(start_line).Trim(true).Trim(false), start_line+1, start_col+1, end_line+1, end_col+1));
  }
}

bool ShowFindDlg(wxWindow* parent, const wxString& current_selection, const wxString& current_file, const wxString root_folder, wxStyledTextCtrl* output) {
  static ride::FindDlg find_dlg_data;
  FindDlg dlg(parent, current_selection, find_dlg_data);
  if (wxID_OK != dlg.ShowModal()) return false;
  dlg.ToData(find_dlg_data);

  std::vector<FindResult> results;

  // we can't create a styled ctrl so we cheat by having a 0x0 widget on the find dlg
  // and use that for searching...

  // todo: get the current stc from the main application so we can search in not saved files..

  wxString file_info = current_file;

  if (dlg.LookInCurrentFile()) {
    FindInFiles(dlg.GetStc(), current_file, dlg.GetText(), dlg.GetFlags(), &results);
  }
  else {
    wxArrayString files;
    const wxString pattern = find_dlg_data.file_types();
    const size_t count = wxDir::GetAllFiles(root_folder, &files, pattern, wxDIR_FILES);
    file_info = wxString::Format("%d files in %s", count, root_folder);
    for (const auto file : files) {
      FindInFiles(dlg.GetStc(), file, dlg.GetText(), dlg.GetFlags(), &results);
    }
  }
  

  const auto count = results.size();

  ClearOutput(output);
  WriteLine(output, wxString::Format("Searching for %s in %s", dlg.GetText(), file_info));
  WriteLine(output, wxString::Format("Found %d matches", count));
  WriteLine(output, "");
  for (auto res : results) {
    // try to format the same way rust related error looks like so we can reuse the parser code for both and get some synergy effects
    const wxString mess = wxString::Format("%s:%d : %d : %d : %d found: %s", res.file, res.start_line, res.start_col, res.end_line, res.end_col, res.content);
    WriteLine(output, mess);
  }

  return true;
}
