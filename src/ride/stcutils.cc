#include "ride/stcutils.h"

#include <ride/wx.h>

#include <wx/stc/stc.h>

#include <string>
#include <vector>
#include <algorithm>

#include "settings.proto.h"	 // NOLINT this is how we include it

#include "ride/language.h"
#include "ride/project.h"
#include "ride/wxutils.h"

enum
{
	FOLDING_WIDTH = 16
};

int C(ride::EdgeStyle e)
{
	switch (e)
	{
	case ride::EDGESTYLE_BACKGROUND: return wxSTC_EDGE_BACKGROUND;
	case ride::EDGESTYLE_LINE: return wxSTC_EDGE_LINE;
	case ride::EDGESTYLE_NONE: return wxSTC_EDGE_NONE;
	default: assert(false && "Invalid edge style"); return wxSTC_EDGE_NONE;
	}
}

int C(ride::ViewWhitespace e)
{
	switch (e)
	{
	case ride::VIEWWHITESPACE_ALWAYS: return wxSTC_WS_VISIBLEALWAYS;
	case ride::VIEWWHITESPACE_AFTER_IDENT: return wxSTC_WS_VISIBLEAFTERINDENT;
	case ride::VIEWWHITESPACE_HIDDEN: return wxSTC_WS_INVISIBLE;
	default: assert(false && "Invalid whitespace style"); return wxSTC_WS_INVISIBLE;
	}
}

int C(ride::WrapMode e)
{
	switch (e)
	{
	case ride::WRAPMODE_CHAR: return wxSTC_WRAP_CHAR;
	case ride::WRAPMODE_WORD: return wxSTC_WRAP_WORD;
	case ride::WRAPMODE_NONE: return wxSTC_WRAP_NONE;
	default: assert(false && "Invalid wrap mode"); return wxSTC_WRAP_NONE;
	}
}

int C(ride::FoldFlags f)
{
	int ret = 0;

	if (f.LINEBEFORE_EXPANDED)
	{
		ret |= wxSTC_FOLDFLAG_LINEBEFORE_EXPANDED;
	}
	if (f.LINEBEFORE_CONTRACTED)
	{
		ret |= wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED;
	}
	if (f.LINEAFTER_EXPANDED)
	{
		ret |= wxSTC_FOLDFLAG_LINEAFTER_EXPANDED;
	}
	if (f.LINEAFTER_CONTRACTED)
	{
		ret |= wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED;
	}
	if (f.LEVELNUMBERS)
	{
		ret |= wxSTC_FOLDFLAG_LEVELNUMBERS;
	}

	return ret;
}

bool Contains(const std::vector<wxString>& vec, const std::string& str)
{
	return std::find(vec.begin(), vec.end(), str) != vec.end();
}

std::string DetermineTypeface(const std::string& suggestion)
{
	const std::vector<wxString> all_fonts = ListFonts(false);
	if (Contains(all_fonts, suggestion))
	{
		// the suggestion is found
		return suggestion;
	}

	const std::vector<wxString> monospaced = ListFonts(true);

#define FONTCHECK(FONTNAME) \
	do \
	{ \
		if (Contains(monospaced, FONTNAME)) \
		{ \
			return FONTNAME; \
		} \
	} while (false)
	// http://hivelogic.com/articles/top-10-programming-fonts
	// http://www.slant.co/topics/67/~what-are-the-best-programming-fonts
	FONTCHECK("Source Code Pro");
	FONTCHECK("Consolas");
	FONTCHECK("Ubuntu Mono");
	FONTCHECK("Inconsolata-g");
	FONTCHECK("Inconsolata");
	FONTCHECK("Monaco");
	FONTCHECK("DejaVu Sans Mono");
	FONTCHECK("Deja Vu Sans Mono");
	FONTCHECK("Anonymous Pro");
	// FONTCHECK("Menlo"); // doen't work on size 10?
	FONTCHECK("Envy Code R");
	FONTCHECK("Droid Sans Mono");
	FONTCHECK("Liberation Mono");
	FONTCHECK("Monofur");
	FONTCHECK("Proggy");
	FONTCHECK("Profont");
	FONTCHECK("ProFontX");
	FONTCHECK("Andale Mono");
	FONTCHECK("Courier New");
	FONTCHECK("Courier");
#undef FONTCHECK

	return "";
}

