// Copyright (2015) Gustav

#include "ride/settingsenviromenttab.h"

#include "ride/form.h"
#include "ride/mainwindow.h"
#include "ride/settings.h"
#include "ride/wxutils.h"
#include "ride/pathtester.h"

SettingsEnviromentTab::SettingsEnviromentTab(wxWindow* parent,
                                             SettingsCommon* common)
    : ui::SettingsEnviromentPanel(parent),
      common_(common),
      update_settings_(false) {
  SettingsToGui(true);
}

wxString InfoBool(bool b) {
  if (b)
    return "OK: ";
  else
    return "Invalid. ";
}

void SettingsEnviromentTab::SettingsToGui(bool togui) {
  if (update_settings_) return;
  update_settings_ = true;

  ride::MachineSettings m = common_->current_machine();

  DIALOG_DATA(m, rustc, uiRustcPath, _Str);
  DIALOG_DATA(m, cargo, uiCargoPath, _Str);
  DIALOG_DATA(m, racer, uiRacerPath, _Str);
  DIALOG_DATA(m, protoc, uiProtocPath, _Str);

  if (togui == false) {
    *common_->current_machine_mod() = m;
  }

  if (togui == false) {
    update_settings_ = false;
    return;
  }
  const PathTester p(m);
  uiRustcStatus->SetLabelText(InfoBool(p.RustcIsValid()) + p.rustc());
  uiCargoStatus->SetLabelText(InfoBool(p.CargoIsValid()) + p.cargo());
  uiRacerStatus->SetLabelText(InfoBool(p.RacerIsValid()) + p.racer());
  uiProtocStatus->SetLabelText(InfoBool(p.ProtocIsValid()) + p.protoc());

  update_settings_ = false;
}

void SettingsEnviromentTab::OnCargo(wxCommandEvent& event) {
  wxFileDialog fd(this, "Choose cargo");
  if (wxID_CANCEL == fd.ShowModal()) return;
  common_->current_machine_mod()->set_cargo(fd.GetPath().c_str().AsChar());
  SettingsToGui(true);
}

void SettingsEnviromentTab::OnRustc(wxCommandEvent& event) {
  wxFileDialog fd(this, "Choose rustc");
  if (wxID_CANCEL == fd.ShowModal()) return;
  common_->current_machine_mod()->set_rustc(fd.GetPath().c_str().AsChar());
  SettingsToGui(true);
}

void SettingsEnviromentTab::OnRacer(wxCommandEvent& event) {
  wxFileDialog fd(this, "Choose racer");
  if (wxID_CANCEL == fd.ShowModal()) return;
  common_->current_machine_mod()->set_racer(fd.GetPath().c_str().AsChar());
  SettingsToGui(true);
}

void SettingsEnviromentTab::OnProtoc(wxCommandEvent& event) {
  wxFileDialog fd(this, "Choose protoc");
  if (wxID_CANCEL == fd.ShowModal()) return;
  common_->current_machine_mod()->set_protoc(fd.GetPath().c_str().AsChar());
  SettingsToGui(true);
}

void SettingsEnviromentTab::OnText(wxCommandEvent& event) {
  SettingsToGui(false);
}

void SettingsEnviromentTab::OnRefresh(wxCommandEvent& event) {
  SettingsToGui(true);
}
