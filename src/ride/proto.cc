#include "ride/proto.h"

#include <fstream>
#include "ride/settings.proto.h"

#include "nlohmann/json.hpp"

// ------------------------------------------------------------------------------------------------
// filer
struct Filer
{
	bool is_loading;
	nlohmann::json json;

	std::string require_string_prop(const std::string& name)
	{
		return "";
	}
};

// ------------------------------------------------------------------------------------------------
// filing

template<typename T>
void tser(Filer* filer, std::optional<T>* val)
{
	T t;
	ser(filer, &t);
	*val = t;
}

void ser(Filer* filer, bool* value)
{
	if(filer->is_loading)
	{
		*value = filer->json;
	}
	else
	{
		filer->json = *value;
	}
}

void ser(Filer* filer, int* value)
{
	if(filer->is_loading)
	{
		*value = filer->json;
	}
	else
	{
		filer->json = *value;
	}
}

void ser(Filer* filer, std::string* value)
{
	if(filer->is_loading)
	{
		*value = filer->json;
	}
	else
	{
		filer->json = *value;
	}
}


#define F_ENUM(ENUM) void ser(Filer* filer, ride::ENUM* value)
#define S_ENUM_DECLARE(ENUM) const auto prop = filer->is_loading ? filer->require_string_prop(#ENUM) : ""
#define S_ENUM_VAL(x, n) do { if(filer->is_loading) { if(prop == n) \
	{*value = ride::x; return; } } else \
	{ if(*value == ride::x) { filer->json = n; } } } while(false)

#define F_STRUCT(STRUCT) void ser(Filer* filer, ride::STRUCT* value)
#define S_PROP(PROP, NAME) do { if(filer->is_loading) \
	{auto prop = filer->json.find(NAME); if(prop != filer->json.end()) { Filer ff{true, *prop}; ser(&ff, &value->PROP);} } else \
	{Filer ff{false, {}}; ser(&ff, &value->PROP); if(ff.json.is_null() == false) { filer->json[NAME] = ff.json;} } } while(false)
#define S_PROP_O(PROP, NAME) do { if(filer->is_loading) \
	{auto prop = filer->json.find(NAME); if(prop != filer->json.end()) { Filer ff{true, *prop}; tser(&ff, &value->PROP);} } else \
	{ if(value->PROP.has_value()) { Filer ff{false, {}}; ser(&ff, &(*value->PROP)); if(ff.json.is_null() == false) { filer->json[NAME] = ff.json;} } } } while(false)
#define S_PROP_V(PROP, NAME) do {\
	if(filer->is_loading) \
	{\
		auto prop = filer->json.find(NAME);\
		value->PROP = {};\
		if(prop != filer->json.end()) for(auto it: *prop) {\
			Filer ff{true, it};\
			decltype(value->PROP)::value_type v;\
			ser(&ff, &v);\
			value->PROP.emplace_back(v);\
		}\
	} else \
	{\
		nlohmann::json o = nlohmann::json::array();\
		for(auto& s: value->PROP) {\
			Filer ff{false, {}};\
			ser(&ff, &s);\
			if(ff.json.is_null() == false) {\
				o.push_back(ff.json);\
			}\
		}\
		filer->json[NAME] = o;\
	} } while(false)

F_ENUM(EdgeStyle)
{
	S_ENUM_DECLARE(EdgeStyle);
	S_ENUM_VAL(EDGESTYLE_NONE, "none");
	S_ENUM_VAL(EDGESTYLE_LINE, "line");
	S_ENUM_VAL(EDGESTYLE_BACKGROUND, "background");
}

F_ENUM(ViewWhitespace)
{
	S_ENUM_DECLARE(ViewWhitespace);
	S_ENUM_VAL(VIEWWHITESPACE_HIDDEN, "hidden");
	S_ENUM_VAL(VIEWWHITESPACE_ALWAYS, "always");
	S_ENUM_VAL(VIEWWHITESPACE_AFTER_IDENT, "after_ident");
}

F_ENUM(WrapMode)
{
	S_ENUM_DECLARE(WrapMode);
	S_ENUM_VAL(WRAPMODE_NONE, "none");
	S_ENUM_VAL(WRAPMODE_CHAR, "char");
	S_ENUM_VAL(WRAPMODE_WORD, "word");
}

