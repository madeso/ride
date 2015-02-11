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

  void OnOnlyFixedSysChanged(wxCommandEvent& event);

  void OnSelectedStyleChanged(wxCommandEvent& event);
  void OnStyleFontChanged(wxCommandEvent& event);
  void OnStyleCheckChanged(wxCommandEvent& event);
  void OnStyleComboboxChanged(wxCommandEvent& event);
  void OnStyleColorChanged(wxColourPickerEvent& event);

private:
  bool allowStyleChange;
  void styleChanged();
  void styleToGui(bool togui);
  bool allowApply;
  void updateFonts();
  void updateFontDisplay();

  void apply();
  void editToGui(bool togui);
  MainWindow* main;
  ride::Settings global;
  ride::Settings edit;
};

#endif // SETTINGS_DLG_H