void SetStyle(
	wxStyledTextCtrl* text,
	int id,
	const std::optional<ride::Style>& style,
	const ride::FontsAndColors& theme
)
{
	if (style.has_value() == false) return;
	if (style->typeface.has_value())
	{
		const std::string typeface = DetermineTypeface(*style->typeface);
		if (false == typeface.empty())
		{
			text->StyleSetFaceName(id, typeface);
		}
	}

	if (style->bold.has_value())
	{
		text->StyleSetBold(id, *style->bold);
	}
	if (style->italic.has_value())
	{
		text->StyleSetItalic(id, *style->italic);
	}
	if (style->underline.has_value())
	{
		text->StyleSetUnderline(id, *style->underline);
	}
	if (style->font_size.has_value())
	{
		text->StyleSetSize(id, *style->font_size);
	}
	if (style->foreground.has_value())
	{
		if (const auto foreground = theme.GetColor(*style->foreground, "foreground"); foreground.has_value())
		{
			text->StyleSetForeground(id, *foreground);
		}
	}
	if (style->background.has_value())
	{
		if (const auto background = theme.GetColor(*style->background, "background"); background.has_value())
		{
			text->StyleSetBackground(id, *background);
		}
	}
}

int C(const ride::IndicatorStyle style)
{
	switch (style)
	{
	case ride::INDICATORSTYLE_PLAIN: return wxSTC_INDIC_PLAIN;
	case ride::INDICATORSTYLE_SQUIGGLE: return wxSTC_INDIC_SQUIGGLE;
	case ride::INDICATORSTYLE_TT: return wxSTC_INDIC_TT;
	case ride::INDICATORSTYLE_DIAGONAL: return wxSTC_INDIC_DIAGONAL;
	case ride::INDICATORSTYLE_STRIKE: return wxSTC_INDIC_STRIKE;
	case ride::INDICATORSTYLE_HIDDEN: return wxSTC_INDIC_HIDDEN;
	case ride::INDICATORSTYLE_BOX: return wxSTC_INDIC_BOX;
	case ride::INDICATORSTYLE_ROUNDBOX: return wxSTC_INDIC_ROUNDBOX;
	case ride::INDICATORSTYLE_STRAIGHTBOX: return wxSTC_INDIC_STRAIGHTBOX;
	case ride::INDICATORSTYLE_DASH: return wxSTC_INDIC_DASH;
	case ride::INDICATORSTYLE_DOTS: return wxSTC_INDIC_DOTS;
	case ride::INDICATORSTYLE_SQUIGGLELOW: return wxSTC_INDIC_SQUIGGLELOW;
	case ride::INDICATORSTYLE_DOTBOX: return wxSTC_INDIC_DOTBOX;
	default: assert(0 && "Unknown indicator style!"); return wxSTC_INDIC_HIDDEN;
	}
}

int C(const ride::VirtualSpace space)
{
	switch (space)
	{
	case ride::VIRTUALSPACE_NONE: return wxSTC_VS_NONE;
	case ride::VIRTUALSPACE_RECTANGULARSELECTION: return wxSTC_VS_RECTANGULARSELECTION;
	case ride::VIRTUALSPACE_USERACCESSIBLE: return wxSTC_VS_USERACCESSIBLE;
	default: assert(0 && "Unknown virtual space"); return wxSTC_VS_NONE;
	}
}

int C(ride::WrapVisualFlagsLocation loc)
{
	switch (loc)
	{
	case ride::WRAPVISUALFLAGLOC_DEFAULT: return wxSTC_WRAPVISUALFLAGLOC_DEFAULT;
	case ride::WRAPVISUALFLAGLOC_END_BY_TEXT: return wxSTC_WRAPVISUALFLAGLOC_END_BY_TEXT;
	case ride::WRAPVISUALFLAGLOC_START_BY_TEXT: return wxSTC_WRAPVISUALFLAGLOC_START_BY_TEXT;
	default: assert(0 && "Unknown visual flag location"); return wxSTC_WRAPVISUALFLAGLOC_DEFAULT;
	}
}

int C(ride::WrapIndentMode mode)
{
	switch (mode)
	{
	case ride::WRAPINDENT_FIXED: return wxSTC_WRAPINDENT_FIXED;
	case ride::WRAPINDENT_SAME: return wxSTC_WRAPINDENT_SAME;
	case ride::WRAPINDENT_INDENT: return wxSTC_WRAPINDENT_INDENT;
	default: assert(0 && "Unknown wrap indent mode"); return wxSTC_WRAPINDENT_FIXED;
	}
}