F_ENUM(AutoIndentation)
{
	S_ENUM_DECLARE(AutoIndentation);
	S_ENUM_VAL(AUTOINDENTATION_NONE, "none");
	S_ENUM_VAL(AUTOINDENTATION_KEEP, "keep");
	S_ENUM_VAL(AUTOINDENTATION_SMART, "mart");
}

F_ENUM(IndicatorStyle)
{
	S_ENUM_DECLARE(IndicatorStyle);
	S_ENUM_VAL(INDICATORSTYLE_PLAIN, "plain");
	S_ENUM_VAL(INDICATORSTYLE_SQUIGGLE, "squiggle");
	S_ENUM_VAL(INDICATORSTYLE_TT, "tt");
	S_ENUM_VAL(INDICATORSTYLE_DIAGONAL, "diagonal");
	S_ENUM_VAL(INDICATORSTYLE_STRIKE, "strike");
	S_ENUM_VAL(INDICATORSTYLE_HIDDEN, "hidden");
	S_ENUM_VAL(INDICATORSTYLE_BOX, "box");
	S_ENUM_VAL(INDICATORSTYLE_ROUNDBOX, "roundbox");
	S_ENUM_VAL(INDICATORSTYLE_STRAIGHTBOX, "straightbox");
	S_ENUM_VAL(INDICATORSTYLE_DASH, "dash");
	S_ENUM_VAL(INDICATORSTYLE_DOTS, "dots");
	S_ENUM_VAL(INDICATORSTYLE_SQUIGGLELOW, "squigglelow");
	S_ENUM_VAL(INDICATORSTYLE_DOTBOX, "dotbox");
};

F_ENUM(VirtualSpace)
{
	S_ENUM_DECLARE(VirtualSpace);
	S_ENUM_VAL(VIRTUALSPACE_NONE, "none");
	S_ENUM_VAL(VIRTUALSPACE_RECTANGULARSELECTION, "rectangularselection");
	S_ENUM_VAL(VIRTUALSPACE_USERACCESSIBLE, "useraccessible");
};

F_ENUM(WrapVisualFlagsLocation)
{
	S_ENUM_DECLARE(WrapVisualFlagsLocation);
	S_ENUM_VAL(WRAPVISUALFLAGLOC_DEFAULT, "default");
	S_ENUM_VAL(WRAPVISUALFLAGLOC_END_BY_TEXT, "end_by_text");
	S_ENUM_VAL(WRAPVISUALFLAGLOC_START_BY_TEXT, "start_by_text");
};

F_ENUM(WrapIndentMode)
{
	S_ENUM_DECLARE(WrapIndentMode);
	S_ENUM_VAL(WRAPINDENT_FIXED, "fixed");
	S_ENUM_VAL(WRAPINDENT_SAME, "same");
	S_ENUM_VAL(WRAPINDENT_INDENT, "indent");
};

F_ENUM(Annotation)
{
	S_ENUM_DECLARE(Annotation);
	S_ENUM_VAL(ANNOTATION_HIDDEN, "hidden");
	S_ENUM_VAL(ANNOTATION_STANDARD, "standard");
	S_ENUM_VAL(ANNOTATION_BOXED, "boxed");
};

F_ENUM(MarkerSymbol)
{
	S_ENUM_DECLARE(MarkerSymbol);
	S_ENUM_VAL(MARKERSYMBOL_CIRCLE, "circle");
	S_ENUM_VAL(MARKERSYMBOL_ROUNDRECT, "roundrect");
	S_ENUM_VAL(MARKERSYMBOL_ARROW, "arrow");
	S_ENUM_VAL(MARKERSYMBOL_SMALLRECT, "smallrect");
	S_ENUM_VAL(MARKERSYMBOL_SHORTARROW, "shortarrow");
	S_ENUM_VAL(MARKERSYMBOL_EMPTY, "empty");
	S_ENUM_VAL(MARKERSYMBOL_ARROWDOWN, "arrowdown");
	S_ENUM_VAL(MARKERSYMBOL_MINUS, "minus");
	S_ENUM_VAL(MARKERSYMBOL_PLUS, "plus");
};

