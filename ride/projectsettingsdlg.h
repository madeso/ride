#ifndef PROJECTSETTINGS_DLG_H
#define PROJECTSETTINGS_DLG_H

#include "ride/wx.h"

class MainWindow;
class Project;

void DoProjectSettingsDlg(wxWindow* parent, MainWindow* mainwindow,
                          Project* project);

#endif  // SETTINGS_DLG_H
