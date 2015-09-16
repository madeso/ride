// Copyright (2015) Gustav

#ifndef RIDE_RUNCONFIGURATIONSDLG_H_
#define RIDE_RUNCONFIGURATIONSDLG_H_

#include <ride/wx.h>

class MainWindow;
class Project;

bool DoRunConfigurationsDlg(wxWindow* parent, MainWindow* mainwindow,
                            Project* project);

#endif  // RIDE_RUNCONFIGURATIONSDLG_H_
