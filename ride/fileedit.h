#ifndef RIDE_FILEEDIT_H
#define RIDE_FILEEDIT_H

#include <wx/wx.h>
#include <wx/stc/stc.h>

class wxAuiNotebook;

class FileEdit : public wxControl {
public:
  FileEdit(wxAuiNotebook* anotebook, wxWindow* parent);

  bool canClose() const;

  /** Event callback when a margin is clicked, used here for code folding */
  void OnMarginClick(wxStyledTextEvent& event);

  ~FileEdit();

private:
  wxStyledTextCtrl* text;
  wxAuiNotebook* notebook;

private:
  wxDECLARE_EVENT_TABLE();
};

#endif  // RIDE_FILEEDIT_H
