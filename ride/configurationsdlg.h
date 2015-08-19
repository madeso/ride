// Copyright (2015) Gustav

#ifndef RIDE_CONFIGURATIONSDLG_H_
#define RIDE_CONFIGURATIONSDLG_H_

#include <ride/wx.h>

class MainWindow;
class Project;

bool DoConfigurationsDlg(wxWindow* parent, MainWindow* mainwindow,
                         Project* project);

#endif  // RIDE_CONFIGURATIONSDLG_H_
