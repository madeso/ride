// Copyright (2015) Gustav

#ifndef RIDE_SETTINGSWINDOWTAB_H_
#define RIDE_SETTINGSWINDOWTAB_H_

#include <ride/wx.h>
#include "ride/generated/ui.h"
#include "ride/settingscommon.h"

class MainWindow;

class SettingsWindowTab : public ui::SettingsWindowPanel {
 public:
  SettingsWindowTab(wxWindow* parent, SettingsCommon* common);

  void WindowToGui(bool togui);

 protected:
  void OnWindowColorChange(wxColourPickerEvent& event);
  void OnWindowComboChange(wxCommandEvent& event);
  void OnWindowText(wxCommandEvent& event);
  void OnWindowCheck(wxCommandEvent& event);

 private:
  SettingsCommon* common_;
  bool allow_send_window_to_main_;
  void SendWindowToMain();
};

#endif  // RIDE_SETTINGSWINDOWTAB_H_
