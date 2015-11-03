// Copyright (2015) Gustav

#include "ride/settingsenviromenttab.h"

#include "ride/form.h"
#include "ride/mainwindow.h"
#include "ride/settings.h"
#include "ride/wxutils.h"
#include "ride/pathtester.h"

SettingsEnviromentTab::SettingsEnviromentTab(wxWindow* parent,
                                             SettingsCommon* common)
    : ui::SettingsEnviromentPanel(parent), common_(common) {
  SettingsToGui(true);
}

void SettingsEnviromentTab::SettingsToGui(bool togui) {
  ride::MachineSettings m = common_->current_machine();

  DIALOG_DATA(m, rustc, uiRustcPath, _Str);
  DIALOG_DATA(m, cargo, uiCargoPath, _Str);
  DIALOG_DATA(m, racer, uiRacerPath, _Str);
  DIALOG_DATA(m, protoc, uiProtocPath, _Str);

  if (togui == false) {
    return;
  }
  const PathTester p(m);
  uiRustcStatus->SetLabelText(p.rustc());
  uiCargoStatus->SetLabelText(p.cargo());
  uiRacerStatus->SetLabelText(p.racer());
  uiProtocStatus->SetLabelText(p.protoc());
}

void SettingsEnviromentTab::OnCargo(wxCommandEvent& event) { event.Skip(); }
void SettingsEnviromentTab::OnRustc(wxCommandEvent& event) { event.Skip(); }
void SettingsEnviromentTab::OnRacer(wxCommandEvent& event) { event.Skip(); }
void SettingsEnviromentTab::OnProtoc(wxCommandEvent& event) { event.Skip(); }
void SettingsEnviromentTab::OnRefresh(wxCommandEvent& event) {
  SettingsToGui(true);
}
