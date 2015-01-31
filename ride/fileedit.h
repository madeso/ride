#ifndef RIDE_FILEEDIT_H
#define RIDE_FILEEDIT_H

#include <wx/wx.h>
#include <wx/stc/stc.h>

class wxAuiNotebook;

class FileEdit : public wxControl {
public:
  FileEdit(wxAuiNotebook* anotebook, wxWindow* parent, const wxString& source, const wxString& file);

  bool canClose();

  /** Event callback when a margin is clicked, used here for code folding */
  void OnMarginClick(wxStyledTextEvent& event);
  void OnTextChanged(wxStyledTextEvent& event);

  ~FileEdit();

private:
  void updateFilename();
  void updateTitle();

private:
  wxStyledTextCtrl* text;
  wxAuiNotebook* notebook;
  wxString filename;
  wxString docname;
  bool dirty;

private:
  wxDECLARE_EVENT_TABLE();
};

#endif  // RIDE_FILEEDIT_H
