#ifndef RIDE_FINDCONTROL_H_
#define RIDE_FINDCONTROL_H_

#include "ride/wx.h"
#include <wx/stc/stc.h>

class MainWindow;

class FindControl : public wxStyledTextCtrl {
public:
  FindControl(MainWindow* main);

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

#endif // RIDE_FINDCONTROL_H_
