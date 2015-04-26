#ifndef RIDE_FILEEDIT_H
#define RIDE_FILEEDIT_H

#include "ride/wx.h"
#include <wx/stc/stc.h>

class wxAuiNotebook;
class MainWindow;
class CompilerMessage;
class Language;

namespace ride {
  class Settings;
}

enum class ShowAutoCompleteAction {
  NO_FORCE // no forcing at all
  ,FORCE_SIMPLE // simple forcing
  ,FORCE_KEEP // forcing with no auto-hide
};

class FileEdit : public wxControl {
public:
  FileEdit(wxAuiNotebook* anotebook, MainWindow* parent, const wxString& file);
  const wxString& filename() const;
  void SetSelection(int start_line, int start_index, int end_line, int end_index);
  void GetSelection(int* start_line, int* start_index, int* end_line, int* end_index);
  void AddCompilerMessage(const CompilerMessage& mess);
  void ClearCompilerMessages();
  wxString GetLanguageName();

  bool CanClose(bool canAbort);
  void UpdateTextControl();
  void ReloadFileIfNeeded();
  void FileHasBeenRenamed(const wxString& new_path);

  void Find(wxStyledTextCtrl* output, const wxString& project);
  void Replace(wxStyledTextCtrl* output, const wxString& project);
  void FindInFiles(wxStyledTextCtrl* output, const wxString& project);
  void ReplaceInFiles(wxStyledTextCtrl* output, const wxString& project);

  void SetFocus();
  void SetFocusFromKbd();

  wxStyledTextCtrl* GetStc();
public:
  bool Save();
  bool SaveAs();
  void Undo();
  void Redo();
  void Cut();
  void Copy();
  void Paste();
  void Duplicate();
  void Delete();
  void MatchBrace();
  void SelectBrace();
  void GotoLine();
  void Indent();
  void UnIndent();
  void SelectAll();
  void SelectLine();
  void ToUpper();
  void ToLower();
  void MoveLinesUp();
  void MoveLinesDown();
  void OpenInOnlineDocumentation();
  void ShowProperties();

  void ShowAutocomplete(ShowAutoCompleteAction action = ShowAutoCompleteAction::FORCE_KEEP);

public:
  /** Event callback when a margin is clicked, used here for code folding */
  void OnMarginClick(wxStyledTextEvent& event);
  void OnCharAdded(wxStyledTextEvent& event);
  void OnUpdateUi(wxStyledTextEvent& event);
  void OnChanged(wxStyledTextEvent& event);

  ~FileEdit();
private:
  void UpdateFilename();
  void UpdateTitle();
  bool SaveTo(const wxString& target);

  bool ShouldBeSaved();
  void HighlightCurrentWord();
  void UpdateBraceMatching();

  wxString CalculateDocumentName() const;
  void LoadFile();
  void UpdateFileTime();

private:
  MainWindow* main_;
  wxStyledTextCtrl* text_;
  wxAuiNotebook* notebook_;
  wxString filename_;

  Language* current_language_;

  int highlight_current_word_last_start_position_;
  int highlight_current_word_last_end_position_;
  wxDateTime last_modification_time_;

private:
  wxDECLARE_EVENT_TABLE();
};

#endif  // RIDE_FILEEDIT_H
