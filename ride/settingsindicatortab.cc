// Copyright (2015) Gustav

#include "ride/settingsindicatortab.h"

#include <ride/wx.h>

#include <vector>
#include <algorithm>

#include "ride/generated/ui.h"

#include "ride/form.h"
#include "ride/mainwindow.h"
#include "ride/wxutils.h"
#include "ride/guilist.h"
#include "ride/proto.h"
#include "ride/settingscommon.h"
#include "ride/settingsfontstab.h"

void ToGui(ride::IndicatorStyle data, wxComboBox* gui) {
  gui->SetSelection(static_cast<int>(data));
}

ride::IndicatorStyle ToData_IS(wxComboBox* gui) {
  RETURN_COMBOBOX_VALUE(IndicatorStyle, gui->GetSelection());
}

void SettingsIndicatorTab::OnIndicatorListChanged(wxCommandEvent& event) {
  allow_send_indicator_to_main_ = false;
  IndicatorToGui(true);
  allow_send_indicator_to_main_ = true;
}

void SettingsIndicatorTab::OnIndicatorCombobox(wxCommandEvent& event) {
  SendIndicatorToMain();
}

void SettingsIndicatorTab::OnIndicatorColor(wxColourPickerEvent& event) {
  SendIndicatorToMain();
}

void SettingsIndicatorTab::OnIndicatorCheckbox(wxCommandEvent& event) {
  SendIndicatorToMain();
}

void SettingsIndicatorTab::OnIndicatorText(wxCommandEvent& event) {
  SendIndicatorToMain();
}

void SettingsIndicatorTab::SendIndicatorToMain() {
  if (allow_send_indicator_to_main_ == false) {
    return;
  }
  IndicatorToGui(false);
  common_->UpdateMain();
}

class IndicatorLink {
 public:
  explicit IndicatorLink(const wxString& name) : name_(name) {}

  const wxString& name() const {
    assert(this);
    return name_;
  }

  virtual void IndicatorToGui(
      bool togui, ride::FontsAndColors* fonts_and_colors,
      ride::Settings& current_settings_, wxComboBox* uiIndicatorStyle,
      wxColourPickerCtrl* uiIndicatorColor, wxCheckBox* uiIndicatorUnder,
      wxTextCtrl* uiIndicatorAlpha, wxTextCtrl* uiIndicatorOutlineAlpha) = 0;

 private:
  wxString name_;
};

std::vector<IndicatorLink*> BuildIndicatorLinks() {
  std::vector<IndicatorLink*> ret;
#define DEF_INDICATOR_LINK(NAME, ID)                                       \
  struct IndicatorLink##ID : public IndicatorLink {                        \
    IndicatorLink##ID() : IndicatorLink(NAME) {}                           \
    void IndicatorToGui(bool togui, ride::FontsAndColors* col,             \
                        ride::Settings& set, wxComboBox* uiIndicatorStyle, \
                        wxColourPickerCtrl* uiIndicatorColor,              \
                        wxCheckBox* uiIndicatorUnder,                      \
                        wxTextCtrl* uiIndicatorAlpha,                      \
                        wxTextCtrl* uiIndicatorOutlineAlpha) {             \
      DIALOG_DATA(set, ID, uiIndicatorStyle, _IS);                         \
      ride::Indicator ind = col->ID();                                     \
                                                                           \
      DIALOG_DATAX(ind, foreground, uiIndicatorColor);                     \
      DIALOG_DATA(ind, under, uiIndicatorUnder, );                         \
      DIALOG_DATA(ind, alpha, uiIndicatorAlpha, _I32);                     \
      DIALOG_DATA(ind, outline_alpha, uiIndicatorOutlineAlpha, _I32);      \
                                                                           \
      if (togui == false) {                                                \
        col->set_allocated_##ID(Allocate(ind));                            \
      }                                                                    \
    }                                                                      \
  };                                                                       \
  static IndicatorLink##ID indicator_link_##ID;                            \
  ret.push_back(&indicator_link_##ID)

  DEF_INDICATOR_LINK("Error", indicator_error);
  DEF_INDICATOR_LINK("Warning", indicator_warning);
  DEF_INDICATOR_LINK("Search highlight", indicator_search_highlight);
  DEF_INDICATOR_LINK("Select highlight", indicator_select_highlight);

#undef DEF_INDICATOR_LINK
  return ret;
}

const std::vector<IndicatorLink*>& GetIndicatorLinks() {
  static std::vector<IndicatorLink*> indicator_links = BuildIndicatorLinks();
  return indicator_links;
}

void SettingsIndicatorTab::IndicatorToGui(bool togui) {
  ride::FontsAndColors fonts_and_colors =
      common_->current_settings().fonts_and_colors();

  int selected_item = uiIndicatorList->GetSelection();
  if (selected_item == -1) return;

  IndicatorLink* link = reinterpret_cast<IndicatorLink*>(
      uiIndicatorList->GetClientData(selected_item));
  assert(link);
  if (link == NULL) return;
  link->IndicatorToGui(togui, &fonts_and_colors,
                       *common_->current_settings_mod(), uiIndicatorStyle,
                       uiIndicatorColor, uiIndicatorUnder, uiIndicatorAlpha,
                       uiIndicatorOutlineAlpha);

  if (togui == false) {
    common_->current_settings_mod()->set_allocated_fonts_and_colors(
        Allocate(fonts_and_colors));
  }
}

SettingsIndicatorTab::SettingsIndicatorTab(wxWindow* parent,
                                           SettingsCommon* common)
    : ui::SettingsIndicatorsPanel(parent),
      common_(common),
      allow_send_indicator_to_main_(false) {
  for (auto link : GetIndicatorLinks()) {
    uiIndicatorList->Append(link->name(), link);
  }
  IndicatorToGui(true);
}
