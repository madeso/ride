// Copyright (2015) Gustav

#ifndef RIDE_FINDRESULTSCONTROL_H_
#define RIDE_FINDRESULTSCONTROL_H_

#include <ride/wx.h>
#include <wx/stc/stc.h>

class MainWindow;

/** The main control of the find results window/pane.
 * This is where the result of functions like 'find in files' are placed.
 */
class FindResultsControl : public wxStyledTextCtrl {
 public:
  explicit FindResultsControl(MainWindow* main);

  void UpdateStyle();
  const wxString GetContextLineContent();

 protected:
  void OnContextMenu(wxContextMenuEvent& event);
  void OnCopyThisCompilerMessage(wxCommandEvent& event);
  void OnClearCompilerOuput(wxCommandEvent& event);
  void OnSelectAll(wxCommandEvent& event);
  void OnCopy(wxCommandEvent& event);
  void OnDoubleClick(wxMouseEvent& event);
  void BindEvents();

 private:
  int context_positon_;
  MainWindow* main_;
};

#endif  // RIDE_FINDRESULTSCONTROL_H_
