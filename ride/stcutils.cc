#include "ride/stcutils.h"

#include "ride/wx.h"
#include <wx/stc/stc.h>
#include "ride/language.h"
#include "settings.pb.h"
#include "project.h"

enum { FOLDING_WIDTH = 16 };

int C(ride::EdgeStyle e) {
  switch (e) {
    case ride::EDGESTYLE_BACKGROUND:
      return wxSTC_EDGE_BACKGROUND;
    case ride::EDGESTYLE_LINE:
      return wxSTC_EDGE_LINE;
    case ride::EDGESTYLE_NONE:
      return wxSTC_EDGE_NONE;
    default:
      assert(false && "Invalid edge style");
      return wxSTC_EDGE_NONE;
  }
}

int C(ride::ViewWhitespace e) {
  switch (e) {
    case ride::VIEWWHITESPACE_ALWAYS:
      return wxSTC_WS_VISIBLEALWAYS;
    case ride::VIEWWHITESPACE_AFTER_IDENT:
      return wxSTC_WS_VISIBLEAFTERINDENT;
    case ride::VIEWWHITESPACE_HIDDEN:
      return wxSTC_WS_INVISIBLE;
    default:
      assert(false && "Invalid whitespace style");
      return wxSTC_WS_INVISIBLE;
  }
}

int C(ride::WrapMode e) {
  switch (e) {
    case ride::WRAPMODE_CHAR:
      return wxSTC_WRAP_CHAR;
    case ride::WRAPMODE_WORD:
      return wxSTC_WRAP_WORD;
    case ride::WRAPMODE_NONE:
      return wxSTC_WRAP_NONE;
    default:
      assert(false && "Invalid wrap mode");
      return wxSTC_WRAP_NONE;
  }
}

int C(ride::FoldFlags f) {
  int ret = 0;

  if (f.linebefore_expanded()) {
    ret |= wxSTC_FOLDFLAG_LINEBEFORE_EXPANDED;
  }
  if (f.linebefore_contracted()) {
    ret |= wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED;
  }
  if (f.lineafter_expanded()) {
    ret |= wxSTC_FOLDFLAG_LINEAFTER_EXPANDED;
  }
  if (f.lineafter_contracted()) {
    ret |= wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED;
  }
  if (f.levelnumbers()) {
    ret |= wxSTC_FOLDFLAG_LEVELNUMBERS;
  }

  return ret;
}

void SetStyle(wxStyledTextCtrl* text, int id, const ride::Style& style) {
  if (style.use_typeface()) {
    text->StyleSetFaceName(id, style.typeface());
  }

  if (style.use_bold()) {
    // optional bool bold = 4;
    text->StyleSetBold(id, style.bold());
  }

  if (style.use_italic()) {
    // optional bool italic = 6;
    text->StyleSetItalic(id, style.italic());
  }

  if (style.use_underline()) {
    // optional bool underline = 8;
    text->StyleSetUnderline(id, style.underline());
  }

  if (style.use_font_size()) {
    // optional int32 font_size = 10;
    text->StyleSetSize(id, style.font_size());
  }

  if (style.use_foreground()) {
    text->StyleSetForeground(id, C(style.foreground()));
  }
  if (style.use_background()) {
    text->StyleSetBackground(id, C(style.background()));
  }
}

int C(const ride::IndicatorStyle style) {
  switch (style) {
    case ride::INDICATORSTYLE_PLAIN:
      return wxSTC_INDIC_PLAIN;
    case ride::INDICATORSTYLE_SQUIGGLE:
      return wxSTC_INDIC_SQUIGGLE;
    case ride::INDICATORSTYLE_TT:
      return wxSTC_INDIC_TT;
    case ride::INDICATORSTYLE_DIAGONAL:
      return wxSTC_INDIC_DIAGONAL;
    case ride::INDICATORSTYLE_STRIKE:
      return wxSTC_INDIC_STRIKE;
    case ride::INDICATORSTYLE_HIDDEN:
      return wxSTC_INDIC_HIDDEN;
    case ride::INDICATORSTYLE_BOX:
      return wxSTC_INDIC_BOX;
    case ride::INDICATORSTYLE_ROUNDBOX:
      return wxSTC_INDIC_ROUNDBOX;
    case ride::INDICATORSTYLE_STRAIGHTBOX:
      return wxSTC_INDIC_STRAIGHTBOX;
    case ride::INDICATORSTYLE_DASH:
      return wxSTC_INDIC_DASH;
    case ride::INDICATORSTYLE_DOTS:
      return wxSTC_INDIC_DOTS;
    case ride::INDICATORSTYLE_SQUIGGLELOW:
      return wxSTC_INDIC_SQUIGGLELOW;
    case ride::INDICATORSTYLE_DOTBOX:
      return wxSTC_INDIC_DOTBOX;
    default:
      assert(0 && "Unknown indicator style!");
      return wxSTC_INDIC_HIDDEN;
  }
}

