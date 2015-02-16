#ifndef RIDE_FILEEDIT_H
#define RIDE_FILEEDIT_H

#include "ride/wx.h"
#include <wx/stc/stc.h>

class wxAuiNotebook;
class MainWindow;
class CompilerMessage;

class FileEdit : public wxControl {
public:
  FileEdit(wxAuiNotebook* anotebook, MainWindow* parent, const wxString& source, const wxString& file);
  const wxString& getFileName() const;
  void setSelection(int start_line, int start_index, int end_line, int end_index);
  void AddCompilerMessage(const CompilerMessage& mess);
  void ClearCompilerMessages();
  void Focus();

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
  void Find();
  void Replace();
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
  void ShowProperties();

public:
  bool canClose(bool canAbort);

  /** Event callback when a margin is clicked, used here for code folding */
  void OnMarginClick(wxStyledTextEvent& event);
  void OnCharAdded(wxStyledTextEvent& event);
  void OnUpdateUi(wxStyledTextEvent& event);
  void OnChanged(wxStyledTextEvent& event);

  void UpdateTextControl();

  ~FileEdit();
private:
  void updateFilename();
  void updateTitle();
  bool saveTo(const wxString& target);

  bool shouldBeSaved();
  void HighlightCurrentWord();

private:
  MainWindow* main;
  wxStyledTextCtrl* text;
  wxAuiNotebook* notebook;
  wxString filename;
  wxString docname;
  int m_LineNrMargin;

  Language* currentLanguage;

private:
  wxDECLARE_EVENT_TABLE();
};

#define FILE_PATTERN "All files (*.*)|*.*"

#endif  // RIDE_FILEEDIT_H
