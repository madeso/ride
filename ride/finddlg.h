#ifndef RIDE_FINDDLG_H
#define RIDE_FINDDLG_H

#include "ride/wx.h"

class wxStyledTextCtrl;
class MainWindow;

enum class FindAction {
  Find, Replace
};

bool ShowFindDlg(MainWindow* main, const wxString& current_selection, const wxString& current_file,
  const wxString root_folder, wxStyledTextCtrl* output, FindAction find);

#endif  // RIDE_FINDDLG_H
