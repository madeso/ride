#pragma once

#include <string>
#include <vector>
#include <optional>
#include <set>

namespace ride
{

enum EdgeStyle
{
	EDGESTYLE_NONE,
	EDGESTYLE_LINE,
	EDGESTYLE_BACKGROUND,
};

enum ViewWhitespace
{
	VIEWWHITESPACE_HIDDEN,
	VIEWWHITESPACE_ALWAYS,
	VIEWWHITESPACE_AFTER_IDENT,
};

enum WrapMode
{
	WRAPMODE_NONE,
	WRAPMODE_CHAR,
	WRAPMODE_WORD,
};

enum AutoIndentation
{
	AUTOINDENTATION_NONE,
	AUTOINDENTATION_KEEP,
	AUTOINDENTATION_SMART,
};

enum IndicatorStyle
{
	INDICATORSTYLE_PLAIN,
	INDICATORSTYLE_SQUIGGLE,
	INDICATORSTYLE_TT,
	INDICATORSTYLE_DIAGONAL,
	INDICATORSTYLE_STRIKE,
	INDICATORSTYLE_HIDDEN,
	INDICATORSTYLE_BOX,
	INDICATORSTYLE_ROUNDBOX,
	INDICATORSTYLE_STRAIGHTBOX,
	INDICATORSTYLE_DASH,
	INDICATORSTYLE_DOTS,
	INDICATORSTYLE_SQUIGGLELOW,
	INDICATORSTYLE_DOTBOX,
};

enum VirtualSpace
{
	VIRTUALSPACE_NONE,
	VIRTUALSPACE_RECTANGULARSELECTION,
	VIRTUALSPACE_USERACCESSIBLE,
};

enum WrapVisualFlagsLocation
{
	/// Visual flags drawn near border
	WRAPVISUALFLAGLOC_DEFAULT,
	/// Visual flag at end of subline drawn near text
	WRAPVISUALFLAGLOC_END_BY_TEXT,
	/// Visual flag at beginning of subline drawn near text
	WRAPVISUALFLAGLOC_START_BY_TEXT,
};

enum WrapIndentMode
{
	/// Wrapped sublines aligned to left of window plus amount set by wrap_start_indent
	WRAPINDENT_FIXED,
	/// Wrapped sublines are aligned to first subline indent
	WRAPINDENT_SAME,
	/// Wrapped sublines are aligned to first subline indent plus one more level of indentation
	WRAPINDENT_INDENT,
};

enum Annotation
{
	ANNOTATION_HIDDEN,	// 	Annotations are not displayed.
	ANNOTATION_STANDARD,  // 	Annotations are drawn left justified with no adornment.
	ANNOTATION_BOXED,  // 	Annotations are indented to match the text and are surrounded by a box.
};

enum MarkerSymbol
{
	MARKERSYMBOL_CIRCLE,
	MARKERSYMBOL_ROUNDRECT,
	MARKERSYMBOL_ARROW,
	MARKERSYMBOL_SMALLRECT,
	MARKERSYMBOL_SHORTARROW,
	MARKERSYMBOL_EMPTY,
	MARKERSYMBOL_ARROWDOWN,
	MARKERSYMBOL_MINUS,
	MARKERSYMBOL_PLUS,
};

enum CaretStyle
{
	CARETSTYLE_INVISIBLE,
	CARETSTYLE_LINE,
	CARETSTYLE_BLOCK,
};

enum CaretSticky
{
	CARETSTICKY_OFF,
	CARETSTICKY_WHITESPACE,
	CARETSTICKY_ON,
};

enum StatusbarStyle
{
	STATUSBAR_STYLE_NONE,
	STATUSBAR_STYLE_BAR,
	STATUSBAR_STYLE_RAISED,
	STATUSBAR_STYLE_SUNKEN
};

enum AutoComplete
{
	AUTOCOMPLETE_NONE,
	AUTOCOMPLETE_PARA
};

enum HomeEndStyle
{
	HES_BASIC,
	HES_DISPLAY,
	HES_WRAP,
	HES_VCBASIC,
	HES_VCDISPLAY,
	HES_VCWRAP,
	HES_SCROLL
};

enum WindowState
{
	WINDOWSTATE_NORMAL,
	WINDOWSTATE_ICONIZED,
	WINDOWSTATE_MAXIMIZED,
	WINDOWSTATE_FULLSCREEN
};

enum FindDlgTarget
{
	FDT_NORMAL_TEXT,
	FDT_NORMAL_REGEX,
	FDT_NORMAL_POSIX
};

struct Color
{
	int r = 0;
	int g = 0;
	int b = 0;
};

struct Indicator
{
	wxString foreground;
	bool under = true;
	int alpha = 0;
	int outline_alpha;
};

struct FoldFlags
{
	bool LINEBEFORE_EXPANDED = false;
	bool LINEBEFORE_CONTRACTED = false;
	bool LINEAFTER_EXPANDED = false;
	bool LINEAFTER_CONTRACTED = false;
	bool LEVELNUMBERS = false;
};

struct Style
{
	std::optional<std::string> typeface;

