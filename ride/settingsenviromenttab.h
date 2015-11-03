// Copyright (2015) Gustav

#ifndef RIDE_SETTINGSENVIROMENTTAB_H_
#define RIDE_SETTINGSENVIROMENTTAB_H_

#include <ride/wx.h>
#include "ride/generated/ui.h"
#include "ride/settingscommon.h"

class MainWindow;

class SettingsEnviromentTab : public ui::SettingsEnviromentPanel {
 public:
  SettingsEnviromentTab(wxWindow* parent, SettingsCommon* common);

  void SettingsToGui(bool togui);

 protected:
  void OnCargo(wxCommandEvent& event);
  void OnRustc(wxCommandEvent& event);
  void OnRacer(wxCommandEvent& event);
  void OnProtoc(wxCommandEvent& event);
  void OnRefresh(wxCommandEvent& event);

 private:
  SettingsCommon* common_;
};

#endif  // RIDE_SETTINGSENVIROMENTTAB_H_
