#ifndef SETTINGS_DLG_H
#define SETTINGS_DLG_H

#include "ride/generated/ui.h"
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

  //////////////////////////////////////////////////////////////////////////
  // Style tab
  void OnOnlyFixedSysChanged(wxCommandEvent& event);
  void OnSelectedStyleChanged(wxCommandEvent& event);
  void OnStyleFontChanged(wxCommandEvent& event);
  void OnStyleCheckChanged(wxCommandEvent& event);
  void OnStyleTextChanged(wxCommandEvent& event);
  void OnStyleColorChanged(wxColourPickerEvent& event);

  //////////////////////////////////////////////////////////////////////////
  // Edit tab
  void OnCheckboxChanged(wxCommandEvent& event);
  void OnComboboxChanged(wxCommandEvent& event);
  void OnColorChanged(wxColourPickerEvent& event);
  void OnEditChanged(wxCommandEvent& event);
  void OnlyAllowNumberChars(wxKeyEvent& event);

private:
  MainWindow* main_window_;
  ride::Settings global_settings_;
  ride::Settings current_settings_;

  //////////////////////////////////////////////////////////////////////////
  // Style tab
  bool allow_send_style_to_main_;
  void UpdateStyleEnable();
  void SendStyleToMain();
  void StyleToGui(bool togui);
  void UpdateStyleFonts();
  void StyleUpdateFontDisplay();
  void StyleSaveSelectedIndex();

  //////////////////////////////////////////////////////////////////////////
  // Edit tab
  bool allow_send_edit_to_main_;
  void SendEditToMain();
  void EditToGui(bool togui);
};

#endif // SETTINGS_DLG_H
