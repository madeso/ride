#ifndef RIDE_OUTPUTCONTROL_H
#define RIDE_OUTPUTCONTROL_H

#include "ride/wx.h"
#include <wx/stc/stc.h>

class MainWindow;

class OutputControl : public wxStyledTextCtrl {
public:
  OutputControl(MainWindow* main);

  void UpdateStyle();
  const wxString GetContextLineContent();

protected:
  void OnContextMenu(wxContextMenuEvent& event);
  void OnCopyThisCompilerMessage(wxCommandEvent& event);
  void OnClearCompilerOuput(wxCommandEvent& event);
  void OnSelectAll(wxCommandEvent& event);
  void OnCopy(wxCommandEvent& event);
  void OnSearchForThisCompilerMessage(wxCommandEvent& event);
  void OnDoubleClick(wxMouseEvent& event);
  wxDECLARE_EVENT_TABLE();

private:
  MainWindow* main_;
  int context_positon_;
};

#endif  // RIDE_OUTPUTCONTROL_H
