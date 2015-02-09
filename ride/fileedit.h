#ifndef RIDE_FILEEDIT_H
#define RIDE_FILEEDIT_H

#include "ride/wx.h"
#include <wx/stc/stc.h>

class wxAuiNotebook;
class MainWindow;

class FileEdit : public wxControl {
public:
  FileEdit(wxAuiNotebook* anotebook, MainWindow* parent, const wxString& source, const wxString& file);

  bool canClose(bool canAbort);
  bool save();
  bool saveAs();

  /** Event callback when a margin is clicked, used here for code folding */
  void OnMarginClick(wxStyledTextEvent& event);
  void OnTextChanged(wxStyledTextEvent& event);

  void UpdateTextControl();

  ~FileEdit();
private:
  void updateFilename();
  void updateTitle();
  bool saveTo(const wxString& target);

  bool shouldBeSaved();

private:
  MainWindow* main;
  wxStyledTextCtrl* text;
  wxAuiNotebook* notebook;
  wxString filename;
  wxString docname;
  bool dirty;
  int m_LineNrMargin;

  Language* currentLanguage;

private:
  wxDECLARE_EVENT_TABLE();
};

#define FILE_PATTERN "All files (*.*)|*.*"

#endif  // RIDE_FILEEDIT_H
