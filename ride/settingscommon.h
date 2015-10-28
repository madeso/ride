// Copyright (2015) Gustav

#ifndef RIDE_SETTINGSCOMMON_H_
#define RIDE_SETTINGSCOMMON_H_

#include "ride/settings.h"

class MainWindow;

class SettingsCommon {
 public:
  explicit SettingsCommon(MainWindow* main_window);

  const ride::Settings& current_settings() const;
  ride::Settings* current_settings_mod();

  void UpdateMain();
  void Revert();
  void Apply(wxWindow* main);

 private:
  MainWindow* main_window_;
  ride::Settings global_settings_;
  ride::Settings current_settings_;
};

#endif  // RIDE_SETTINGSCOMMON_H_
