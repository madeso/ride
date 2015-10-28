// Copyright (2015) Gustav

#include "ride/settingscommon.h"
#include "ride/mainwindow.h"
#include "ride/wxutils.h"

SettingsCommon::SettingsCommon(MainWindow* main_window)
    : main_window_(main_window) {
  global_settings_ = main_window_->settings();
  current_settings_ = global_settings_;
}

const ride::Settings& SettingsCommon::current_settings() const {
  return current_settings_;
}

ride::Settings* SettingsCommon::current_settings_mod() {
  return &current_settings_;
}

void SettingsCommon::UpdateMain() {
  for (int i = 0; i < 2; ++i) {
    main_window_->set_settings(current_settings_);
  }
}

void SettingsCommon::Revert() {
  current_settings_ = global_settings_;
  main_window_->set_settings(global_settings_);
}

void SettingsCommon::Apply(wxWindow* main) {
  main_window_->set_settings(current_settings_);
  if (false == SaveSettings(main, current_settings_)) {
    ShowError(main, "Failed to save settings", "Failed!");
  }
}