int C(const ride::VirtualSpace space) {
  switch (space) {
    case ride::VIRTUALSPACE_NONE:
      return wxSTC_SCVS_NONE;
    case ride::VIRTUALSPACE_RECTANGULARSELECTION:
      return wxSTC_SCVS_RECTANGULARSELECTION;
    case ride::VIRTUALSPACE_USERACCESSIBLE:
      return wxSTC_SCVS_USERACCESSIBLE;
    default:
      assert(0 && "Unknown virtual space");
      return wxSTC_SCVS_NONE;
  }
}

int C(ride::WrapVisualFlagsLocation loc) {
  switch (loc) {
    case ride::WRAPVISUALFLAGLOC_DEFAULT:
      return wxSTC_WRAPVISUALFLAGLOC_DEFAULT;
    case ride::WRAPVISUALFLAGLOC_END_BY_TEXT:
      return wxSTC_WRAPVISUALFLAGLOC_END_BY_TEXT;
    case ride::WRAPVISUALFLAGLOC_START_BY_TEXT:
      return wxSTC_WRAPVISUALFLAGLOC_START_BY_TEXT;
    default:
      assert(0 && "Unknown visual flag location");
      return wxSTC_WRAPVISUALFLAGLOC_DEFAULT;
  }
}

int C(ride::WrapIndentMode mode) {
  switch (mode) {
    case ride::WRAPINDENT_FIXED:
      return wxSTC_WRAPINDENT_FIXED;
    case ride::WRAPINDENT_SAME:
      return wxSTC_WRAPINDENT_SAME;
    case ride::WRAPINDENT_INDENT:
      return wxSTC_WRAPINDENT_INDENT;
    default:
      assert(0 && "Unknown wrap indent mode");
      return wxSTC_WRAPINDENT_FIXED;
  }
}

int C(ride::WrapVisualFlags flags) {
  int ret = wxSTC_WRAPVISUALFLAG_NONE;
  if (flags.end()) {
    ret |= wxSTC_WRAPVISUALFLAG_END;
  }
  if (flags.start()) {
    ret |= wxSTC_WRAPVISUALFLAG_START;
  }
  if (flags.margin()) {
    ret |= wxSTC_WRAPVISUALFLAG_MARGIN;
  }
  return ret;
};

int C(ride::Annotation ann) {
  switch (ann) {
    case ride::ANNOTATION_HIDDEN:
      return wxSTC_ANNOTATION_HIDDEN;
    case ride::ANNOTATION_STANDARD:
      return wxSTC_ANNOTATION_STANDARD;
    case ride::ANNOTATION_BOXED:
      return wxSTC_ANNOTATION_BOXED;
    default:
      assert(0 && "Unknown annotation style");
      return wxSTC_ANNOTATION_STANDARD;
  }
}

void SetIndicator(wxStyledTextCtrl* text, int index,
                  const ride::Indicator& indicator,
                  const ride::IndicatorStyle indicator_style) {
  text->IndicatorSetUnder(index, indicator.under());
  text->IndicatorSetAlpha(index, indicator.alpha());
  text->IndicatorSetOutlineAlpha(index, indicator.outline_alpha());
  text->IndicatorSetForeground(index, C(indicator.foreground()));
  text->IndicatorSetStyle(index, C(indicator_style));
}

void SetupScintillaCurrentLine(wxStyledTextCtrl* text_ctrl,
                               const ride::Settings& set) {
  text_ctrl->SetCaretLineBackground(C(set.fonts_and_colors().selected_line()));
  if (set.current_line_overdraw()) {
    text_ctrl->SetCaretLineBackAlpha(set.current_line_alpha());
  } else {
    text_ctrl->SetCaretLineBackAlpha(wxSTC_ALPHA_NOALPHA);
  }
  text_ctrl->SetCaretLineVisible(set.current_line_visible());
  // todo: set SCI_SETCARETLINEVISIBLEALWAYS to true, this will make it easier
  // to change settings
}

