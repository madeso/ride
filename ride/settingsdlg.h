#ifndef SETTINGS_DLG_H
#define SETTINGS_DLG_H

#include "ride/ui.h"
#include "ride/settings.h"

class MainWindow;

class SettingsDlg : public ui::Settings
{
public:
  SettingsDlg(wxWindow* parent, MainWindow* mainwindow);
protected:
  void OnApply(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);

  void OnCheckboxChanged(wxCommandEvent& event);
  void OnComboboxChanged(wxCommandEvent& event);
  void OnColorChanged(wxColourPickerEvent& event);
  void OnEditChanged(wxCommandEvent& event);

private:
  bool allowApply;

  void apply();
  void editToGui(bool togui);
  MainWindow* main;
  ride::Settings global;
  ride::Settings edit;
};

#endif // SETTINGS_DLG_H
