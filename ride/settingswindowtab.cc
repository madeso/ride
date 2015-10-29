// Copyright (2015) Gustav

#include "ride/settingswindowtab.h"

#include <vector>
#include <algorithm>

#include "ride/form.h"
#include "ride/mainwindow.h"
#include "ride/settings.h"
#include "ride/wxutils.h"

SettingsWindowTab::SettingsWindowTab(wxWindow* parent, SettingsCommon* common)
    : ui::SettingsWindowPanel(parent),
      common_(common),
      allow_send_window_to_main_(false) {
  WindowToGui(true);
}

void SettingsWindowTab::SendWindowToMain() {
  if (allow_send_window_to_main_ == false) {
    return;
  }
  WindowToGui(false);
  common_->UpdateMain();
}

void SettingsWindowTab::WindowToGui(bool togui) {
  ride::Settings& current_settings_ = *common_->current_settings_mod();

  ride::FontsAndColors fonts_and_colors = current_settings_.fonts_and_colors();

  DIALOG_DATAX(fonts_and_colors, dock_background, uiWindowDockCommonBackground);
  DIALOG_DATAX(fonts_and_colors, dock_sash, uiWindowDockCommonSash);
  DIALOG_DATAX(fonts_and_colors, dock_active_caption,
               uiWindowDockActiveCaption);
  DIALOG_DATAX(fonts_and_colors, dock_active_caption_gradient,
               uiWindowDockActiveGradient);
  DIALOG_DATAX(fonts_and_colors, dock_inactive_caption,
               uiWindowDockInactiveCaption);
  DIALOG_DATAX(fonts_and_colors, dock_inactive_caption_gradient,
               uiWindowDockInactiveGradient);
  DIALOG_DATAX(fonts_and_colors, dock_active_caption_text,
               uiWindowDockActiveText);
  DIALOG_DATAX(fonts_and_colors, dock_inactive_caption_text,
               uiWindowDockInactiveText);
  DIALOG_DATAX(fonts_and_colors, dock_border, uiWindowDockCommonBorder);
  DIALOG_DATAX(fonts_and_colors, dock_gripper, uiWindowDockCommonGripper);
  DIALOG_DATAX(fonts_and_colors, tab_background, uiWindowTabCommonBackground);
  DIALOG_DATAX(fonts_and_colors, tab_border, uiWindowTabCommonBorder);
  DIALOG_DATAX(fonts_and_colors, tab_sash, uiWindowTabCommonSash);
  DIALOG_DATAX(fonts_and_colors, tab_active_tab, uiWindowTabActiveTab);
  DIALOG_DATAX(fonts_and_colors, tab_inactive_tab, uiWindowTabInactiveTab);
  DIALOG_DATAX(fonts_and_colors, tab_active_border, uiWindowTabActiveBorder);
  DIALOG_DATAX(fonts_and_colors, tab_inactive_border,
               uiWindowTabInactiveBorder);
  DIALOG_DATAX(fonts_and_colors, tab_active_text, uiWindowTabActiveText);
  DIALOG_DATAX(fonts_and_colors, tab_inactive_text, uiWindowTabInactiveText);
  DIALOG_DATAX(fonts_and_colors, statusbar_shadow, uiWindowStatusbarShadow);
  DIALOG_DATAX(fonts_and_colors, statusbar_highlight,
               uiWindowStatusbarHighlight);
  DIALOG_DATAX(fonts_and_colors, statusbar_foreground,
               uiWindowStatusbarForeground);
  DIALOG_DATAX(fonts_and_colors, statusbar_background,
               uiWindowStatusbarBackground);

  /*
  optional StatusbarStyle statusbar_style	    = 1019 [
  */

  DIALOG_DATA(current_settings_, switcher_vs_focus, uiSwitcherVsFocus, );
  DIALOG_DATA(current_settings_, switcher_row_count, uiSwitcherItemsPerRow,
              _I32);
  DIALOG_DATA(current_settings_, switcher_item_maxwidth, uiSwitcherItemMaxWidth,
              _I32);
  DIALOG_DATA(current_settings_, switcher_item_maxheight,
              uiSwitcherItemMaxHeight, _I32);
  DIALOG_DATA(current_settings_, switcher_text_margin_x, uiSwitcherTextMarginX,
              _I32);
  DIALOG_DATA(current_settings_, switcher_text_margin_y, uiSwitcherTextMarginY,
              _I32);
  DIALOG_DATA(current_settings_, switcher_x_margin, uiSwitcherXMargin, _I32);
  DIALOG_DATA(current_settings_, switcher_y_margin, uiSwitcherYMargin, _I32);
  DIALOG_DATA(current_settings_, switcher_row_spacing, uiSwitcherRowSpacing,
              _I32);
  DIALOG_DATA(current_settings_, switcher_col_spacing, uiSwitcherColSpacing,
              _I32);
  DIALOG_DATA(current_settings_, switcher_dlg_main_border, uiSwitcherMainBorder,
              _I32);
  DIALOG_DATA(current_settings_, switcher_dlg_item_border, uiSwitcherItemBorder,
              _I32);
  DIALOG_DATA(current_settings_, switcher_min_width, uiSwitcherMinWidth, _I32);
  DIALOG_DATA(current_settings_, switcher_min_height, uiSwitcherMinHeight,
              _I32);

  DIALOG_DATAX(fonts_and_colors, switcher_background_color,
               uiSwitcherItemBackground);
  DIALOG_DATAX(fonts_and_colors, switcher_text_color, uiSwitcherItemText);
  DIALOG_DATAX(fonts_and_colors, switcher_selection_color, uiSwitcherSelection);
  DIALOG_DATAX(fonts_and_colors, switcher_selection_outline_color,
               uiSwitcherSelectionOutline);
  DIALOG_DATAX(fonts_and_colors, switcher_dialog_color, uiSwitcherDialog);
  DIALOG_DATAX(fonts_and_colors, switcher_base_color, uiSwitcherBackground);

  if (togui == false) {
    current_settings_.set_allocated_fonts_and_colors(
        Allocate(fonts_and_colors));
  }
}
void SettingsWindowTab::OnWindowColorChange(wxColourPickerEvent& event) {
  assert(this);
  SendWindowToMain();
}
void SettingsWindowTab::OnWindowComboChange(wxCommandEvent& event) {
  assert(this);
  SendWindowToMain();
}

void SettingsWindowTab::OnWindowText(wxCommandEvent& event) {
  assert(this);
  SendWindowToMain();
}
void SettingsWindowTab::OnWindowCheck(wxCommandEvent& event) {
  assert(this);
  SendWindowToMain();
}
