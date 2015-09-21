// Copyright (2015) Gustav

#ifndef RIDE_FINDDLG_H_
#define RIDE_FINDDLG_H_

#include <ride/wx.h>

class FindResultsControl;
class MainWindow;

enum class FindAction { Find, Replace };

enum class FindScope { File, Project };

bool ShowFindDlg(MainWindow* main, const wxString& current_selection,
                 const wxString& current_file, const wxString root_folder,
                 FindResultsControl* output, FindAction find,
                 FindScope find_scope);

#endif  // RIDE_FINDDLG_H_
