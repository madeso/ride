// Copyright (2015) Gustav

#ifndef RIDE_SETTINGSFONTSTAB_H_
#define RIDE_SETTINGSFONTSTAB_H_

#include <ride/wx.h>
#include "ride/generated/ui.h"
#include "ride/settingscommon.h"

class MainWindow;

class SettingsFontsTab : public ui::SettingsFontsPanel {
 public:
  SettingsFontsTab(wxWindow* parent, SettingsCommon* common);

  void StyleToGui(bool togui);

 protected:
  virtual void OnlyAllowNumberChars(wxKeyEvent& event);

  void OnOnlyFixedSysChanged(wxCommandEvent& event);
  void OnSelectedStyleChanged(wxCommandEvent& event);
  void OnStyleFontChanged(wxCommandEvent& event);
  void OnStyleCheckChanged(wxCommandEvent& event);
  void OnStyleTextChanged(wxCommandEvent& event);
  void OnStyleColorChanged(wxColourPickerEvent& event);

 private:
  SettingsCommon* common_;
  bool allow_send_style_to_main_;

  void UpdateStyleEnable();
  void SendStyleToMain();
  void UpdateStyleFonts();
  void StyleUpdateFontDisplay();
  void StyleSaveSelectedIndex();
};

#endif  // RIDE_SETTINGSFONTSTAB_H_