int C(ride::WrapVisualFlags flags)
{
	int ret = wxSTC_WRAPVISUALFLAG_NONE;
	if (flags.end)
	{
		ret |= wxSTC_WRAPVISUALFLAG_END;
	}
	if (flags.start)
	{
		ret |= wxSTC_WRAPVISUALFLAG_START;
	}
	if (flags.margin)
	{
		ret |= wxSTC_WRAPVISUALFLAG_MARGIN;
	}
	return ret;
}

int C(ride::Annotation ann)
{
	switch (ann)
	{
	case ride::ANNOTATION_HIDDEN: return wxSTC_ANNOTATION_HIDDEN;
	case ride::ANNOTATION_STANDARD: return wxSTC_ANNOTATION_STANDARD;
	case ride::ANNOTATION_BOXED: return wxSTC_ANNOTATION_BOXED;
	default: assert(0 && "Unknown annotation style"); return wxSTC_ANNOTATION_STANDARD;
	}
}

int C(ride::CaretStyle st)
{
	switch (st)
	{
	case ride::CARETSTYLE_INVISIBLE: return wxSTC_CARETSTYLE_INVISIBLE;
	case ride::CARETSTYLE_LINE: return wxSTC_CARETSTYLE_LINE;
	case ride::CARETSTYLE_BLOCK: return wxSTC_CARETSTYLE_BLOCK;
	default: assert(0 && "Unknown caret sticky"); return wxSTC_CARETSTYLE_LINE;
	}
}

int C(ride::CaretSticky st)
{
	switch (st)
	{
	case ride::CARETSTICKY_OFF: return wxSTC_CARETSTICKY_OFF;
	case ride::CARETSTICKY_WHITESPACE: return wxSTC_CARETSTICKY_WHITESPACE;
	case ride::CARETSTICKY_ON: return wxSTC_CARETSTICKY_ON;
	default: assert(0 && "Unknown caret sticky"); return wxSTC_CARETSTICKY_ON;
	}
}

void SetIndicator(
	wxStyledTextCtrl* text,
	int index,
	const ride::Indicator& indicator,
	const ride::IndicatorStyle indicator_style,
	const ride::FontsAndColors& theme
)
{
	text->IndicatorSetUnder(index, indicator.under);
	text->IndicatorSetAlpha(index, indicator.alpha);
	text->IndicatorSetOutlineAlpha(index, indicator.outline_alpha);

	if (const auto found = theme.GetColor(indicator.foreground, "foreground"); found.has_value())
	{
		text->IndicatorSetForeground(index, *found);
	}

	text->IndicatorSetStyle(index, C(indicator_style));
}

void SetupScintillaCurrentLine(wxStyledTextCtrl* text_ctrl, const ride::Settings& set, const ride::FontsAndColors& theme)
{
	if (const auto selected_line = LOOKUP_COLOR(theme, selected_line); selected_line.has_value())
	{
		text_ctrl->SetCaretLineBackground(*selected_line);
	}

	if (set.current_line_overdraw)
	{
		text_ctrl->SetCaretLineBackAlpha(set.current_line_alpha);
	}
	else
	{
		text_ctrl->SetCaretLineBackAlpha(wxSTC_ALPHA_NOALPHA);
	}
	text_ctrl->SetCaretLineVisible(set.current_line_visible);

	// text_ctrl->SetCaretLineVisibleAlways(true);
	// todo: set SCI_SETCARETLINEVISIBLEALWAYS to true, this will make it easier
	// to change settings and caret

	if (const auto caret_foreground = LOOKUP_COLOR(theme, caret_foreground); caret_foreground.has_value())
	{
		text_ctrl->SetCaretForeground(*caret_foreground);
	}
	text_ctrl->SetCaretPeriod(set.caret_period);
	text_ctrl->SetCaretWidth(set.caret_width);
	text_ctrl->SetCaretSticky(C(set.caret_sticky));
	text_ctrl->SetCaretStyle(C(set.caret_style));
}

