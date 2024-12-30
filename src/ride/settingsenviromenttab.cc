#include "ride/settingsenviromenttab.h"

#include "ride/form.h"
#include "ride/mainwindow.h"
#include "ride/settings.h"
#include "ride/wxutils.h"
#include "ride/pathtester.h"

SettingsEnviromentTab::SettingsEnviromentTab(wxWindow* parent, SettingsCommon* common)
	: ui::SettingsEnviromentPanel(parent)
	, common_(common)
	, update_settings_(false)
{
	SettingsToGui(true);
}

wxString InfoBool(bool b)
{
	if (b)
		return "OK: ";
	else
		return "Invalid. ";
}

wxString GetEnv(const wxString& var, const wxString& def)
{
	wxString val;
	if (wxGetEnv(var, &val))
		return val;
	else
		return def;
}

void SettingsEnviromentTab::SettingsToGui(bool togui)
{
	if (update_settings_) return;
	update_settings_ = true;

	ride::MachineSettings m = common_->current_machine();

	DIALOG_DATA(m, rustc, uiRustcPath, _Str);
	DIALOG_DATA(m, cargo, uiCargoPath, _Str);
	DIALOG_DATA(m, racer, uiRacerPath, _Str);
	DIALOG_DATA(m, protoc, uiProtocPath, _Str);
	DIALOG_DATA(m, rust_src_path, uiRustSrc, _Str);

	if (togui == false)
	{
		*common_->current_machine_mod() = m;
	}

	if (togui == false)
	{
		update_settings_ = false;
		return;
	}
	const PathTester p(m);
	uiRustcStatus->SetLabelText(InfoBool(p.RustcIsValid()) + p.rustc());
	uiCargoStatus->SetLabelText(InfoBool(p.CargoIsValid()) + p.cargo());
	uiRacerStatus->SetLabelText(InfoBool(p.RacerIsValid()) + p.racer());
	uiProtocStatus->SetLabelText(InfoBool(p.ProtocIsValid()) + p.protoc());

	const wxString path = m.rust_src_path != "" ? wxString(m.rust_src_path.c_str())
												: GetEnv("RUST_SRC_PATH", "No path available");
	uiRustSrcStatus->SetLabelText(path);

	update_settings_ = false;
}

void SettingsEnviromentTab::OnCargo(wxCommandEvent& event)
{
	wxFileDialog fd(this, "Choose cargo");
	if (wxID_CANCEL == fd.ShowModal()) return;
	common_->current_machine_mod()->cargo = fd.GetPath().c_str().AsChar();
	SettingsToGui(true);
}

void SettingsEnviromentTab::OnRustc(wxCommandEvent& event)
{
	wxFileDialog fd(this, "Choose rustc");
	if (wxID_CANCEL == fd.ShowModal()) return;
	common_->current_machine_mod()->rustc = fd.GetPath().c_str().AsChar();
	SettingsToGui(true);
}

void SettingsEnviromentTab::OnRacer(wxCommandEvent& event)
{
	wxFileDialog fd(this, "Choose racer");
	if (wxID_CANCEL == fd.ShowModal()) return;
	common_->current_machine_mod()->racer = fd.GetPath().c_str().AsChar();
	SettingsToGui(true);
}

void SettingsEnviromentTab::OnProtoc(wxCommandEvent& event)
{
	wxFileDialog fd(this, "Choose protoc");
	if (wxID_CANCEL == fd.ShowModal()) return;
	common_->current_machine_mod()->protoc = fd.GetPath().c_str().AsChar();
	SettingsToGui(true);
}

void SettingsEnviromentTab::OnRustSrc(wxCommandEvent& event)
{
	wxDirDialog fd(this, "Choose RUST_SRC_PATH", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (wxID_CANCEL == fd.ShowModal()) return;
	common_->current_machine_mod()->rust_src_path = fd.GetPath().c_str().AsChar();
	SettingsToGui(true);
}

void SettingsEnviromentTab::OnText(wxCommandEvent& event)
{
	SettingsToGui(false);
}

void SettingsEnviromentTab::OnRefresh(wxCommandEvent& event)
{
	SettingsToGui(true);
}
