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

SettingsThemesTab::SettingsThemesTab(wxWindow* parent, SettingsCommon* common, ToGuiSender* togui)
	: ui::SettingsThemesPanel(parent)
	, main_(togui)
	, common_(common)
	, theme_list_(uiThemeList, this)
	, allow_to_gui_(true)
{
	theme_list_.Setup(
		uiThemeListAdd, uiThemeListRemove, uiThemeListChange, uiThemeListUp, uiThemeListDown
	);
	ThemeToGui(true);
}

void SettingsThemesTab::ThemeToGui(bool togui)
{
	if (! allow_to_gui_) return;
	theme_list_.ToGui(common_->current_settings_mod(), togui);
}

void SettingsThemesTab::OnThemeApplySelected(wxCommandEvent& event)
{
	int selected = uiThemeList->GetSelection();
	if (selected == -1)
	{
		return;
	}
	common_->current_settings_mod()->fonts_and_colors
		= common_->current_settings().themes[selected].data;

	allow_to_gui_ = false;
	main_->SendToGui(true);
	allow_to_gui_ = true;

	common_->UpdateMain();
}

void SettingsThemesTab::OnThemeExportSelected(wxCommandEvent& event)
{
	int selected = uiThemeList->GetSelection();
	if (selected == -1)
	{
		return;
	}

	wxFileDialog saveFileDialog(
		this,
		_("Save theme"),
		"",
		"",
		"Ride themes (*.ridetheme)|*.ridetheme",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

	const auto saved
		= SaveProtoJson(common_->current_settings().themes[selected], saveFileDialog.GetPath());
	if (saved != "")
	{
		ShowError(this, "Failed to save theme: " + saved, "Failed to save");
	}
}

void SettingsThemesTab::OnThemeImport(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(
		this,
		_("Open theme"),
		"",
		"",
		"Ride themes (*.ridetheme)|*.ridetheme",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

	ride::Theme theme;
	const auto saved = LoadProtoJson(&theme, saveFileDialog.GetPath());
	if (saved != "")
	{
		ShowError(this, "Failed to import theme:" + saved, "Failed to import");
		return;
	}

	common_->current_settings_mod()->themes.push_back(theme);
	ThemeToGui(true);
}

void SettingsThemesTab::OnAdd(wxCommandEvent& event)
{
	if (false == theme_list_.Add(common_->current_settings_mod()))
	{
		return;
	}
	ThemeToGui(true);
}

void SettingsThemesTab::OnEdit(wxCommandEvent& event)
{
	if (false == theme_list_.Edit(common_->current_settings_mod()))
	{
		return;
	}
	ThemeToGui(true);
}

void SettingsThemesTab::OnRemove(wxCommandEvent& event)
{
	if (false == theme_list_.Remove(common_->current_settings_mod()))
	{
		return;
	}
	ThemeToGui(true);
}

void SettingsThemesTab::OnUp(wxCommandEvent& event)
{
	if (false == theme_list_.Up(common_->current_settings_mod()))
	{
		return;
	}
	ThemeToGui(true);
}

void SettingsThemesTab::OnDown(wxCommandEvent& event)
{
	if (false == theme_list_.Down(common_->current_settings_mod()))
	{
		return;
	}
	ThemeToGui(true);
}
