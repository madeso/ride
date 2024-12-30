#pragma once

#include <string>
#include <vector>
#include <optional>

namespace ride
{

enum EdgeStyle {
  EDGESTYLE_NONE,
  EDGESTYLE_LINE,
  EDGESTYLE_BACKGROUND,
};

enum ViewWhitespace {
  VIEWWHITESPACE_HIDDEN,
  VIEWWHITESPACE_ALWAYS,
  VIEWWHITESPACE_AFTER_IDENT,
};

enum WrapMode {
  WRAPMODE_NONE,
  WRAPMODE_CHAR,
  WRAPMODE_WORD,
};

enum AutoIndentation {
	AUTOINDENTATION_NONE,
	AUTOINDENTATION_KEEP,
	AUTOINDENTATION_SMART,
};

enum IndicatorStyle {
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

enum VirtualSpace {
	VIRTUALSPACE_NONE,
	VIRTUALSPACE_RECTANGULARSELECTION,
	VIRTUALSPACE_USERACCESSIBLE,
};

enum WrapVisualFlagsLocation {
	WRAPVISUALFLAGLOC_DEFAULT, // Visual flags drawn near border
	WRAPVISUALFLAGLOC_END_BY_TEXT, // Visual flag at end of subline drawn near text
	WRAPVISUALFLAGLOC_START_BY_TEXT, // Visual flag at beginning of subline drawn near text
};

enum WrapIndentMode {
	WRAPINDENT_FIXED, // Wrapped sublines aligned to left of window plus amount set by wrap_start_indent
	WRAPINDENT_SAME, // Wrapped sublines are aligned to first subline indent
	WRAPINDENT_INDENT, // Wrapped sublines are aligned to first subline indent plus one more level of indentation
};

enum Annotation {
	ANNOTATION_HIDDEN, // 	Annotations are not displayed.
	ANNOTATION_STANDARD, // 	Annotations are drawn left justified with no adornment.
	ANNOTATION_BOXED, // 	Annotations are indented to match the text and are surrounded by a box.
};

enum MarkerSymbol {
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

enum CaretStyle {
	CARETSTYLE_INVISIBLE,
	CARETSTYLE_LINE,
	CARETSTYLE_BLOCK,
};

enum CaretSticky {
	CARETSTICKY_OFF,
	CARETSTICKY_WHITESPACE,
	CARETSTICKY_ON,
};

enum StatusbarStyle {
	STATUSBAR_STYLE_NONE,
	STATUSBAR_STYLE_BAR,
	STATUSBAR_STYLE_RAISED,
	STATUSBAR_STYLE_SUNKEN
};

enum AutoComplete {
	AUTOCOMPLETE_NONE,
	AUTOCOMPLETE_PARA
};

enum HomeEndStyle {
	HES_BASIC,
	HES_DISPLAY,
	HES_WRAP,
	HES_VCBASIC,
	HES_VCDISPLAY,
	HES_VCWRAP,
	HES_SCROLL
};

enum WindowState {
  WINDOWSTATE_NORMAL,
  WINDOWSTATE_ICONIZED,
  WINDOWSTATE_MAXIMIZED,
  WINDOWSTATE_FULLSCREEN
};

enum FindDlgTarget {
	FDT_NORMAL_TEXT,
	FDT_NORMAL_REGEX,
	FDT_NORMAL_POSIX
};


struct Color {
	int r;
	int g;
	int b;
};

struct Indicator {
	Color foreground;
	bool under = true;
	int alpha;
	int outline_alpha;
};

struct FoldFlags {
  bool LINEBEFORE_EXPANDED = false;
  bool LINEBEFORE_CONTRACTED = false;
  bool LINEAFTER_EXPANDED = false;
  bool LINEAFTER_CONTRACTED = false;
  bool LEVELNUMBERS = false;
};

struct Style {
  bool use_typeface = false;
  std::string typeface;

  bool use_bold = false;
  bool bold = false;

