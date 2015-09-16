// Copyright (2015) Gustav

#ifndef RIDE_BUILDCONFIGURATIONSDLG_H_
#define RIDE_BUILDCONFIGURATIONSDLG_H_

#include <ride/wx.h>

class MainWindow;
class Project;

bool DoBuildConfigurationsDlg(wxWindow* parent, MainWindow* mainwindow,
                              Project* project, int selected);

#endif  // RIDE_BUILDCONFIGURATIONSDLG_H_
