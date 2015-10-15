// Copyright (2015) Gustav

#include "ride/finddlg.h"

#include <ride/wx.h>

#include <wx/dir.h>

#include <wx/stc/stc.h>

#include <memory>
#include <vector>

#include "ride/generated/ui.h"

#include "ride/fileedit.h"
#include "ride/mainwindow.h"
#include "ride/settings.h"
#include "ride/wxutils.h"
#include "ride/findresultscontrol.h"

class FindDlg : public ui::Find {
 public:
  FindDlg(wxWindow* parent, const wxString& find, const ride::FindDlg& data,
          FindAction find_action, FindScope find_scope);

  const wxString GetText() const { return uiFindText->GetValue(); }

  const int GetFlags();

  void ToData(ride::FindDlg* data) const;

  wxStyledTextCtrl* GetStc() { return m_scintilla1; }

  bool LookInCurrentFile() const { return uiLookIn->GetSelection() == 0; }

  bool IsRecursive() const { return uiIncludeSubFolders->GetValue(); }

  const wxString getReplaceText() const { return uiReplaceText->GetValue(); }

  bool KeepFilesOpen() const { return uiKeepFilesOpen->GetValue(); }

 protected:
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);
  void OnEnter(wxCommandEvent& event) { OnOk(event); }
};

FindDlg::FindDlg(wxWindow* parent, const wxString& find,
                 const ride::FindDlg& data, FindAction find_action,
                 FindScope find_scope)
    : ui::Find(parent, wxID_ANY) {
  uiFindText->SetValue(find);
  uiFindText->SelectAll();
  uiFindText->SetFocus();

  uiLookIn->Append("Current file");
  uiLookIn->Append("This project");
  uiLookIn->SetSelection(
      static_cast<int>(find_scope));  // assume the enum is file -> project

  uiFindTarget->Append("Normal text");
  uiFindTarget->Append("Regex");
  uiFindTarget->Append("Posix");

  uiIncludeSubFolders->SetValue(data.sub_folders());
  uiMatchCase->SetValue(data.match_case());
  uiMatchWholeWord->SetValue(data.match_whole_word());
  uiFindWordStart->SetValue(data.match_start());
  uiFileTypes->SetValue(data.file_types().c_str());
  uiFindTarget->SetSelection(data.target());

  uiReplaceStatic->Enable(find_action == FindAction::Replace);
  uiReplaceText->Enable(find_action == FindAction::Replace);
}

void FindDlg::ToData(ride::FindDlg* data) const {
  data->set_sub_folders(uiIncludeSubFolders->GetValue());
  data->set_match_case(uiMatchCase->GetValue());
  data->set_match_whole_word(uiMatchWholeWord->GetValue());
  data->set_match_start(uiFindWordStart->GetValue());
  data->set_file_types(uiFileTypes->GetValue());
  data->set_target(
      static_cast<ride::FindDlgTarget>(uiFindTarget->GetSelection()));
}

void FindDlg::OnCancel(wxCommandEvent& event) { EndModal(wxID_CANCEL); }

void FindDlg::OnOk(wxCommandEvent& event) {
  if (uiFindText->GetValue().length() <= 0) {
    ShowInfo(this, "Need to enter search text", "Missing Search Text");
    return;
  }
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
  FindResult(const wxString& f, const wxString& co, const int sl, const int sc,
             const int el, const int ec)
      : file(f),
        content(co),
        start_line(sl),
        start_col(sc),
        end_line(el),
        end_col(ec) {}

  wxString file;
  wxString content;
  int start_line;
  int start_col;
  int end_line;
  int end_col;
};

class UndoRedo {
 public:
  explicit UndoRedo(wxStyledTextCtrl* s) : stc(s) {
    assert(stc);
    stc->BeginUndoAction();
  }
  ~UndoRedo() { stc->EndUndoAction(); }

  wxStyledTextCtrl* stc;
};