  bool use_italic = false;
  bool italic = false;

  bool use_underline = false;
  bool underline = false;

  bool use_font_size = false;
  int font_size = 10;

  bool use_foreground = false;
  Color foreground;

  bool use_background = false;
  Color background;
};

// the idea of moving out all fonts and colors is that likely theese are
// what people want share: obsidian/zenburn/monokai and https://studiostyl.es/
struct FontsAndColors {
  Color selected_line;
  Color fold_margin_hi;
  Color fold_margin_low;

  bool use_selection_foreground;
  Color selection_foreground;
  bool use_selection_background;
  Color selection_background;

  Color edgeColor;

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

  Color folderend_foreground;
  Color folderopenmid_foreground;
  Color foldermidtail_foreground;
  Color foldertail_foreground;
  Color foldersub_foreground;
  Color folder_foreground;
  Color folderopen_foreground;

  Color folderend_background;
  Color folderopenmid_background;
  Color foldermidtail_background;
  Color foldertail_background;
  Color foldersub_background;
  Color folder_background;
  Color folderopen_background;

  Color caret_foreground;

  // common styles
  std::optional<Style> style_comment;
  std::optional<Style> style_commentline;
  std::optional<Style> style_commentdoc;
  std::optional<Style> style_number;
  std::optional<Style> style_keyword;
  std::optional<Style> style_string;
  std::optional<Style> style_character;
  std::optional<Style> style_uuid;
  std::optional<Style> style_preprocessor;
  std::optional<Style> style_operator;
  std::optional<Style> style_identifier;
  std::optional<Style> style_string_eol;
  std::optional<Style> style_verbatim;
  std::optional<Style> style_regex;
  std::optional<Style> style_commentlinedoc;
  std::optional<Style> style_keyword_types;
  std::optional<Style> style_commentdockeyword;
  std::optional<Style> style_commentdockeyworderror;
  std::optional<Style> style_globalclass;
  std::optional<Style> style_stringraw;
  std::optional<Style> style_tripleverbatim;
  std::optional<Style> style_hashquotedstring;
  std::optional<Style> style_preprocessorcomment;

  std::optional<Style> markdown_line_begin;
  std::optional<Style> markdown_strong1;
  std::optional<Style> markdown_strong2;
  std::optional<Style> markdown_em1;
  std::optional<Style> markdown_em2;
  std::optional<Style> markdown_header1;
  std::optional<Style> markdown_header2;
  std::optional<Style> markdown_header3;
  std::optional<Style> markdown_header4;
  std::optional<Style> markdown_header5;
  std::optional<Style> markdown_header6;
  std::optional<Style> markdown_prechar;
  std::optional<Style> markdown_ulist_item;
  std::optional<Style> markdown_olist_item;
  std::optional<Style> markdown_blockquote;
  std::optional<Style> markdown_strikeout;
  std::optional<Style> markdown_hrule;
  std::optional<Style> markdown_link;
  std::optional<Style> markdown_code;
  std::optional<Style> markdown_code2;
  std::optional<Style> markdown_codebk;
  
  std::optional<Style> props_section;
  std::optional<Style> props_assignment;
  std::optional<Style> props_defval;
  std::optional<Style> props_key;
  
  std::optional<Style> h_tag;
  std::optional<Style> h_tagunknown;
  std::optional<Style> h_attribute;
  std::optional<Style> h_attributeunknown;
  std::optional<Style> h_number;
  std::optional<Style> h_doublestring;
  std::optional<Style> h_singlestring;
  std::optional<Style> h_other;
  std::optional<Style> h_entity;
  std::optional<Style> h_tagend;
  std::optional<Style> h_xmlstart;
  std::optional<Style> h_xmlend;
  std::optional<Style> h_script;
  std::optional<Style> h_asp;
  std::optional<Style> h_aspat;
  std::optional<Style> h_cdata;
  std::optional<Style> h_question;
  std::optional<Style> h_value;
  std::optional<Style> h_xccomment;
  