F_ENUM(CaretStyle)
{
	S_ENUM_DECLARE(CaretStyle);
	S_ENUM_VAL(CARETSTYLE_INVISIBLE, "invisible");
	S_ENUM_VAL(CARETSTYLE_LINE, "line");
	S_ENUM_VAL(CARETSTYLE_BLOCK, "block");
};

F_ENUM(CaretSticky)
{
	S_ENUM_DECLARE(CaretSticky);
	S_ENUM_VAL(CARETSTICKY_OFF, "off");
	S_ENUM_VAL(CARETSTICKY_WHITESPACE, "whitespace");
	S_ENUM_VAL(CARETSTICKY_ON, "on");
};

F_ENUM(StatusbarStyle)
{
	S_ENUM_DECLARE(StatusbarStyle);
	S_ENUM_VAL(STATUSBAR_STYLE_NONE, "none");
	S_ENUM_VAL(STATUSBAR_STYLE_BAR, "bar");
	S_ENUM_VAL(STATUSBAR_STYLE_RAISED, "raised");
	S_ENUM_VAL(STATUSBAR_STYLE_SUNKEN, "sunken");
};

F_ENUM(AutoComplete)
{
	S_ENUM_DECLARE(AutoComplete);
	S_ENUM_VAL(AUTOCOMPLETE_NONE, "none");
	S_ENUM_VAL(AUTOCOMPLETE_PARA, "para");
};

F_ENUM(HomeEndStyle)
{
	S_ENUM_DECLARE(HomeEndStyle);
	S_ENUM_VAL(HES_BASIC, "basic");
	S_ENUM_VAL(HES_DISPLAY, "display");
	S_ENUM_VAL(HES_WRAP, "wrap");
	S_ENUM_VAL(HES_VCBASIC, "vcbasic");
	S_ENUM_VAL(HES_VCDISPLAY, "vcdisplay");
	S_ENUM_VAL(HES_VCWRAP, "vcwrap");
	S_ENUM_VAL(HES_SCROLL, "scroll");
};

F_ENUM(WindowState)
{
	S_ENUM_DECLARE(WindowState);
	S_ENUM_VAL(WINDOWSTATE_NORMAL, "normal");
	S_ENUM_VAL(WINDOWSTATE_ICONIZED, "iconized");
	S_ENUM_VAL(WINDOWSTATE_MAXIMIZED, "maximized");
	S_ENUM_VAL(WINDOWSTATE_FULLSCREEN, "fullscreen");
};