void SetupScintillaDefaultStyles(wxStyledTextCtrl* text_ctrl,
                                 const ride::Settings& set) {
  SetStyle(text_ctrl, wxSTC_STYLE_DEFAULT,
           set.fonts_and_colors().default_style());
  SetStyle(text_ctrl, wxSTC_STYLE_LINENUMBER,
           set.fonts_and_colors().line_number_style());
  SetStyle(text_ctrl, wxSTC_STYLE_BRACELIGHT,
           set.fonts_and_colors().bracelight_style());
  SetStyle(text_ctrl, wxSTC_STYLE_BRACEBAD,
           set.fonts_and_colors().bracebad_style());
  SetStyle(text_ctrl, wxSTC_STYLE_CONTROLCHAR,
           set.fonts_and_colors().controlchar_style());
  SetStyle(text_ctrl, wxSTC_STYLE_INDENTGUIDE,
           set.fonts_and_colors().indentguide_style());
  SetStyle(text_ctrl, wxSTC_STYLE_CALLTIP,
           set.fonts_and_colors().calltip_style());

  SetStyle(text_ctrl, STYLE_ANNOTATION_ERROR,
           set.fonts_and_colors().annotation_error_style());
  SetStyle(text_ctrl, STYLE_ANNOTATION_WARNING,
           set.fonts_and_colors().annotation_warning_style());
}

int C(ride::MarkerSymbol sym) {
  switch (sym) {
    case ride::MARKERSYMBOL_CIRCLE:
      return wxSTC_MARK_CIRCLE;
    case ride::MARKERSYMBOL_ROUNDRECT:
      return wxSTC_MARK_ROUNDRECT;
    case ride::MARKERSYMBOL_ARROW:
      return wxSTC_MARK_ARROW;
    case ride::MARKERSYMBOL_SMALLRECT:
      return wxSTC_MARK_SMALLRECT;
    case ride::MARKERSYMBOL_SHORTARROW:
      return wxSTC_MARK_SHORTARROW;
    case ride::MARKERSYMBOL_EMPTY:
      return wxSTC_MARK_EMPTY;
    case ride::MARKERSYMBOL_ARROWDOWN:
      return wxSTC_MARK_ARROWDOWN;
    case ride::MARKERSYMBOL_MINUS:
      return wxSTC_MARK_MINUS;
    case ride::MARKERSYMBOL_PLUS:
      return wxSTC_MARK_PLUS;
    default:
      assert(false && "Invalid marker symbol");
      return wxSTC_MARK_EMPTY;
  }
}

void SetMarker(wxStyledTextCtrl* text_ctrl, int number,
               ride::MarkerSymbol mark_symbol, ride::Color foreground,
               ride::Color background) {
  text_ctrl->MarkerDefine(number, C(mark_symbol), C(foreground), C(background));
}

void SetupLineMargin(wxStyledTextCtrl* text_ctrl, const ride::Settings& set) {
  // calculate the maximum number a the line margin could contain
  const int number_of_lines = text_ctrl->GetNumberOfLines();

  const size_t length = wxString::Format("%d", number_of_lines).Length();
  const wxString maximum_line_numbers = wxString(length, '9');

  int line_margin_width_ = text_ctrl->TextWidth(
      wxSTC_STYLE_LINENUMBER, wxString::Format("_%s", maximum_line_numbers));
  text_ctrl->SetMarginWidth(ID_MARGIN_LINENUMBER,
                            set.linenumberenable() ? line_margin_width_ : 0);
}