  std::optional<Style> cmake_stringdq;
  std::optional<Style> cmake_stringlq;
  std::optional<Style> cmake_stringrq;
  std::optional<Style> cmake_commands;
  std::optional<Style> cmake_parameters;
  std::optional<Style> cmake_variable;
  std::optional<Style> cmake_userdefined;
  std::optional<Style> cmake_whiledef;
  std::optional<Style> cmake_foreachdef;
  std::optional<Style> cmake_ifdefinedef;
  std::optional<Style> cmake_macrodef;
  std::optional<Style> cmake_stringvar;
  std::optional<Style> cmake_number;

  std::optional<Style> lua_label;
  
  std::optional<Style> yaml_number;
  std::optional<Style> yaml_reference;
  std::optional<Style> yaml_document;
  std::optional<Style> yaml_text;
  std::optional<Style> yaml_error;

  std::optional<Style> rust_lifetime;
  std::optional<Style> rust_lex_error;

  Color dock_background			   ;
  Color dock_sash					   ;
  Color dock_active_caption		   ;
  Color dock_active_caption_gradient  ;
  Color dock_inactive_caption		   ;
  Color dock_inactive_caption_gradient;
  Color dock_active_caption_text	   ;
  Color dock_inactive_caption_text	   ;
  Color dock_border				   ;
  Color dock_gripper				   ;

  Color tab_background				   ;
  Color tab_border				       ;
  Color tab_sash				       ;
  Color tab_active_tab				   ;
  Color tab_inactive_tab			   ;
  Color tab_active_border			   ;
  Color tab_inactive_border		   ;
  Color tab_active_text			   ;
  Color tab_inactive_text			   ;
  
  StatusbarStyle statusbar_style	    = STATUSBAR_STYLE_BAR;
  Color statusbar_shadow		       ;
  Color statusbar_highlight	       ;
  Color statusbar_foreground		   ;
  Color statusbar_background		   ;

  Color switcher_background_color;
  Color switcher_text_color;
  Color switcher_selection_color;
  Color switcher_selection_outline_color;
  Color switcher_dialog_color;
  Color switcher_base_color;
};

struct Theme {
	std::string name;
	// std::string author;
	// std::string description;
	bool can_remove = true;

	FontsAndColors data;
};

struct WrapVisualFlags {
	bool end = false; // Visual flag at end of subline of a wrapped line.
	bool start = false; // Visual flag at begin of subline of a wrapped line. Subline is indented by at least 1 to make room for the flag.
	bool margin = false; // Visual flag in line number margin.
};

struct Settings {
  bool lineNumberEnable = true;
  bool foldEnable = true;
  bool displayEOLEnable;
  bool indentGuideEnable = true;
  ViewWhitespace whitespace;
  WrapMode wordWrap;

  EdgeStyle edgeStyle = EDGESTYLE_LINE;
  
  bool has_fonts_and_colors = false;
  FontsAndColors fonts_and_colors;

  int edgeColumn = 80;

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
  WrapVisualFlagsLocation wrap_visual_flags_location;
  WrapIndentMode wrap_indent_mode;
  int wrap_start_indent;
  Annotation annotations = ANNOTATION_BOXED;

  bool current_line_visible = false;
  int current_line_alpha = 10;
  bool current_line_overdraw = false;

  bool highlight_word_also_highlight_keywords = false;

  AutoIndentation auto_indentation = AUTOINDENTATION_SMART;

  MarkerSymbol folderend = MARKERSYMBOL_EMPTY;
  MarkerSymbol folderopenmid = MARKERSYMBOL_EMPTY;
  MarkerSymbol foldermidtail = MARKERSYMBOL_EMPTY;
  MarkerSymbol foldertail = MARKERSYMBOL_EMPTY;
  MarkerSymbol foldersub = MARKERSYMBOL_EMPTY;
  MarkerSymbol folder = MARKERSYMBOL_EMPTY;
  MarkerSymbol folderopen = MARKERSYMBOL_EMPTY;

