#ifndef SETTINGS_DLG_H
#define SETTINGS_DLG_H

#include "ride/ui.h"
#include "ride/settings.h"

class MainWindow;

class SettingsDlg : public ui::Settings
{
public:
  SettingsDlg(wxWindow* parent, MainWindow* mainwindow);
  void PrepareForShow();
protected:
  void OnApply(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);
private:
  void editToGui(bool togui);
  MainWindow* main;
  ::Settings global;
  ::Settings edit;
};

#endif // SETTINGS_DLG_H