void SetupScintilla(wxStyledTextCtrl* text_ctrl, const ride::Settings& set,
                    Language* language, Project* project) {
  const ride::FontsAndColors& cols = set.fonts_and_colors();
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

  text_ctrl->SetFoldMarginColour(true,
                                 C(set.fonts_and_colors().fold_margin_low()));
  text_ctrl->SetFoldMarginHiColour(true,
                                   C(set.fonts_and_colors().fold_margin_hi()));

  text_ctrl->SetMarginWidth(ID_MARGIN_FOLDING,
                            set.foldenable() ? FOLDING_WIDTH : 0);
  text_ctrl->SetMarginSensitive(ID_MARGIN_FOLDING, set.foldenable());
  text_ctrl->SetFoldFlags(C(set.foldflags()));

#define FRONT_AND_BACK(x) x##_foreground(), x##_background()
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDEREND, set.folderend(),
            FRONT_AND_BACK(set.fonts_and_colors().folderend));
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDEROPENMID, set.folderopenmid(),
            FRONT_AND_BACK(set.fonts_and_colors().folderopenmid));
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDERMIDTAIL, set.foldermidtail(),
            FRONT_AND_BACK(set.fonts_and_colors().foldermidtail));
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDERTAIL, set.foldertail(),
            FRONT_AND_BACK(set.fonts_and_colors().foldertail));
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDERSUB, set.foldersub(),
            FRONT_AND_BACK(set.fonts_and_colors().foldersub));
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDER, set.folder(),
            FRONT_AND_BACK(set.fonts_and_colors().folder));
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDEROPEN, set.folderopen(),
            FRONT_AND_BACK(set.fonts_and_colors().folderopen));
#undef FRONT_AND_BACK

  // set spaces and indention
  text_ctrl->SetTabWidth(project ? project->tabwidth() : set.tabwidth());
  text_ctrl->SetUseTabs(project ? project->usetabs() : set.usetabs());
  text_ctrl->SetTabIndents(set.tabindents());
  text_ctrl->SetBackSpaceUnIndents(set.backspaceunindents());
  text_ctrl->SetIndent(set.tabwidth());

  text_ctrl->SetViewEOL(set.displayeolenable());
  text_ctrl->SetIndentationGuides(set.indentguideenable());
  text_ctrl->SetEdgeMode(C(set.edgestyle()));
  text_ctrl->SetEdgeColour(C(set.fonts_and_colors().edgecolor()));
  text_ctrl->SetEdgeColumn(set.edgecolumn());
  text_ctrl->SetViewWhiteSpace(C(set.whitespace()));
  text_ctrl->SetOvertype(false);
  text_ctrl->SetReadOnly(false);
  text_ctrl->SetWrapMode(C(set.wordwrap()));

  // set visibility
  // todo: investigate this
  text_ctrl->SetVisiblePolicy(wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1);
  text_ctrl->SetXCaretPolicy(
      wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);
  text_ctrl->SetYCaretPolicy(
      wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);

  text_ctrl->SetAdditionalSelectionTyping(true);
  text_ctrl->SetAdditionalCaretsBlink(true);
  text_ctrl->SetAdditionalCaretsVisible(true);

  SetIndicator(text_ctrl, ID_INDICATOR_ERROR,
               set.fonts_and_colors().indicator_error(), set.indicator_error());
  SetIndicator(text_ctrl, ID_INDICATOR_WARNING,
               set.fonts_and_colors().indicator_warning(),
               set.indicator_warning());
  SetIndicator(text_ctrl, ID_INDICATOR_SEARCH_HIGHLIGHT,
               set.fonts_and_colors().indicator_search_highlight(),
               set.indicator_search_highlight());
  SetIndicator(text_ctrl, ID_INDICATOR_SELECT_HIGHLIGHT,
               set.fonts_and_colors().indicator_select_highlight(),
               set.indicator_select_highlight());

  SetupScintillaDefaultStyles(text_ctrl, set);

  text_ctrl->SetEndAtLastLine(set.end_at_last_line());
  text_ctrl->SetVirtualSpaceOptions(C(set.virtual_space()));
  text_ctrl->SetUseVerticalScrollBar(set.vertical_scrollbar());
  text_ctrl->SetUseHorizontalScrollBar(set.horizontal_scrollbar());

  text_ctrl->SetWrapVisualFlags(C(set.wrap_visual_flags()));
  text_ctrl->SetWrapVisualFlagsLocation(C(set.wrap_visual_flags_location()));
  text_ctrl->SetWrapIndentMode(C(set.wrap_indent_mode()));
  text_ctrl->SetWrapStartIndent(set.wrap_start_indent());

  text_ctrl->AnnotationSetVisible(C(set.annotations()));

  if (cols.use_selection_foreground()) {
    text_ctrl->SetSelForeground(true, C(cols.selection_foreground()));
  }

  if (cols.use_selection_background()) {
    text_ctrl->SetSelBackground(true, C(cols.selection_background()));
  }

  SetupScintillaCurrentLine(text_ctrl, set);
}
