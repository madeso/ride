// Copyright (2015) Gustav

#ifndef RIDE_OUTPUTCONTROL_H_
#define RIDE_OUTPUTCONTROL_H_

#include <ride/wx.h>
#include <wx/stc/stc.h>

class MainWindow;

class OutputControl : public wxControl {
 public:
  explicit OutputControl(MainWindow* main);

  void UpdateStyle();
  const wxString GetContextLineContent();

  void ClearOutput();
  void WriteLine(const wxString& str);

 protected:
  void OnContextMenu(wxContextMenuEvent& event);
  void OnRunThisCompilerMessage(wxCommandEvent& event);
  void OnCopyThisCompilerMessage(wxCommandEvent& event);
  void OnClearCompilerOuput(wxCommandEvent& event);
  void OnSelectAll(wxCommandEvent& event);
  void OnCopy(wxCommandEvent& event);
  void OnSearchForThisCompilerMessage(wxCommandEvent& event);
  void OnDoubleClick(wxMouseEvent& event);
  void BindEvents();

 private:
  MainWindow* main_;
  wxStyledTextCtrl* text_;
  int context_positon_;
};

#endif  // RIDE_OUTPUTCONTROL_H_
