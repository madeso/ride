// Copyright (2015) Gustav

#include "ride/settingsmarkerstab.h"

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

void ToGui(ride::MarkerSymbol data, wxComboBox* gui) {
  gui->SetSelection(static_cast<int>(data));
}

ride::MarkerSymbol ToData_MS(wxComboBox* gui) {
  RETURN_COMBOBOX_VALUE(MarkerSymbol, gui->GetSelection());
}

void SettingsMarkersTab::OnMarkerComboChanged(wxCommandEvent& event) {
  SendMarkerToMain();
}

void SettingsMarkersTab::OnMarkerColorChanged(wxColourPickerEvent& event) {
  SendMarkerToMain();
}

void SettingsMarkersTab::OnMarkerComboChanged(wxColourPickerEvent& event) {
  SendMarkerToMain();
}

class MarkerLink {
 public:
  explicit MarkerLink(const wxString& name) : name_(name) {}

  const wxString& name() const {
    assert(this);
    return name_;
  }

  virtual void MarkerToGui(bool togui, ride::FontsAndColors* ref,
                           ride::Settings& set, wxComboBox* sym,
                           wxColourPickerCtrl* fore,
                           wxColourPickerCtrl* back) = 0;

 private:
  wxString name_;
};

std::vector<MarkerLink*> BuildMarkerLinks() {
  std::vector<MarkerLink*> ret;
#define DEF_MARKER_LINK(NAME, ID)                                          \
  struct MarkerLink##ID : public MarkerLink {                              \
    MarkerLink##ID() : MarkerLink(NAME) {}                                 \
    void MarkerToGui(bool togui, ride::FontsAndColors* col,                \
                     ride::Settings& set, wxComboBox* sym,                 \
                     wxColourPickerCtrl* fore, wxColourPickerCtrl* back) { \
      DIALOG_DATA(set, ID, sym, _MS);                                      \
      DIALOG_DATAX((*col), ID##_foreground, fore);                         \
      DIALOG_DATAX((*col), ID##_background, back);                         \
    }                                                                      \
  };                                                                       \
  static MarkerLink##ID marker_link_##ID;                                  \
  ret.push_back(&marker_link_##ID)

  DEF_MARKER_LINK("Folder", folder);
  DEF_MARKER_LINK("Folder open", folderopen);
  DEF_MARKER_LINK("Folder open mid", folderopenmid);

  DEF_MARKER_LINK("Folder end", folderend);
  DEF_MARKER_LINK("Folder mid tail", foldermidtail);
  DEF_MARKER_LINK("Folder tail", foldertail);
  DEF_MARKER_LINK("Folder sub", foldersub);

#undef DEF_MARKER_LINK
  return ret;
}

const std::vector<MarkerLink*>& GetMarkerLinks() {
  static std::vector<MarkerLink*> marker_links = BuildMarkerLinks();
  return marker_links;
}

void SettingsMarkersTab::OnMarkerListChanged(wxCommandEvent& event) {
  allow_send_marker_to_main_ = false;
  MarkerToGui(true);
  allow_send_marker_to_main_ = true;
}

void SettingsMarkersTab::SendMarkerToMain() {
  if (allow_send_marker_to_main_ == false) {
    return;
  }
  MarkerToGui(false);
  common_->UpdateMain();
}

void SettingsMarkersTab::MarkerToGui(bool togui) {
  ride::FontsAndColors fonts_and_colors =
      common_->current_settings().fonts_and_colors();

  int selected_item = uiMarkerList->GetSelection();
  if (selected_item == -1) return;

  MarkerLink* link =
      reinterpret_cast<MarkerLink*>(uiMarkerList->GetClientData(selected_item));
  assert(link);
  if (link == NULL) return;
  link->MarkerToGui(togui, &fonts_and_colors, *common_->current_settings_mod(),
                    uiMarkerSymbol, uiMarkerForegroundColor,
                    uiMarkerBackgroundColor);

  if (togui == false) {
    common_->current_settings_mod()->set_allocated_fonts_and_colors(
        Allocate(fonts_and_colors));
  }
}

SettingsMarkersTab::SettingsMarkersTab(wxWindow* parent, SettingsCommon* common)
    : ui::SettingsMarkersPanel(parent),
      common_(common),
      allow_send_marker_to_main_(false) {
  for (auto link : GetMarkerLinks()) {
    uiMarkerList->Append(link->name(), link);
  }

  MarkerToGui(true);
}
