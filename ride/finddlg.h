#ifndef RIDE_FINDDLG_H
#define RIDE_FINDDLG_H

#include "ride/wx.h"

class wxStyledTextCtrl;
class MainWindow;

enum class FindAction {
  Find, Replace
};

enum class FindScope {
  File, Project
};

bool ShowFindDlg(MainWindow* main, const wxString& current_selection, const wxString& current_file,
  const wxString root_folder, wxStyledTextCtrl* output, FindAction find, FindScope find_scope);

#endif  // RIDE_FINDDLG_H