void SetupScintillaDefaultStyles(wxStyledTextCtrl* text_ctrl, const ride::FontsAndColors& set)
{
	SetStyle(text_ctrl, wxSTC_STYLE_DEFAULT, set.default_style, set);
	SetStyle(text_ctrl, wxSTC_STYLE_LINENUMBER, set.line_number_style, set);
	SetStyle(text_ctrl, wxSTC_STYLE_BRACELIGHT, set.bracelight_style, set);
	SetStyle(text_ctrl, wxSTC_STYLE_BRACEBAD, set.bracebad_style, set);
	SetStyle(text_ctrl, wxSTC_STYLE_CONTROLCHAR, set.controlchar_style, set);
	SetStyle(text_ctrl, wxSTC_STYLE_INDENTGUIDE, set.indentguide_style, set);
	SetStyle(text_ctrl, wxSTC_STYLE_CALLTIP, set.calltip_style, set);

	SetStyle(text_ctrl, STYLE_ANNOTATION_ERROR, set.annotation_error_style, set);
	SetStyle(text_ctrl, STYLE_ANNOTATION_WARNING, set.annotation_warning_style, set);
}

int C(ride::MarkerSymbol sym)
{
	switch (sym)
	{
	case ride::MARKERSYMBOL_CIRCLE: return wxSTC_MARK_CIRCLE;
	case ride::MARKERSYMBOL_ROUNDRECT: return wxSTC_MARK_ROUNDRECT;
	case ride::MARKERSYMBOL_ARROW: return wxSTC_MARK_ARROW;
	case ride::MARKERSYMBOL_SMALLRECT: return wxSTC_MARK_SMALLRECT;
	case ride::MARKERSYMBOL_SHORTARROW: return wxSTC_MARK_SHORTARROW;
	case ride::MARKERSYMBOL_EMPTY: return wxSTC_MARK_EMPTY;
	case ride::MARKERSYMBOL_ARROWDOWN: return wxSTC_MARK_ARROWDOWN;
	case ride::MARKERSYMBOL_MINUS: return wxSTC_MARK_MINUS;
	case ride::MARKERSYMBOL_PLUS: return wxSTC_MARK_PLUS;
	default: assert(false && "Invalid marker symbol"); return wxSTC_MARK_EMPTY;
	}
}

void SetMarker(
	wxStyledTextCtrl* text_ctrl,
	int number,
	int mark_symbol, // todo(Gustav): switch to ride::MarkerSymbol
	const ride::FontsAndColors& colors
)
{
	text_ctrl->MarkerDefine(
		number,
		mark_symbol,
		LOOKUP_COLOR(colors, marker_foreground).value_or(ride::BLACK),
		LOOKUP_COLOR(colors, marker_background).value_or(ride::WHITE)
	);
}

void SetupLineMargin(wxStyledTextCtrl* text_ctrl, const ride::Settings& set)
{
	// calculate the maximum number a the line margin could contain
	const int number_of_lines = text_ctrl->GetNumberOfLines();

	const size_t length = wxString::Format("%d", number_of_lines).Length();
	const wxString maximum_line_numbers = wxString(length, '9');

	int line_margin_width_ = text_ctrl->TextWidth(
		wxSTC_STYLE_LINENUMBER, wxString::Format("_%s", maximum_line_numbers)
	);
	text_ctrl->SetMarginWidth(ID_MARGIN_LINENUMBER, set.lineNumberEnable ? line_margin_width_ : 0);
}