  AutoComplete autocomplete_curly_braces = AUTOCOMPLETE_PARA;
  AutoComplete autocomplete_parentheses = AUTOCOMPLETE_PARA;
  AutoComplete autocomplete_brackets = AUTOCOMPLETE_PARA;
  AutoComplete autocomplete_doublequote = AUTOCOMPLETE_PARA;
  AutoComplete autocomplete_singlequote = AUTOCOMPLETE_PARA;

  bool show_multiline_indicators = false;

  bool show_compiler_messages_as_annotations = true;

  std::vector<Theme> themes;

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

  // settings for c++
  bool foldComment = true;
  bool foldCompact = true;
  bool foldPreproc = true;
  bool     styling_within_preprocessor = false;     //  For C++ code determines whether all preprocessor code is styled in the preprocessor style (0, the default) or only from the initial # to the end of the command word(1).
  bool     lexer_cpp_allow_dollars = false;         // Set to 0 to disallow the '$' character in identifiers with the cpp lexer.
  bool     lexer_cpp_track_preprocessor = false;    //  Set to 1 to interpret #if/#else/#endif to grey out code that is not active.
  bool     lexer_cpp_update_preprocessor = false;   //  Set to 1 to update preprocessor definitions when #define found.
  bool     lexer_cpp_triplequoted_strings = false;  //  Set to 1 to enable highlighting of triple-quoted std::strings.
  bool     lexer_cpp_hashquoted_strings = false;    //  Set to 1 to enable highlighting of hash-quoted std::strings.
  bool     fold_cpp_syntax_based = true;           //  Set this property to 0 to disable syntax based folding.
  bool     fold_cpp_comment_multiline = true;      //  Set this property to 0 to disable folding multi-line comments when fold.comment=1.
  bool     fold_cpp_comment_explicit = true;       //  Set this property to 0 to disable folding explicit fold points when fold.comment=1.
  bool     fold_cpp_explicit_anywhere = false;      //  Set this property to 1 to enable explicit fold points anywhere, not just in line comments.
  bool     fold_at_else = true;                    //  This option enables C++ folding on a "} else {" line of an if statement.

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

struct OpenFile {
  std::string path;

  int start_line = -1;
  int start_index = -1;
  int end_line = -1;
  int end_index = -1;
};

struct Session {
  int window_x;
  int window_y;
  int window_width;
  int window_height;
  WindowState state = WINDOWSTATE_NORMAL;

  std::vector<OpenFile> files;
  std::string project;
  std::string aui_perspective;
};

/* ******************************************************************************************* */


struct FindDlg {
  bool sub_folders = true;
  bool match_case = true;
  bool match_whole_word = false;
  bool match_start = false;
  FindDlgTarget target = FDT_NORMAL_TEXT;
  std::string file_types = "*.rs;*.toml";
};

struct DialogData {
	FindDlg find_dlg;
};

/* ******************************************************************************************* */

struct BuildSetting {
	std::string name = "<untitled build setting>";
	bool release = false;
	std::vector<std::string> features;
	bool default_features = true;
	std::string target = "";
	bool verbose = false;

	std::string custom_arguments = "";
};

struct Project {
  int tabWidth = 4;
  bool useTabs = false;

  std::vector<BuildSetting> build_settings;
};

/* ******************************************************************************************* */

struct RunSetting {
	std::string name = "Run";

	std::string application = "%built_application%";
	std::string arguments = "";
	std::string folder = "%cargo_folder%";
	std::string cmd_before_launch = "";
	bool wait_for_exit = false;
};

struct UserProject {
	int build_setting = -1;
	int run_setting = -1;
	std::vector<RunSetting> run;
};


/* ****************************************************************************************** */

struct MachineSettings {
	std::string cargo = "cargo";
	std::string rustc = "rustc";
	std::string protoc = "protoc";
	std::string racer = "racer";
	std::string rust_src_path = "";
};

}
