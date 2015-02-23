#ifndef SETTINGS_DLG_H
#define SETTINGS_DLG_H

#include "ride/generated/ui.h"
#include "ride/settings.h"

class MainWindow;

class SettingsDlg : public ui::Settings
{
public:
  SettingsDlg(wxWindow* parent, MainWindow* mainwindow);

protected:
  void OnApply(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);

  //////////////////////////////////////////////////////////////////////////
  // Style tab
  void OnOnlyFixedSysChanged(wxCommandEvent& event);
  void OnSelectedStyleChanged(wxCommandEvent& event);
  void OnStyleFontChanged(wxCommandEvent& event);
  void OnStyleCheckChanged(wxCommandEvent& event);
  void OnStyleTextChanged(wxCommandEvent& event);
  void OnStyleColorChanged(wxColourPickerEvent& event);

  //////////////////////////////////////////////////////////////////////////
  // Indicator tab
  void OnIndicatorListChanged(wxCommandEvent& event);
  void OnIndicatorCombobox(wxCommandEvent& event);
  void OnIndicatorColor(wxColourPickerEvent& event);
  void OnIndicatorCheckbox(wxCommandEvent& event);
  void OnIndicatorText(wxCommandEvent& event);

  //////////////////////////////////////////////////////////////////////////
  // Marker tab
  void OnMarkerListChanged(wxCommandEvent& event);
  void OnMarkerComboChanged(wxCommandEvent& event);
  void OnMarkerColorChanged(wxColourPickerEvent& event);
  void OnMarkerComboChanged(wxColourPickerEvent& event);

  //////////////////////////////////////////////////////////////////////////
  // Edit tab
  void OnCheckboxChanged(wxCommandEvent& event);
  void OnComboboxChanged(wxCommandEvent& event);
  void OnColorChanged(wxColourPickerEvent& event);
  void OnEditChanged(wxCommandEvent& event);
  void OnlyAllowNumberChars(wxKeyEvent& event);

private:
  MainWindow* main_window_;
  ride::Settings global_settings_;
  ride::Settings current_settings_;

  //////////////////////////////////////////////////////////////////////////
  // Style tab
  bool allow_send_style_to_main_;
  void UpdateStyleEnable();
  void SendStyleToMain();
  void StyleToGui(bool togui);
  void UpdateStyleFonts();
  void StyleUpdateFontDisplay();
  void StyleSaveSelectedIndex();

  //////////////////////////////////////////////////////////////////////////
  // Indicator tab
  bool allow_send_indicator_to_main_;
  void SendIndicatorToMain();
  void IndicatorToGui(bool togui);

  //////////////////////////////////////////////////////////////////////////
  // Marker tab
  bool allow_send_marker_to_main_;
  void SendMarkerToMain();
  void MarkerToGui(bool togui);

  //////////////////////////////////////////////////////////////////////////
  // Edit tab
  bool allow_send_edit_to_main_;
  void SendEditToMain();
  void EditToGui(bool togui);
};

#endif // SETTINGS_DLG_H