int FindInStc(wxStyledTextCtrl* stc, const wxString& file, const wxString& text,
              int flags, std::vector<FindResult>* res, FindAction find_action,
              const wxString& replaceText) {
  assert(res);
  assert(stc);
  std::unique_ptr<UndoRedo> undoredo;

  // there is only need to create a undo/redo object if we're replacing
  if (find_action == FindAction::Replace) {
    undoredo.reset(new UndoRedo(stc));
  }
  int next_index = 0;
  int count = 0;
  while (true) {
    int end_index = 0;
    const int start_index =
        FindStcText(stc, next_index, stc->GetLength(), text, flags, &end_index);
    if (start_index == -1) return count;
    assert(start_index != end_index);

    const int target_start_index = start_index;
    int target_end_index = end_index;

    // replace text
    if (find_action == FindAction::Replace) {
      stc->SetTargetStart(start_index);
      stc->SetTargetEnd(end_index);
      const bool useRegex =
          flags & wxSTC_FIND_REGEXP || flags & wxSTC_FIND_POSIX;
      const int change = useRegex ? stc->ReplaceTargetRE(replaceText)
                                  : stc->ReplaceTarget(replaceText);
      assert(change > 0);
      next_index = start_index + change;
      target_end_index = start_index + change;
    } else {
      next_index = end_index;
    }

    // add item to the result list
    const int start_line = stc->LineFromPosition(target_start_index);
    const int start_col =
        target_start_index - stc->PositionFromLine(start_line);
    const int end_line = stc->LineFromPosition(target_end_index);
    const int end_col = target_end_index - stc->PositionFromLine(end_line);
    res->push_back(
        FindResult(file, stc->GetLine(start_line).Trim(true).Trim(false),
                   start_line + 1, start_col + 1, end_line + 1, end_col + 1));
    ++count;
  }

  return count;
}

void FindInFiles(MainWindow* parent, wxStyledTextCtrl* fallback,
                 const wxString& file, const wxString& text, int flags,
                 std::vector<FindResult>* res, FindAction find_action,
                 const wxString& replaceText, bool keepFilesOpen) {
  FileEdit* edit = parent->GetFile(file);
  wxStyledTextCtrl* stc = NULL;
  if (edit) {
    stc = edit->GetStc();
  } else {
    fallback->LoadFile(file);

    if (find_action == FindAction::Replace && keepFilesOpen) {
      int found = FindStcText(fallback, 0, stc->GetLength(), text, flags, NULL);
      if (found > 0) {
        FileEdit* opened_edit = parent->OpenFile(file);
        stc = opened_edit->GetStc();
      }
    }
    if (stc == NULL) {
      stc = fallback;
    }
  }
  assert(stc);
  int count = FindInStc(stc, file, text, flags, res, find_action, replaceText);

  if (find_action == FindAction::Replace && count > 0 && stc == fallback) {
    if (false == stc->SaveFile(file)) {
      ShowError(parent, "Failed to save after replace!", "Error saving!");
    }
  }
}

bool ShowFindDlg(MainWindow* parent, const wxString& current_selection,
                 const wxString& current_file, const wxString root_folder,
                 FindResultsControl* output, FindAction find_action,
                 FindScope find_scope) {
  static ride::FindDlg find_dlg_data;
  FindDlg dlg(parent, current_selection, find_dlg_data, find_action,
              find_scope);

  if (wxID_OK != dlg.ShowModal()) return false;
  dlg.ToData(&find_dlg_data);

  std::vector<FindResult> results;
  wxString file_info = current_file;

  // we can't create a styled ctrl so we cheat by having a 0x0 widget on the
  // find dlg
  // and use that for searching/replacing...
  wxStyledTextCtrl* fallback = dlg.GetStc();

  if (dlg.LookInCurrentFile()) {
    FindInFiles(parent, fallback, current_file, dlg.GetText(), dlg.GetFlags(),
                &results, find_action, dlg.getReplaceText(),
                dlg.KeepFilesOpen());
  } else {
    wxArrayString files;
    const std::vector<wxString> patterns =
        Split(find_dlg_data.file_types(), ";");
    size_t count = 0;
    for (const auto pattern : patterns) {
      count += wxDir::GetAllFiles(
          root_folder, &files, pattern,
          dlg.IsRecursive() ? wxDIR_FILES | wxDIR_DIRS : wxDIR_FILES);
    }

    file_info = wxString::Format("%d files in %s", count, root_folder);
    for (const auto file : files) {
      FindInFiles(parent, fallback, file, dlg.GetText(), dlg.GetFlags(),
                  &results, find_action, dlg.getReplaceText(),
                  dlg.KeepFilesOpen());
    }
  }

  const auto count = results.size();

  output->ClearOutput();

  const wxString find_text =
      find_action == FindAction::Find
          ? wxString::Format("Searched for '%s'", dlg.GetText())
          : wxString::Format("Replaced '%s' with '%s'", dlg.GetText(),
                             dlg.getReplaceText());
  output->WriteLine(wxString::Format("%s in %s", find_text, file_info));
  output->WriteLine(wxString::Format(
      "%s %d matches", find_action == FindAction::Find ? "Found" : "Replaced",
      count));
  output->WriteLine("");
  for (auto res : results) {
    // try to format the same way rust related error looks like so we can reuse
    // the parser code for both and get some synergy effects
    const wxString mess = wxString::Format(
        "%s:%d : %d : %d : %d %s: %s", res.file, res.start_line, res.start_col,
        res.end_line, res.end_col,
        find_action == FindAction::Find ? "found" : "replaced", res.content);
    output->WriteLine(mess);
  }

  return true;
}