	std::optional<bool> bold;
	std::optional<bool> italic;
	std::optional<bool> underline;
	std::optional<int> font_size;
	
	std::optional<wxString> foreground;
	std::optional<wxString> background;
};

#define LOOKUP_COLOR(c, name) (c).GetColor((c).name, #name)
#define LOOKUP_COLOR_R(c, name) (c).GetColor(*(c).name, #name)

/// the idea of moving out all fonts and colors is that likely theese are
/// what people want share: obsidian/zenburn/monokai and https://studiostyl.es/
struct FontsAndColors
{
	std::unordered_map<wxString, Color> colors;	// named colors
	std::unordered_map<wxString, Style> styles;	 // named styles
	std::unordered_map<wxString, wxString> alias; // color/style alias

	std::optional<Style> GetStyle(const wxString& name) const;
	std::optional<wxColor> GetColor(const wxString& name, const char* const display_name) const;

	wxString selected_line;
	wxString fold_margin_hi;
	wxString fold_margin_low;

	std::optional<wxString> selection_foreground;
	std::optional<wxString> selection_background;

	wxString edgeColor;

	// indicators
	Indicator indicator_error;
	Indicator indicator_warning;
	Indicator indicator_search_highlight;
	Indicator indicator_select_highlight;

	std::optional<Style> line_number_style;
	Style default_style;
	std::optional<Style> bracelight_style;
	std::optional<Style> bracebad_style;
	std::optional<Style> controlchar_style;
	std::optional<Style> indentguide_style;
	std::optional<Style> calltip_style;
	std::optional<Style> annotation_warning_style;
	std::optional<Style> annotation_error_style;

	wxString explorer_foreground;
	wxString explorer_background;

	wxString marker_foreground;
	wxString marker_background;

	wxString caret_foreground;

	wxString dock_background;
	wxString dock_sash;
	wxString dock_active_caption;
	wxString dock_active_caption_gradient;
	wxString dock_inactive_caption;
	wxString dock_inactive_caption_gradient;
	wxString dock_active_caption_text;
	wxString dock_inactive_caption_text;
	wxString dock_border;
	wxString dock_gripper;

	wxString tab_background;
	wxString tab_border;
	wxString tab_sash;
	wxString tab_active_tab;
	wxString tab_inactive_tab;
	wxString tab_active_border;
	wxString tab_inactive_border;
	wxString tab_active_text;
	wxString tab_inactive_text;

	wxString statusbar_shadow;
	wxString statusbar_highlight;
	wxString statusbar_foreground;
	wxString statusbar_background;

	wxString switcher_background_color;
	wxString switcher_text_color;
	wxString switcher_selection_color;
	wxString switcher_selection_outline_color;
	wxString switcher_dialog_color;
	wxString switcher_base_color;
};

struct Theme
{
	std::string name;
	// std::string author;
	// std::string description;
	bool can_remove = true;

	FontsAndColors data;
};

struct WrapVisualFlags
{
	/// Visual flag at end of subline of a wrapped line.
	bool end = false;
	/// Visual flag at begin of subline of a wrapped line. Subline is indented by at least 1 to make room for the flag.
	bool start = false;
	/// Visual flag in line number margin.
	bool margin = false;
};

struct Settings
{
	bool lineNumberEnable = true;
	bool foldEnable = true;
	bool displayEOL;
	bool indentGuideEnable = true;
	ViewWhitespace whitespace = VIEWWHITESPACE_AFTER_IDENT;
	WrapMode wordWrap = WRAPMODE_NONE;

	EdgeStyle edgeStyle = EDGESTYLE_LINE;

	wxString LookupSetting(const wxString& name) const;

	std::string current_theme;

	int edgeColumn = 80;

	StatusbarStyle statusbar_style = STATUSBAR_STYLE_BAR;

	int tabWidth = 4;
	bool useTabs = false;
	bool tabIndents = true;
	bool backspaceUnindents = true;

	FoldFlags foldflags;
	VirtualSpace virtual_space = VIRTUALSPACE_NONE;
	bool end_at_last_line = false;
	bool vertical_scrollbar = true;
	bool horizontal_scrollbar = true;

	WrapVisualFlags wrap_visual_flags;
	WrapVisualFlagsLocation wrap_visual_flags_location = WRAPVISUALFLAGLOC_DEFAULT;
	WrapIndentMode wrap_indent_mode = WRAPINDENT_FIXED;
	int wrap_start_indent = 120;
	Annotation annotations = ANNOTATION_BOXED;

	bool current_line_visible = false;
	int current_line_alpha = 10;
	bool current_line_overdraw = false;

	bool highlight_word_also_highlight_keywords = false;

