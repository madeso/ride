#include "ride/settingseditortab.h"

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

void ToGui(ride::ViewWhitespace data, wxComboBox* gui)
{
	gui->SetSelection(static_cast<int>(data));
}

ride::ViewWhitespace ToData_VW(wxComboBox* gui)
{
	RETURN_COMBOBOX_VALUE(ViewWhitespace, gui->GetSelection());
}

void ToGui(ride::WrapMode data, wxComboBox* gui)
{
	gui->SetSelection(static_cast<int>(data));
}

ride::WrapMode ToData_WM(wxComboBox* gui)
{
	RETURN_COMBOBOX_VALUE(WrapMode, gui->GetSelection());
}

void ToGui(ride::EdgeStyle data, wxComboBox* gui)
{
	gui->SetSelection(static_cast<int>(data));
}

ride::EdgeStyle ToData_ES(wxComboBox* gui)
{
	RETURN_COMBOBOX_VALUE(EdgeStyle, gui->GetSelection());
}

void ToGui(ride::AutoIndentation data, wxComboBox* gui)
{
	gui->SetSelection(static_cast<int>(data));
}

ride::AutoIndentation ToData_AI(wxComboBox* gui)
{
	RETURN_COMBOBOX_VALUE(AutoIndentation, gui->GetSelection());
}

void ToGui(ride::AutoComplete data, wxComboBox* gui)
{
	gui->SetSelection(static_cast<int>(data));
}

ride::AutoComplete ToData_AC(wxComboBox* gui)
{
	RETURN_COMBOBOX_VALUE(AutoComplete, gui->GetSelection());
}

//////////////////////////////////////////////////////////////////////////

SettingsEditorTab::SettingsEditorTab(wxWindow* parent, SettingsCommon* common)
	: ui::SettingsEditorPanel(parent)
	, common_(common)
	, allow_send_edit_to_main_(false)
{
	const std::vector<wxString> ac_names = {"None", "Paredit"};
	for (const wxString& ac_name: ac_names)
	{
		uiEditAutocompleteCurlyBraces->Append(ac_name);
		uiEditAutocompleteParentheses->Append(ac_name);
		uiEditAutocompleteBrackets->Append(ac_name);
		uiEditAutocompleteSingleQuote->Append(ac_name);
		uiEditAutocompleteDoubleQuote->Append(ac_name);
	}
	EditToGui(true);
}

void SettingsEditorTab::OnCheckboxChanged(wxCommandEvent& event)
{
	assert(this);
	SendEditToMain();
}

void SettingsEditorTab::OnComboboxChanged(wxCommandEvent& event)
{
	assert(this);
	SendEditToMain();
}

void SettingsEditorTab::OnColorChanged(wxColourPickerEvent& event)
{
	assert(this);
	SendEditToMain();
}

void SettingsEditorTab::OnEditChanged(wxCommandEvent& event)
{
	assert(this);
	SendEditToMain();
}

void SettingsEditorTab::SendEditToMain()
{
	if (allow_send_edit_to_main_ == false)
	{
		return;
	}
	EditToGui(false);
	common_->UpdateMain();
}

//////////////////////////////////////////////////////////////////////////

void SettingsEditorTab::EditToGui(bool togui)
{
	ride::Settings& current_settings_ = *common_->current_settings_mod();
	ride::FontsAndColors fonts_and_colors = current_settings_.fonts_and_colors;
	ride::FoldFlags foldflags = current_settings_.foldflags;

	DIALOG_DATA(current_settings_, displayEOLEnable, uiDisplayEOL, );
	DIALOG_DATA(current_settings_, lineNumberEnable, uiShowLineNumbers, );
	DIALOG_DATA(current_settings_, indentGuideEnable, uiIndentGuide, );

	DIALOG_DATA(current_settings_, tabWidth, uiTabWidth, _I32);
	DIALOG_DATA(current_settings_, edgeColumn, uiEdgeColumn, _I32);

	DIALOG_DATA(current_settings_, whitespace, uiViewWhitespace, _VW);
	DIALOG_DATA(current_settings_, wordWrap, uiWordwrap, _WM);
	DIALOG_DATA(current_settings_, edgeStyle, uiEdgeStyle, _ES);
	DIALOG_DATA(current_settings_, auto_indentation, uiEditIndentation, _AI);

	DIALOG_DATA(current_settings_, tabIndents, uiTabIndents, );
	DIALOG_DATA(current_settings_, useTabs, uiUseTabs, );
	DIALOG_DATA(current_settings_, backspaceUnindents, uiBackspaceUnindents, );

	DIALOG_DATA(current_settings_, foldEnable, uiAllowFolding, );

	DIALOG_DATA(foldflags, LEVELNUMBERS, uiFoldLevelNumbers, );
	DIALOG_DATA(foldflags, LINEBEFORE_EXPANDED, uiFoldLineBeforeExpanded, );
	DIALOG_DATA(foldflags, LINEBEFORE_CONTRACTED, uiFoldLineBeforeContracted, );
	DIALOG_DATA(foldflags, LINEAFTER_EXPANDED, uiFoldLineAfterExpanded, );
	DIALOG_DATA(foldflags, LINEAFTER_CONTRACTED, uiFoldLineAfterContracted, );

	DIALOG_DATAX(fonts_and_colors, edgeColor, uiEdgeColor);

	DIALOG_DATA(fonts_and_colors, use_selection_foreground, uiEditUseSelectionForeground, );
	DIALOG_DATAX(fonts_and_colors, selection_foreground, uiEditSelectionForeground);
	DIALOG_DATA(fonts_and_colors, use_selection_background, uiEditUseSelectionBackground, );
	DIALOG_DATAX(fonts_and_colors, selection_background, uiEditSelectionBackground);

	DIALOG_DATA(current_settings_, current_line_visible, uiCurrentLineVisible, );
	DIALOG_DATA(current_settings_, current_line_alpha, uiEditCurrentLineAlpha, _I32);
	DIALOG_DATA(current_settings_, current_line_overdraw, uiEditCurrentLineOverdraw, );
	DIALOG_DATAX(fonts_and_colors, selected_line, uiEditCurrentLineColor);

	DIALOG_DATAX(fonts_and_colors, fold_margin_hi, uiEditFoldHi);
	DIALOG_DATAX(fonts_and_colors, fold_margin_low, uiEditFoldLow);

	DIALOG_DATA(
		current_settings_, highlight_word_also_highlight_keywords, uiEditHighlightKeyword,
	);

	DIALOG_DATA(current_settings_, autocomplete_curly_braces, uiEditAutocompleteCurlyBraces, _AC);
	DIALOG_DATA(current_settings_, autocomplete_parentheses, uiEditAutocompleteParentheses, _AC);
	DIALOG_DATA(current_settings_, autocomplete_brackets, uiEditAutocompleteBrackets, _AC);
	DIALOG_DATA(current_settings_, autocomplete_singlequote, uiEditAutocompleteSingleQuote, _AC);
	DIALOG_DATA(current_settings_, autocomplete_doublequote, uiEditAutocompleteDoubleQuote, _AC);

	DIALOG_DATA(current_settings_, show_multiline_indicators, uiEditShowMultilineIndicators, );
	DIALOG_DATA(
		current_settings_, show_compiler_messages_as_annotations, uiEditShowCompilerAnnotations,
	);

	if (togui == false)
	{
		current_settings_.fonts_and_colors = fonts_and_colors;
		current_settings_.foldflags = foldflags;
	}
}

void SettingsEditorTab::OnlyAllowNumberChars(wxKeyEvent& event)
{
	::OnlyAllowNumberChars(event);
}
