// Copyright (2015) Gustav

#ifndef RIDE_SETTINGSEDITORTAB_H_
#define RIDE_SETTINGSEDITORTAB_H_

#include <ride/wx.h>
#include "ride/generated/ui.h"
#include "ride/settingscommon.h"

class MainWindow;

class SettingsEditorTab : public ui::SettingsEditorPanel
{
public:

	SettingsEditorTab(wxWindow* parent, SettingsCommon* common);

	void EditToGui(bool togui);

protected:

	void OnCheckboxChanged(wxCommandEvent& event);
	void OnComboboxChanged(wxCommandEvent& event);
	void OnColorChanged(wxColourPickerEvent& event);
	void OnEditChanged(wxCommandEvent& event);
	void OnlyAllowNumberChars(wxKeyEvent& event);

private:

	SettingsCommon* common_;

	bool allow_send_edit_to_main_;
	void SendEditToMain();
};

#endif	// RIDE_SETTINGSEDITORTAB_H_