F_ENUM(FindDlgTarget)
{
	S_ENUM_DECLARE(FindDlgTarget);
	S_ENUM_VAL(FDT_NORMAL_TEXT, "text");
	S_ENUM_VAL(FDT_NORMAL_REGEX, "regex");
	S_ENUM_VAL(FDT_NORMAL_POSIX, "posix");
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// forward declare all structs

F_STRUCT(Color);
F_STRUCT(Indicator);
F_STRUCT(FoldFlags);
F_STRUCT(Style);
F_STRUCT(FontsAndColors);
F_STRUCT(Theme);
F_STRUCT(WrapVisualFlags);
F_STRUCT(Settings);
F_STRUCT(OpenFile);
F_STRUCT(Session);
F_STRUCT(FindDlg);
F_STRUCT(DialogData);
F_STRUCT(BuildSetting);
F_STRUCT(Project);
F_STRUCT(RunSetting);
F_STRUCT(UserProject);
F_STRUCT(MachineSettings);

// ------------------------------------------------------------------------------------------------
// facade



F_STRUCT(Color)
{
	S_PROP(r, "r");
	S_PROP(g, "g");
	S_PROP(b, "b");
};

F_STRUCT(Indicator)
{
	S_PROP(foreground, "foreground");
	S_PROP(under, "under");
	S_PROP(alpha, "alpha");
	S_PROP(outline_alpha, "outline_alpha");
};

F_STRUCT(FoldFlags)
{
	S_PROP(LINEBEFORE_EXPANDED, "linebefore_expanded");
	S_PROP(LINEBEFORE_CONTRACTED, "linebefore_contracted");
	S_PROP(LINEAFTER_EXPANDED, "lineafter_expanded");
	S_PROP(LINEAFTER_CONTRACTED, "lineafter_contracted");
	S_PROP(LEVELNUMBERS, "levelnumbers");
};

F_STRUCT(Style)
{
	S_PROP(use_typeface, "use_typeface");
	S_PROP(typeface, "typeface");
	S_PROP(use_bold, "use_bold");
	S_PROP(bold, "bold");
	S_PROP(use_italic, "use_italic");
	S_PROP(italic, "italic");
	S_PROP(use_underline, "use_underline");
	S_PROP(underline, "underline");
	S_PROP(use_font_size, "use_font_size");
	S_PROP(font_size, "font_size");
	S_PROP(use_foreground, "use_foreground");
	S_PROP(foreground, "foreground");
	S_PROP(use_background, "use_background");
	S_PROP(background, "background");
};

// the idea of moving out all fonts and colors is that likely theese are
// what people want share: obsidian/zenburn/monokai and https://studiostyl.es/
F_STRUCT(FontsAndColors)
{
	S_PROP(selected_line, "selected_line");
	S_PROP(fold_margin_hi, "fold_margin_hi");
	S_PROP(fold_margin_low, "fold_margin_low");
	S_PROP(use_selection_foreground, "use_selection_foreground");
	S_PROP(selection_foreground, "selection_foreground");
	S_PROP(use_selection_background, "use_selection_background");
	S_PROP(selection_background, "selection_background");
	S_PROP(edgeColor, "edgeColor");
	S_PROP(indicator_error, "indicator_error");
	S_PROP(indicator_warning, "indicator_warning");
	S_PROP(indicator_search_highlight, "indicator_search_highlight");
	S_PROP(indicator_select_highlight, "indicator_select_highlight");
	S_PROP(default_style, "default_style");
	
	S_PROP_O(line_number_style, "line_number_style");
	S_PROP_O(bracelight_style, "bracelight_style");
	S_PROP_O(bracebad_style, "bracebad_style");
	S_PROP_O(controlchar_style, "controlchar_style");
	S_PROP_O(indentguide_style, "indentguide_style");
	S_PROP_O(calltip_style, "calltip_style");
	S_PROP_O(annotation_warning_style, "annotation_warning_style");
	S_PROP_O(annotation_error_style, "annotation_error_style");

	S_PROP(folderend_foreground, "folderend_foreground");
	S_PROP(folderopenmid_foreground, "folderopenmid_foreground");
	S_PROP(foldermidtail_foreground, "foldermidtail_foreground");
	S_PROP(foldertail_foreground, "foldertail_foreground");
	S_PROP(foldersub_foreground, "foldersub_foreground");
	S_PROP(folder_foreground, "folder_foreground");
	S_PROP(folderopen_foreground, "folderopen_foreground");
	S_PROP(folderend_background, "folderend_background");
	S_PROP(folderopenmid_background, "folderopenmid_background");
	S_PROP(foldermidtail_background, "foldermidtail_background");
	S_PROP(foldertail_background, "foldertail_background");
	S_PROP(foldersub_background, "foldersub_background");
	S_PROP(folder_background, "folder_background");
	S_PROP(folderopen_background, "folderopen_background");
	S_PROP(caret_foreground, "caret_foreground");

	S_PROP_O(style_comment, "style_comment");
	S_PROP_O(style_commentline, "style_commentline");
	S_PROP_O(style_commentdoc, "style_commentdoc");
	S_PROP_O(style_number, "style_number");
	S_PROP_O(style_keyword, "style_keyword");
	S_PROP_O(style_string, "style_string");
	S_PROP_O(style_character, "style_character");
	S_PROP_O(style_uuid, "style_uuid");
	S_PROP_O(style_preprocessor, "style_preprocessor");
	S_PROP_O(style_operator, "style_operator");
	S_PROP_O(style_identifier, "style_identifier");
	S_PROP_O(style_string_eol, "style_string_eol");
	S_PROP_O(style_verbatim, "style_verbatim");
	S_PROP_O(style_regex, "style_regex");
	S_PROP_O(style_commentlinedoc, "style_commentlinedoc");
	S_PROP_O(style_keyword_types, "style_keyword_types");
	S_PROP_O(style_commentdockeyword, "style_commentdockeyword");
	S_PROP_O(style_commentdockeyworderror, "style_commentdockeyworderror");
	S_PROP_O(style_globalclass, "style_globalclass");
	S_PROP_O(style_stringraw, "style_stringraw");
	S_PROP_O(style_tripleverbatim, "style_tripleverbatim");
	S_PROP_O(style_hashquotedstring, "style_hashquotedstring");
	S_PROP_O(style_preprocessorcomment, "style_preprocessorcomment");
	S_PROP_O(markdown_line_begin, "markdown_line_begin");
	S_PROP_O(markdown_strong1, "markdown_strong1");
	S_PROP_O(markdown_strong2, "markdown_strong2");
	S_PROP_O(markdown_em1, "markdown_em1");
	S_PROP_O(markdown_em2, "markdown_em2");
	S_PROP_O(markdown_header1, "markdown_header1");
	S_PROP_O(markdown_header2, "markdown_header2");
	S_PROP_O(markdown_header3, "markdown_header3");
	S_PROP_O(markdown_header4, "markdown_header4");
	S_PROP_O(markdown_header5, "markdown_header5");
	S_PROP_O(markdown_header6, "markdown_header6");
	S_PROP_O(markdown_prechar, "markdown_prechar");
	S_PROP_O(markdown_ulist_item, "markdown_ulist_item");
	S_PROP_O(markdown_olist_item, "markdown_olist_item");
	S_PROP_O(markdown_blockquote, "markdown_blockquote");
	S_PROP_O(markdown_strikeout, "markdown_strikeout");
	S_PROP_O(markdown_hrule, "markdown_hrule");
	S_PROP_O(markdown_link, "markdown_link");
	S_PROP_O(markdown_code, "markdown_code");
	S_PROP_O(markdown_code2, "markdown_code2");
	S_PROP_O(markdown_codebk, "markdown_codebk");
	S_PROP_O(props_section, "props_section");
	S_PROP_O(props_assignment, "props_assignment");
	S_PROP_O(props_defval, "props_defval");
	S_PROP_O(props_key, "props_key");
	S_PROP_O(h_tag, "h_tag");
	S_PROP_O(h_tagunknown, "h_tagunknown");
	S_PROP_O(h_attribute, "h_attribute");
	S_PROP_O(h_attributeunknown, "h_attributeunknown");
	S_PROP_O(h_number, "h_number");
	S_PROP_O(h_doublestring, "h_doublestring");
	S_PROP_O(h_singlestring, "h_singlestring");
	S_PROP_O(h_other, "h_other");
	S_PROP_O(h_entity, "h_entity");
	S_PROP_O(h_tagend, "h_tagend");
	S_PROP_O(h_xmlstart, "h_xmlstart");
	S_PROP_O(h_xmlend, "h_xmlend");
	S_PROP_O(h_script, "h_script");
	S_PROP_O(h_asp, "h_asp");
	S_PROP_O(h_aspat, "h_aspat");
	S_PROP_O(h_cdata, "h_cdata");
	S_PROP_O(h_question, "h_question");
	S_PROP_O(h_value, "h_value");
	S_PROP_O(h_xccomment, "h_xccomment");
	S_PROP_O(cmake_stringdq, "cmake_stringdq");
	S_PROP_O(cmake_stringlq, "cmake_stringlq");
	S_PROP_O(cmake_stringrq, "cmake_stringrq");
	S_PROP_O(cmake_commands, "cmake_commands");
	S_PROP_O(cmake_parameters, "cmake_parameters");
	S_PROP_O(cmake_variable, "cmake_variable");
	S_PROP_O(cmake_userdefined, "cmake_userdefined");
	S_PROP_O(cmake_whiledef, "cmake_whiledef");
	S_PROP_O(cmake_foreachdef, "cmake_foreachdef");
	S_PROP_O(cmake_ifdefinedef, "cmake_ifdefinedef");
	S_PROP_O(cmake_macrodef, "cmake_macrodef");
	S_PROP_O(cmake_stringvar, "cmake_stringvar");
	S_PROP_O(cmake_number, "cmake_number");
	S_PROP_O(lua_label, "lua_label");
	S_PROP_O(yaml_number, "yaml_number");
	S_PROP_O(yaml_reference, "yaml_reference");
	S_PROP_O(yaml_document, "yaml_document");
	S_PROP_O(yaml_text, "yaml_text");
	S_PROP_O(yaml_error, "yaml_error");
	S_PROP_O(rust_lifetime, "rust_lifetime");
	S_PROP_O(rust_lex_error, "rust_lex_error");

	S_PROP(dock_background, "dock_background");
	S_PROP(dock_sash, "dock_sash");
	S_PROP(dock_active_caption, "dock_active_caption");
	S_PROP(dock_active_caption_gradient, "dock_active_caption_gradient");
	S_PROP(dock_inactive_caption, "dock_inactive_caption");
	S_PROP(dock_inactive_caption_gradient, "dock_inactive_caption_gradient");
	S_PROP(dock_active_caption_text, "dock_active_caption_text");
	S_PROP(dock_inactive_caption_text, "dock_inactive_caption_text");
	S_PROP(dock_border, "dock_border");
	S_PROP(dock_gripper, "dock_gripper");
	S_PROP(tab_background, "tab_background");
	S_PROP(tab_border, "tab_border");
	S_PROP(tab_sash, "tab_sash");
	S_PROP(tab_active_tab, "tab_active_tab");
	S_PROP(tab_inactive_tab, "tab_inactive_tab");
	S_PROP(tab_active_border, "tab_active_border");
	S_PROP(tab_inactive_border, "tab_inactive_border");
	S_PROP(tab_active_text, "tab_active_text");
	S_PROP(tab_inactive_text, "tab_inactive_text");
	S_PROP(statusbar_style, "statusbar_style");
	S_PROP(statusbar_shadow, "statusbar_shadow");
	S_PROP(statusbar_highlight, "statusbar_highlight");
	S_PROP(statusbar_foreground, "statusbar_foreground");
	S_PROP(statusbar_background, "statusbar_background");
	S_PROP(switcher_background_color, "switcher_background_color");
	S_PROP(switcher_text_color, "switcher_text_color");
	S_PROP(switcher_selection_color, "switcher_selection_color");
	S_PROP(switcher_selection_outline_color, "switcher_selection_outline_color");
	S_PROP(switcher_dialog_color, "switcher_dialog_color");
	S_PROP(switcher_base_color, "switcher_base_color");
};

F_STRUCT(Theme)
{
	S_PROP(name, "name");
	S_PROP(can_remove, "can_remove");
	S_PROP(data, "data");
};

F_STRUCT(WrapVisualFlags)
{
	S_PROP(end, "end");
	S_PROP(start, "start");
	S_PROP(margin, "margin");
};

F_STRUCT(Settings)
{
	S_PROP(lineNumberEnable, "lineNumberEnable");
	S_PROP(foldEnable, "foldEnable");
	S_PROP(displayEOLEnable, "displayEOLEnable");
	S_PROP(indentGuideEnable, "indentGuideEnable");
	S_PROP(whitespace, "whitespace");
	S_PROP(wordWrap, "wordWrap");
	S_PROP(edgeStyle, "edgeStyle");
	S_PROP(has_fonts_and_colors, "has_fonts_and_colors");
	S_PROP(fonts_and_colors, "fonts_and_colors");
	S_PROP(edgeColumn, "edgeColumn");
	S_PROP(tabWidth, "tabWidth");
	S_PROP(useTabs, "useTabs");
	S_PROP(tabIndents, "tabIndents");
	S_PROP(backspaceUnindents, "backspaceUnindents");
	S_PROP(foldflags, "foldflags");
	S_PROP(virtual_space, "virtual_space");
	S_PROP(end_at_last_line, "end_at_last_line");
	S_PROP(vertical_scrollbar, "vertical_scrollbar");
	S_PROP(horizontal_scrollbar, "horizontal_scrollbar");
	S_PROP(wrap_visual_flags, "wrap_visual_flags");
	S_PROP(wrap_visual_flags_location, "wrap_visual_flags_location");
	S_PROP(wrap_indent_mode, "wrap_indent_mode");
	S_PROP(wrap_start_indent, "wrap_start_indent");
	S_PROP(annotations, "annotations");
	S_PROP(current_line_visible, "current_line_visible");
	S_PROP(current_line_alpha, "current_line_alpha");
	S_PROP(current_line_overdraw, "current_line_overdraw");
	S_PROP(highlight_word_also_highlight_keywords, "highlight_word_also_highlight_keywords");
	S_PROP(auto_indentation, "auto_indentation");
	S_PROP(folderend, "folderend");
	S_PROP(folderopenmid, "folderopenmid");
	S_PROP(foldermidtail, "foldermidtail");
	S_PROP(foldertail, "foldertail");
	S_PROP(foldersub, "foldersub");
	S_PROP(folder, "folder");
	S_PROP(folderopen, "folderopen");
	S_PROP(autocomplete_curly_braces, "autocomplete_curly_braces");
	S_PROP(autocomplete_parentheses, "autocomplete_parentheses");
	S_PROP(autocomplete_brackets, "autocomplete_brackets");
	S_PROP(autocomplete_doublequote, "autocomplete_doublequote");
	S_PROP(autocomplete_singlequote, "autocomplete_singlequote");
	S_PROP(show_multiline_indicators, "show_multiline_indicators");
	S_PROP(show_compiler_messages_as_annotations, "show_compiler_messages_as_annotations");
	S_PROP_V(themes, "themes");
	S_PROP(caret_period, "caret_period");
	S_PROP(caret_width, "caret_width");
	S_PROP(caret_sticky, "caret_sticky");
	S_PROP(caret_style, "caret_style");
	S_PROP(home_end_style, "home_end_style");
	S_PROP(indicator_error, "indicator_error");
	S_PROP(indicator_warning, "indicator_warning");
	S_PROP(indicator_search_highlight, "indicator_search_highlight");
	S_PROP(indicator_select_highlight, "indicator_select_highlight");
	S_PROP(foldComment, "foldComment");
	S_PROP(foldCompact, "foldCompact");
	S_PROP(foldPreproc, "foldPreproc");
	S_PROP(styling_within_preprocessor, "styling_within_preprocessor");
	S_PROP(lexer_cpp_allow_dollars, "lexer_cpp_allow_dollars");
	S_PROP(lexer_cpp_track_preprocessor, "lexer_cpp_track_preprocessor");
	S_PROP(lexer_cpp_update_preprocessor, "lexer_cpp_update_preprocessor");
	S_PROP(lexer_cpp_triplequoted_strings, "lexer_cpp_triplequoted_strings");
	S_PROP(lexer_cpp_hashquoted_strings, "lexer_cpp_hashquoted_strings");
	S_PROP(fold_cpp_syntax_based, "fold_cpp_syntax_based");
	S_PROP(fold_cpp_comment_multiline, "fold_cpp_comment_multiline");
	S_PROP(fold_cpp_comment_explicit, "fold_cpp_comment_explicit");
	S_PROP(fold_cpp_explicit_anywhere, "fold_cpp_explicit_anywhere");
	S_PROP(fold_at_else, "fold_at_else");
	S_PROP(switcher_vs_focus, "switcher_vs_focus");
	S_PROP(switcher_row_count, "switcher_row_count");
	S_PROP(switcher_text_margin_x, "switcher_text_margin_x");
	S_PROP(switcher_text_margin_y, "switcher_text_margin_y");
	S_PROP(switcher_x_margin, "switcher_x_margin");
	S_PROP(switcher_y_margin, "switcher_y_margin");
	S_PROP(switcher_row_spacing, "switcher_row_spacing");
	S_PROP(switcher_col_spacing, "switcher_col_spacing");
	S_PROP(switcher_item_maxwidth, "switcher_item_maxwidth");
	S_PROP(switcher_item_maxheight, "switcher_item_maxheight");
	S_PROP(switcher_dlg_main_border, "switcher_dlg_main_border");
	S_PROP(switcher_dlg_item_border, "switcher_dlg_item_border");
	S_PROP(switcher_min_width, "switcher_min_width");
	S_PROP(switcher_min_height, "switcher_min_height");
};

/* ******************************************************************************************* */

F_STRUCT(OpenFile)
{
	S_PROP(path, "path");
	S_PROP(start_line, "start_line");
	S_PROP(start_index, "start_index");
	S_PROP(end_line, "end_line");
	S_PROP(end_index, "end_index");
};

F_STRUCT(Session)
{
	S_PROP(window_x, "window_x");
	S_PROP(window_y, "window_y");
	S_PROP(window_width, "window_width");
	S_PROP(window_height, "window_height");
	S_PROP(state, "state");
	S_PROP(aui_perspective, "aui_perspective");
};

F_STRUCT(ProjectSession)
{
	// todo(Gustav): add support for project explorer
	S_PROP_V(files, "files");
}

/* ******************************************************************************************* */


F_STRUCT(FindDlg)
{
	S_PROP(sub_folders, "sub_folders");
	S_PROP(match_case, "match_case");
	S_PROP(match_whole_word, "match_whole_word");
	S_PROP(match_start, "match_start");
	S_PROP(target, "target");
	S_PROP(file_types, "file_types");
};

F_STRUCT(DialogData)
{
	S_PROP(find_dlg, "find_dlg");
};

/* ******************************************************************************************* */

F_STRUCT(BuildSetting)
{
	S_PROP(name, "name");
	S_PROP(release, "release");
	S_PROP_V(features, "features");
	S_PROP(default_features, "default_features");
	S_PROP(target, "target");
	S_PROP(verbose, "verbose");
	S_PROP(custom_arguments, "custom_arguments");
};

F_STRUCT(Project)
{
	S_PROP(tabWidth, "tabWidth");
	S_PROP(useTabs, "useTabs");
	S_PROP_V(build_settings, "build_settings");
};

/* ******************************************************************************************* */

F_STRUCT(RunSetting)
{
	S_PROP(name, "name");
	S_PROP(application, "application");
	S_PROP(arguments, "arguments");
	S_PROP(folder, "folder");
	S_PROP(cmd_before_launch, "cmd_before_launch");
	S_PROP(wait_for_exit, "wait_for_exit");
};

F_STRUCT(UserProject)
{
	S_PROP(build_setting, "build_setting");
	S_PROP(run_setting, "run_setting");
	S_PROP_V(run, "run");
};

/* ****************************************************************************************** */

F_STRUCT(MachineSettings)
{
	S_PROP(cargo, "cargo");
	S_PROP(rustc, "rustc");
	S_PROP(protoc, "protoc");
	S_PROP(racer, "racer");
	S_PROP(rust_src_path, "rust_src_path");
};


// ------------------------------------------------------------------------------------------------
// facade

template<typename T>
wxString GenericLoad(T* mess, const wxFileName& file)
{
	std::ifstream f(file.GetFullPath().ToStdString());
	if(f.good() == false)
	{
		return "failed to open file";
	}

	auto filer = Filer{true, nlohmann::json::parse(f)};
	ser(&filer, mess);
	return "";
}

template<typename T>
wxString GenericSave(T* mess, const wxFileName& file)
{
	auto filer = Filer{false, {}};
	ser(&filer, mess);

	// make sure dir exist
	const auto dir = file.GetPath();
	if(wxFileName::DirExists(dir) == false)
	{
		if(false == wxFileName::Mkdir(dir))
		{
			return "failed to create dir: " + file.GetFullPath();
		}
	}
	
	std::ofstream f(file.GetFullPath().ToStdString());
	f << filer.json.dump(4);
	if(!f.good())
	{
		return "failed to write file to " + file.GetFullPath();
	}

	return "";
}

wxString SaveProtoJson(ride::UserProject* mess, const wxFileName& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::Theme* mess, const wxFileName& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::Settings* mess, const wxFileName& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::Session* mess, const wxFileName& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::Project* mess, const wxFileName& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::MachineSettings* mess, const wxFileName& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::ProjectSession* mess, const wxFileName& file) { return GenericSave(mess, file); }

wxString LoadProtoJson(ride::UserProject* mess, const wxFileName& file) { return GenericLoad(mess, file); }
wxString LoadProtoJson(ride::Theme* mess, const wxFileName& file) { return GenericLoad(mess, file); }
wxString LoadProtoJson(ride::Settings* mess, const wxFileName& file) { return GenericLoad(mess, file); }
wxString LoadProtoJson(ride::Session* mess, const wxFileName& file) { return GenericLoad(mess, file); }
wxString LoadProtoJson(ride::Project* mess, const wxFileName& file) { return GenericLoad(mess, file); }
wxString LoadProtoJson(ride::MachineSettings* mess, const wxFileName& file) { return GenericLoad(mess, file); }
wxString LoadProtoJson(ride::ProjectSession* mess, const wxFileName& file) { return GenericLoad(mess, file); }