	AutoIndentation auto_indentation = AUTOINDENTATION_SMART;

	// MarkerSymbol folderend = MARKERSYMBOL_EMPTY;
	// MarkerSymbol folderopenmid = MARKERSYMBOL_EMPTY;
	// MarkerSymbol foldermidtail = MARKERSYMBOL_EMPTY;
	// MarkerSymbol foldertail = MARKERSYMBOL_EMPTY;
	// MarkerSymbol foldersub = MARKERSYMBOL_EMPTY;
	// MarkerSymbol folder = MARKERSYMBOL_EMPTY;
	// MarkerSymbol folderopen = MARKERSYMBOL_EMPTY;

	AutoComplete autocomplete_curly_braces = AUTOCOMPLETE_PARA;
	AutoComplete autocomplete_parentheses = AUTOCOMPLETE_PARA;
	AutoComplete autocomplete_brackets = AUTOCOMPLETE_PARA;
	AutoComplete autocomplete_doublequote = AUTOCOMPLETE_PARA;
	AutoComplete autocomplete_singlequote = AUTOCOMPLETE_PARA;

	bool show_multiline_indicators = false;

	bool show_compiler_messages_as_annotations = true;

	std::vector<Theme> themes;
	const FontsAndColors* find_current_theme() const;

	int caret_period = 500;
	int caret_width = 1;
	CaretSticky caret_sticky = CARETSTICKY_ON;
	CaretStyle caret_style = CARETSTYLE_LINE;

	HomeEndStyle home_end_style = HES_VCWRAP;

	// indicators
	IndicatorStyle indicator_error = INDICATORSTYLE_SQUIGGLE;
	IndicatorStyle indicator_warning = INDICATORSTYLE_SQUIGGLE;
	IndicatorStyle indicator_search_highlight = INDICATORSTYLE_SQUIGGLE;
	IndicatorStyle indicator_select_highlight = INDICATORSTYLE_SQUIGGLE;

	// settings for ctrl-tab

	bool switcher_vs_focus = true;
	int switcher_row_count = 12;

	int switcher_text_margin_x = 4;
	int switcher_text_margin_y = 2;
	int switcher_x_margin = 3;
	int switcher_y_margin = 3;
	int switcher_row_spacing = 1;
	int switcher_col_spacing = 1;
	int switcher_item_maxwidth = 300;
	int switcher_item_maxheight = 40;
	int switcher_dlg_main_border = 8;
	int switcher_dlg_item_border = 3;

	int switcher_min_width = 1;
	int switcher_min_height = 1;
};

/* ******************************************************************************************* */

using KeywordList = std::set<wxString>;

struct Language
{
	wxString language_name;
	wxString lexer_style;

	// file pattern could be both a extension '.txt' or that the file must be named 'CMakeLists.txt'
	std::vector<wxString> file_patterns;
	std::vector<KeywordList> keywords;
	std::unordered_map<wxString, wxString> properties;

	// language specific mapping
	std::unordered_map<wxString, wxString> bindings;  // scintilla id to style name
};

struct Languages
{
	Language null_language;
	std::vector<Language> languages;
};

/* ******************************************************************************************* */

struct OpenFile
{
	std::string path;

	int start_line = -1;
	int start_index = -1;
	int end_line = -1;
	int end_index = -1;
};

struct Session
{
	int window_x = 0;
	int window_y = 0;
	int window_width = 0;
	int window_height = 0;
	WindowState state = WINDOWSTATE_NORMAL;

	// move perspective to project session?
	std::string aui_perspective;
};

struct ProjectSession
{
	std::vector<OpenFile> files;
};

/* ******************************************************************************************* */


struct FindDlg
{
	bool sub_folders = true;
	bool match_case = true;
	bool match_whole_word = false;
	bool match_start = false;
	FindDlgTarget target = FDT_NORMAL_TEXT;
	std::string file_types = "*.rs;*.toml";
};

struct DialogData
{
	FindDlg find_dlg;
};

/* ******************************************************************************************* */

struct BuildSetting
{
	std::string name = "<untitled build setting>";

	std::string folder = "";
	std::string build = "";
	std::string clean = "";
};

struct Project
{
	int tabWidth = 4;
	bool useTabs = false;

	std::vector<BuildSetting> build_settings;
};

/* ******************************************************************************************* */

struct RunSetting
{
	std::string name = "Run";

	std::string application = "%built_application%";
	std::string arguments = "";
	std::string folder = "%cargo_folder%";
	std::string cmd_before_launch = "";
	bool wait_for_exit = false;
};

struct UserProject
{
	int build_setting = -1;
	int run_setting = -1;
	std::vector<RunSetting> run;
};

/* ****************************************************************************************** */

struct MachineSettings
{
	std::string cargo = "cargo";
	std::string rustc = "rustc";
	std::string protoc = "protoc";
	std::string racer = "racer";
	std::string rust_src_path = "";
};

}  //  namespace ride
