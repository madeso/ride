// Copyright (2015) Gustav

#include "ride/settingsthemestab.h"

#include <vector>
#include <algorithm>

#include "ride/form.h"
#include "ride/mainwindow.h"
#include "ride/settings.h"
#include "ride/wxutils.h"
#include "ride/proto.h"
#include "ride/togui.h"

const wxString ThemeFunctions::ADD_TEXT = "Save current theme as ";
const wxString ThemeFunctions::EDIT_TEXT = "Please specify the new theme name";

SettingsThemesTab::SettingsThemesTab(wxWindow* parent, SettingsCommon* common,
                                     ToGuiSender* togui)
    : ui::SettingsThemesPanel(parent),
      main_(togui),
      common_(common),
      theme_list_(uiThemeList, this),
      allow_to_gui_(true) {
  theme_list_.Setup(uiThemeListAdd, uiThemeListRemove, uiThemeListChange,
                    uiThemeListUp, uiThemeListDown);
  ThemeToGui(true);
}

void SettingsThemesTab::ThemeToGui(bool togui) {
  if (!allow_to_gui_) return;
  theme_list_.ToGui(common_->current_settings_mod(), togui);
}

void SettingsThemesTab::OnThemeApplySelected(wxCommandEvent& event) {
  int selected = uiThemeList->GetSelection();
  if (selected == -1) {
    return;
  }
  common_->current_settings_mod()->set_allocated_fonts_and_colors(
      new ride::FontsAndColors(
          common_->current_settings().themes(selected).data()));

  allow_to_gui_ = false;
  main_->SendToGui(true);
  allow_to_gui_ = true;

  common_->UpdateMain();
}

void SettingsThemesTab::OnThemeExportSelected(wxCommandEvent& event) {
  int selected = uiThemeList->GetSelection();
  if (selected == -1) {
    return;
  }

  wxFileDialog saveFileDialog(this, _("Save theme"), "", "",
                              "Ride themes (*.ridetheme)|*.ridetheme",
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

  bool saved = SaveProtoBinary(common_->current_settings().themes(selected),
                               saveFileDialog.GetPath());
  if (saved == false) {
    ShowError(this, "Failed to save theme", "Failed to save");
  }
}

void SettingsThemesTab::OnThemeImport(wxCommandEvent& event) {
  wxFileDialog saveFileDialog(this, _("Open theme"), "", "",
                              "Ride themes (*.ridetheme)|*.ridetheme",
                              wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

  ride::Theme theme;
  bool saved = LoadProtoBinary(&theme, saveFileDialog.GetPath());
  if (saved == false) {
    ShowError(this, "Failed to import theme", "Failed to import");
    return;
  }

  ride::Theme* new_theme = common_->current_settings_mod()->add_themes();
  new_theme->CopyFrom(theme);
  ThemeToGui(true);
}

void SettingsThemesTab::OnAdd(wxCommandEvent& event) {
  if (false == theme_list_.Add(common_->current_settings_mod())) {
    return;
  }
  ThemeToGui(true);
}

void SettingsThemesTab::OnEdit(wxCommandEvent& event) {
  if (false == theme_list_.Edit(common_->current_settings_mod())) {
    return;
  }
  ThemeToGui(true);
}

void SettingsThemesTab::OnRemove(wxCommandEvent& event) {
  if (false == theme_list_.Remove(common_->current_settings_mod())) {
    return;
  }
  ThemeToGui(true);
}

void SettingsThemesTab::OnUp(wxCommandEvent& event) {
  if (false == theme_list_.Up(common_->current_settings_mod())) {
    return;
  }
  ThemeToGui(true);
}

void SettingsThemesTab::OnDown(wxCommandEvent& event) {
  if (false == theme_list_.Down(common_->current_settings_mod())) {
    return;
  }
  ThemeToGui(true);
}
