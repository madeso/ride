// Copyright (2015) Gustav

#ifndef RIDE_SETTINGSINDICATORTAB_H_
#define RIDE_SETTINGSINDICATORTAB_H_

#include <ride/wx.h>
#include "ride/generated/ui.h"
#include "ride/settingscommon.h"

class MainWindow;

class SettingsIndicatorTab : public ui::SettingsIndicatorsPanel {
 public:
  SettingsIndicatorTab(wxWindow* parent, SettingsCommon* common);

  void IndicatorToGui(bool togui);

 protected:
  void OnIndicatorListChanged(wxCommandEvent& event);
  void OnIndicatorCombobox(wxCommandEvent& event);
  void OnIndicatorColor(wxColourPickerEvent& event);
  void OnIndicatorCheckbox(wxCommandEvent& event);
  void OnIndicatorText(wxCommandEvent& event);

 private:
  SettingsCommon* common_;
  //////////////////////////////////////////////////////////////////////////
  // Indicator tab
  bool allow_send_indicator_to_main_;
  void SendIndicatorToMain();
};

#endif  // RIDE_SETTINGSINDICATORTAB_H_