void SetupScintilla(
	wxStyledTextCtrl* text_ctrl,
	const ride::Settings& set,
	const Language* language,
	Project* project
)
{
	// initialize styles
	text_ctrl->StyleClearAll();

	// setup language color
	assert(language);
	language->StyleDocument(text_ctrl, set);

	//////////////////////////////////////////////////////////////////////////

	// set margin for line numbers
	text_ctrl->SetMarginType(ID_MARGIN_LINENUMBER, wxSTC_MARGIN_NUMBER);
	SetupLineMargin(text_ctrl, set);

	// set margin as unused
	text_ctrl->SetMarginType(ID_MARGIN_DIVIDER, wxSTC_MARGIN_SYMBOL);
	text_ctrl->SetMarginWidth(ID_MARGIN_DIVIDER, 15);
	text_ctrl->SetMarginSensitive(ID_MARGIN_DIVIDER, false);

	// folding settings
	text_ctrl->SetMarginType(ID_MARGIN_FOLDING, wxSTC_MARGIN_SYMBOL);
	text_ctrl->SetMarginMask(ID_MARGIN_FOLDING, wxSTC_MASK_FOLDERS);
	text_ctrl->SetMarginWidth(ID_MARGIN_FOLDING, 15);
	text_ctrl->SetMarginSensitive(ID_MARGIN_FOLDING, true);

	text_ctrl->SetMarginWidth(ID_MARGIN_FOLDING, set.foldEnable ? FOLDING_WIDTH : 0);
	text_ctrl->SetMarginSensitive(ID_MARGIN_FOLDING, set.foldEnable);
	text_ctrl->SetFoldFlags(C(set.foldflags));

	if (const auto* theme = set.find_current_theme())
	{
		text_ctrl->SetFoldMarginColour(true, LOOKUP_COLOR(*theme, fold_margin_low).value_or(ride::BLACK));
		text_ctrl->SetFoldMarginHiColour(true, LOOKUP_COLOR(*theme, fold_margin_hi).value_or(ride::WHITE));

		SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED, *theme);
		SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED, *theme);
		SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER, *theme);
		SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER, *theme);
		SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE, *theme);
		SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS, *theme);
		SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS, *theme);

		text_ctrl->SetEdgeColour(LOOKUP_COLOR(*theme, edgeColor).value_or(ride::BLACK));

		SetIndicator(text_ctrl, ID_INDICATOR_ERROR, theme->indicator_error, set.indicator_error, *theme);
		SetIndicator(text_ctrl, ID_INDICATOR_WARNING, theme->indicator_warning, set.indicator_warning, *theme);
		SetIndicator(text_ctrl, ID_INDICATOR_SEARCH_HIGHLIGHT, theme->indicator_search_highlight, set.indicator_search_highlight, *theme);
		SetIndicator(text_ctrl, ID_INDICATOR_SELECT_HIGHLIGHT, theme->indicator_select_highlight, set.indicator_select_highlight, *theme);

		if (theme->selection_foreground)
		{
			text_ctrl->SetSelForeground(true, LOOKUP_COLOR_R(*theme, selection_foreground).value_or(ride::BLACK));
		}

		if (theme->selection_background)
		{
			text_ctrl->SetSelBackground(true, LOOKUP_COLOR_R(*theme, selection_background).value_or(ride::WHITE));
		}

		SetupScintillaDefaultStyles(text_ctrl, *theme);
		SetupScintillaCurrentLine(text_ctrl, set, *theme);
	}

	// set spaces and indention
	const int tabwidth = project ? project->tabwidth() : set.tabWidth;
	text_ctrl->SetTabWidth(tabwidth);
	text_ctrl->SetUseTabs(project ? project->usetabs() : set.useTabs);
	text_ctrl->SetTabIndents(set.tabIndents);
	text_ctrl->SetBackSpaceUnIndents(set.backspaceUnindents);
	text_ctrl->SetIndent(tabwidth);

	text_ctrl->SetViewEOL(set.displayEOL);
	text_ctrl->SetIndentationGuides(set.indentGuideEnable);
	text_ctrl->SetEdgeMode(C(set.edgeStyle));
	text_ctrl->SetEdgeColumn(set.edgeColumn);
	text_ctrl->SetViewWhiteSpace(C(set.whitespace));
	text_ctrl->SetOvertype(false);
	text_ctrl->SetReadOnly(false);
	text_ctrl->SetWrapMode(C(set.wordWrap));

	// set visibility
	// todo: investigate this
	text_ctrl->SetVisiblePolicy(wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1);
	text_ctrl->SetXCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);
	text_ctrl->SetYCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);

	text_ctrl->SetAdditionalSelectionTyping(true);
	text_ctrl->SetAdditionalCaretsBlink(true);
	text_ctrl->SetAdditionalCaretsVisible(true);

	text_ctrl->SetEndAtLastLine(set.end_at_last_line);
	text_ctrl->SetVirtualSpaceOptions(C(set.virtual_space));
	text_ctrl->SetUseVerticalScrollBar(set.vertical_scrollbar);
	text_ctrl->SetUseHorizontalScrollBar(set.horizontal_scrollbar);

	text_ctrl->SetWrapVisualFlags(C(set.wrap_visual_flags));
	text_ctrl->SetWrapVisualFlagsLocation(C(set.wrap_visual_flags_location));
	text_ctrl->SetWrapIndentMode(C(set.wrap_indent_mode));
	text_ctrl->SetWrapStartIndent(set.wrap_start_indent);

	text_ctrl->AnnotationSetVisible(C(set.annotations));
}
