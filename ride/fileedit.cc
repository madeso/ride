#include "ride/wx.h"
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include "ride/compilermessage.h"

#include <vector>
#include <cassert>

#include "ride/mainwindow.h"
#include "ride/fileedit.h"
#include <wx/numdlg.h> 

enum
{
  ID_MARGIN_LINENUMBER = 0,
  ID_MARGIN_DIVIDER = 1,
  ID_MARGIN_FOLDING = 2
};

const int FOLDING_WIDTH = 16;

enum
{
  ID_INDICATOR_ERROR,
  ID_INDICATOR_WARNING,
  ID_INDICATOR_SEARCH_HIGHLIGHT,
  ID_INDICATOR_SELECT_HIGHLIGHT
};

enum {
  STYLE_LASTPREDEF = wxSTC_STYLE_LASTPREDEFINED,
  STYLE_ANNOTATION_WARNING,
  STYLE_ANNOTATION_ERROR
};

//////////////////////////////////////////////////////////////////////////

void FileEdit::Undo() {
  if (!text_->CanUndo()) return;
  text_->Undo();
  UpdateTitle();
}


void FileEdit::Redo() {
  if (!text_->CanRedo()) return;
  text_->Redo();
  UpdateTitle();
}


void FileEdit::Cut() {
  if (text_->GetReadOnly() || (text_->GetSelectionEnd() - text_->GetSelectionStart() <= 0)) return;
  text_->Cut();
  UpdateTitle();
}


void FileEdit::Copy() {
  if (text_->GetSelectionEnd() - text_->GetSelectionStart() <= 0) return;
  text_->Copy();
}


void FileEdit::Paste() {
  if (!text_->CanPaste()) return;
  text_->Paste();
  UpdateTitle();
}


void FileEdit::Duplicate() {
  text_->SelectionDuplicate();
  UpdateTitle();
}


void FileEdit::Delete() {
  if (text_->GetReadOnly()) return;
  text_->Clear();
  UpdateTitle();
}


void FileEdit::Find() {
  // todo: implement file find
}


void FileEdit::Replace() {
  // todo: implement file replace
}


void FileEdit::MatchBrace() {
  int start_brace = text_->GetCurrentPos();
  int other_brace = text_->BraceMatch(start_brace);
  if (other_brace == -1) return;

  text_->SetSelection(other_brace, other_brace);
}


void FileEdit::SelectBrace() {
  int start_brace = text_->GetCurrentPos();
  int other_brace = text_->BraceMatch(start_brace);
  if (other_brace == -1) return;

  if (other_brace < start_brace) {
    std::swap(start_brace, other_brace);
  }

  assert(start_brace < other_brace);
  text_->SetSelection(start_brace, other_brace + 1);
}


void FileEdit::GotoLine() {
  const int total_plus_one = text_->GetLineCount()+1;
  const wxString message = wxString::Format("Enter line number(1-%d)", total_plus_one);
  const long new_line_one_based = wxGetNumberFromUser(message, wxEmptyString, "Goto line", text_->GetCurrentLine()+1, 1, total_plus_one);
  if (new_line_one_based == -1) return;
  const int pos = text_->PositionFromLine(new_line_one_based-1);
  text_->SetSelection(pos, pos);
  text_->ScrollToLine(new_line_one_based - 1);
}


void FileEdit::Indent() {
  text_->CmdKeyExecute(wxSTC_CMD_TAB);
  UpdateTitle();
}


void FileEdit::UnIndent() {
  text_->CmdKeyExecute(wxSTC_CMD_BACKTAB);
  UpdateTitle();
}


void FileEdit::SelectAll() {
  text_->SetSelection(0, text_->GetTextLength());
}


void FileEdit::SelectLine() {
  int line_start = text_->PositionFromLine(text_->GetCurrentLine());
  int line_end = text_->PositionFromLine(text_->GetCurrentLine() + 1);
  text_->SetSelection(line_start, line_end);
}

void FileEdit::ToUpper() {
  text_->UpperCase();
  UpdateTitle();
}

void FileEdit::ToLower() {
  text_->LowerCase();
  UpdateTitle();
}

void FileEdit::MoveLinesUp() {
  text_->MoveSelectedLinesUp();
  UpdateTitle();
}

void FileEdit::MoveLinesDown() {
  text_->MoveSelectedLinesDown();
  UpdateTitle();
}

void FileEdit::ShowProperties() {
  // todo: implement file properties
}

//////////////////////////////////////////////////////////////////////////

const wxString& FileEdit::filename() const {
  return filename_;
}

int FromLineColToTextOffset(wxStyledTextCtrl* text, int line, int col) {
  if (line == -1) {
    return -1;
  }

  int from = text->PositionFromLine(line - 1);
  if (col > 0) {
    from += col - 1;
  }
  return from;
}

void FileEdit::SetSelection(int start_line, int start_index, int end_line, int end_index) {
  int from = FromLineColToTextOffset(text_, start_line, start_index);
  int to = FromLineColToTextOffset(text_, end_line, end_index);

  if (from == -1) return;

  if (to != -1) {
    text_->SetSelection(from, to);
  }
  else {
    text_->SetSelection(from, from);
  }

  text_->ScrollToLine(start_line - 1);
  if (start_index >= 0) {
    text_->ScrollToColumn(start_index - 1);
  }
}

void FileEdit::Focus() {
  SetFocus();
  text_->SetFocus();
}

void FileEdit::ClearCompilerMessages() {
  text_->AnnotationClearAll();
  text_->SetIndicatorCurrent(ID_INDICATOR_WARNING);
  text_->IndicatorFillRange(0, text_->GetLength());

  text_->SetIndicatorCurrent(ID_INDICATOR_ERROR);
  text_->IndicatorFillRange(0, text_->GetLength());
}

void FileEdit::AddCompilerMessage(const CompilerMessage& mess) {
  assert(filename_ == mess.file());
  const bool is_error = mess.type() == CompilerMessage::TYPE_ERROR;
  const bool is_warning = mess.type() == CompilerMessage::TYPE_WARNING;

  if (is_error || is_warning) {
    // todo: make embedded warning and error annoatations a option
    const int style = is_error ? STYLE_ANNOTATION_ERROR : STYLE_ANNOTATION_WARNING;
    const int line = mess.start_line() -1;
    const wxString type = is_error ? "Error: " : "Warning: ";
    const wxString value = type + mess.message();
    const wxString old_text = text_->AnnotationGetText(line); // todo: setup multicoloring/styling
    const wxString ann = old_text.IsEmpty() ? value : old_text + "\n" + value;
    text_->AnnotationSetText(line, ann);
    text_->AnnotationSetStyle(line, style);

    // only color on a single row, or it might get ugly
    // todo: add option to indicate warning/errors on multiple rows or not
    if (mess.start_line() == mess.end_line()) {
      int from = FromLineColToTextOffset(text_, mess.start_line(), mess.start_index());
      int to = FromLineColToTextOffset(text_, mess.end_line(), mess.end_index());

      const int ind = is_error ? ID_INDICATOR_ERROR : ID_INDICATOR_WARNING;

      if (from >= 0 && to >= 0) {
        text_->SetIndicatorCurrent(ind);
        text_->IndicatorFillRange(from, to - from);
      }
    }
  }
}

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

FileEdit::FileEdit(wxAuiNotebook* anotebook, MainWindow* parent, const wxString& source, const wxString& file)
  : wxControl(parent, wxID_ANY)
  , main_(parent)
  , notebook_(anotebook)
  , current_language_(NULL) 
  , highligh_current_word_last_start_position_(-1)
  , highligh_current_word_last_end_position_(-1)
{
  text_ = new wxStyledTextCtrl(this,  wxID_ANY, wxDefaultPosition, wxDefaultSize,
#ifndef __WXMAC__
    wxSUNKEN_BORDER |
#endif
    wxVSCROLL);

  filename_ = file;

  if (filename_.IsEmpty()) {
    text_->SetText(source);
  }
  else {
    text_->LoadFile(filename_);
  }

  // calculate the maximum number a the line margin could contain
  line_margin_width_ = text_->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_999999"));

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(text_, 1, wxEXPAND);
  SetSizer(sizer);

  notebook_->AddPage(this, wxT(""), true);
  UpdateFilename();
  UpdateTitle();
}

bool FileEdit::Save() {
  if (ShouldBeSaved() == false) return true;
  if (filename_.IsEmpty()) return SaveAs();
  else return SaveTo(filename_);
}

bool FileEdit::SaveAs() {
  wxFileDialog saveFileDialog(this, _("Save file"), "", "",
    GetFilePattern(), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return false;
  return SaveTo(saveFileDialog.GetPath());
}

bool FileEdit::SaveTo(const wxString& target) {
  if (false == text_->SaveFile(target)) {
    return false;
  }
  filename_ = target;
  UpdateFilename();
  UpdateTitle();
  return true;
}

void SetStyle(wxStyledTextCtrl* text, int id, const ride::Style& style) {
  if (style.use_typeface()) {
    text->StyleSetFaceName(id, style.typeface());
  }

  if (style.use_bold()) {
    //optional bool bold = 4;
    text->StyleSetBold(id, style.bold());
  }

  if (style.use_italic()) {
    //optional bool italic = 6;
    text->StyleSetItalic(id, style.italic());
  }

  if (style.use_underline()) {
    //optional bool underline = 8;
    text->StyleSetUnderline(id, style.underline());
  }

  if (style.use_font_size()) {
    //optional int32 font_size = 10;
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
  switch (style)
  {
    case ride::INDICATORSTYLE_PLAIN      : return wxSTC_INDIC_PLAIN       ;
    case ride::INDICATORSTYLE_SQUIGGLE   : return wxSTC_INDIC_SQUIGGLE    ;
    case ride::INDICATORSTYLE_TT         : return wxSTC_INDIC_TT          ;
    case ride::INDICATORSTYLE_DIAGONAL   : return wxSTC_INDIC_DIAGONAL    ;
    case ride::INDICATORSTYLE_STRIKE     : return wxSTC_INDIC_STRIKE      ;
    case ride::INDICATORSTYLE_HIDDEN     : return wxSTC_INDIC_HIDDEN      ;
    case ride::INDICATORSTYLE_BOX        : return wxSTC_INDIC_BOX         ;
    case ride::INDICATORSTYLE_ROUNDBOX   : return wxSTC_INDIC_ROUNDBOX    ;
    case ride::INDICATORSTYLE_STRAIGHTBOX: return wxSTC_INDIC_STRAIGHTBOX ;
    case ride::INDICATORSTYLE_DASH       : return wxSTC_INDIC_DASH        ;
    case ride::INDICATORSTYLE_DOTS       : return wxSTC_INDIC_DOTS        ;
    case ride::INDICATORSTYLE_SQUIGGLELOW: return wxSTC_INDIC_SQUIGGLELOW ;
    case ride::INDICATORSTYLE_DOTBOX     : return wxSTC_INDIC_DOTBOX      ;
  default:
    assert(0 && "Unknown indicator style!");
    return wxSTC_INDIC_HIDDEN;
  }
}

int C(const ride::VirtualSpace space) {
  switch (space) {
  case ride::VIRTUALSPACE_NONE: return wxSTC_SCVS_NONE;
  case ride::VIRTUALSPACE_RECTANGULARSELECTION: return wxSTC_SCVS_RECTANGULARSELECTION;
  case ride::VIRTUALSPACE_USERACCESSIBLE: return wxSTC_SCVS_USERACCESSIBLE;
  default:
    assert(0 && "Unknown virtual space");
    return wxSTC_SCVS_NONE;
  }
}

int C(ride::WrapVisualFlagsLocation loc) {
  switch (loc) {
    case ride::WRAPVISUALFLAGLOC_DEFAULT        : return wxSTC_WRAPVISUALFLAGLOC_DEFAULT      ;
    case ride::WRAPVISUALFLAGLOC_END_BY_TEXT    : return wxSTC_WRAPVISUALFLAGLOC_END_BY_TEXT  ;
    case ride::WRAPVISUALFLAGLOC_START_BY_TEXT  : return wxSTC_WRAPVISUALFLAGLOC_START_BY_TEXT;
    default:
      assert(0 && "Unknown visual flag location");
      return wxSTC_WRAPVISUALFLAGLOC_DEFAULT;
  }
}

int C(ride::WrapIndentMode mode) {
  switch (mode) {
   case ride::WRAPINDENT_FIXED : return wxSTC_WRAPINDENT_FIXED ;
   case ride::WRAPINDENT_SAME  : return wxSTC_WRAPINDENT_SAME  ;
   case ride::WRAPINDENT_INDENT: return wxSTC_WRAPINDENT_INDENT;
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
    case ride::ANNOTATION_HIDDEN      : return wxSTC_ANNOTATION_HIDDEN  ;
    case ride::ANNOTATION_STANDARD    : return wxSTC_ANNOTATION_STANDARD;
    case ride::ANNOTATION_BOXED       : return wxSTC_ANNOTATION_BOXED   ;
    default:
      assert(0 && "Unknown annotation style");
      return wxSTC_ANNOTATION_STANDARD;
  }
}

void SetIndicator(wxStyledTextCtrl* text, int index, const ride::Indicator& indicator) {
  text->IndicatorSetUnder(index, indicator.under());
  text->IndicatorSetAlpha(index, indicator.alpha());
  text->IndicatorSetOutlineAlpha(index, indicator.outline_alpha());
  text->IndicatorSetForeground(index, C(indicator.foreground()));
  text->IndicatorSetStyle(index, C(indicator.style()));
}

wxString b2s01(bool b) {
  if (b) return _("1");
  else return _("0");
}

void FileEdit::UpdateTextControl() {
  // initialize styles
  text_->StyleClearAll();

  // todo: remove these variables when all options are read from settings
  wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));
  const wxColor grey(100, 100, 100);
  const wxColor white(255, 255, 255);
  const wxColor darkgray = wxColour(20, 20, 20);

  const ride::Settings& set = main_->settings();

  // setup language color
  assert(current_language_);
  current_language_->StyleDocument(text_, set);

  //////////////////////////////////////////////////////////////////////////

  // set margin for line numbers
  text_->SetMarginType(ID_MARGIN_LINENUMBER, wxSTC_MARGIN_NUMBER);
  text_->SetMarginWidth(ID_MARGIN_LINENUMBER, set.linenumberenable() ? line_margin_width_ : 0);

  // set margin as unused
  text_->SetMarginType(ID_MARGIN_DIVIDER, wxSTC_MARGIN_SYMBOL);
  text_->SetMarginWidth(ID_MARGIN_DIVIDER, 15);
  text_->SetMarginSensitive(ID_MARGIN_DIVIDER, false);

  // folding settings
  text_->SetMarginType(ID_MARGIN_FOLDING, wxSTC_MARGIN_SYMBOL);
  text_->SetMarginMask(ID_MARGIN_FOLDING, wxSTC_MASK_FOLDERS);
  text_->SetMarginWidth(ID_MARGIN_FOLDING, 15);
  text_->SetMarginSensitive(ID_MARGIN_FOLDING, true);

  text_->SetFoldMarginColour(true, C(set.fonts_and_colors().fold_margin_low()));
  text_->SetFoldMarginHiColour(true, C(set.fonts_and_colors().fold_margin_hi()));

  text_->SetMarginWidth(ID_MARGIN_FOLDING, set.foldenable() ? FOLDING_WIDTH : 0);
  text_->SetMarginSensitive(ID_MARGIN_FOLDING, set.foldenable());
  text_->SetFoldFlags(C(set.foldflags()));
  // todo: move folding symbol options to settings
  text_->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW, grey, grey);
  text_->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN, grey, grey);
  text_->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY, grey, grey);
  text_->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW, grey, white);
  text_->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, grey, white);
  text_->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY, grey, grey);
  text_->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY, grey, grey);

  // set spaces and indention
  text_->SetTabWidth(set.tabwidth());
  text_->SetUseTabs(set.usetabs());
  text_->SetTabIndents(set.tabindents());
  text_->SetBackSpaceUnIndents(set.backspaceunindents());
  text_->SetIndent(set.tabwidth());

  text_->SetViewEOL(set.displayeolenable());
  text_->SetIndentationGuides(set.indentguideenable());
  text_->SetEdgeMode(C(set.edgestyle()));
  text_->SetEdgeColour(C(set.fonts_and_colors().edgecolor()));
  text_->SetEdgeColumn(set.edgecolumn());
  text_->SetViewWhiteSpace(C(set.whitespace()));
  text_->SetOvertype(false);
  text_->SetReadOnly(false);
  text_->SetWrapMode(C(set.wordwrap()));

  // set visibility
  // todo: investigate this
  text_->SetVisiblePolicy(wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1);
  text_->SetXCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);
  text_->SetYCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);

  text_->SetAdditionalSelectionTyping(true);
  text_->SetAdditionalCaretsBlink(true);
  text_->SetAdditionalCaretsVisible(true);

  SetIndicator(text_, ID_INDICATOR_ERROR, set.indicator_error());
  SetIndicator(text_, ID_INDICATOR_WARNING, set.indicator_warning());
  SetIndicator(text_, ID_INDICATOR_SEARCH_HIGHLIGHT, set.indicator_search_highlight());
  SetIndicator(text_, ID_INDICATOR_SELECT_HIGHLIGHT, set.indicator_select_highlight());

  SetStyle(text_, wxSTC_STYLE_DEFAULT, set.fonts_and_colors().default_style());
  SetStyle(text_, wxSTC_STYLE_LINENUMBER, set.fonts_and_colors().line_number_style());
  SetStyle(text_, wxSTC_STYLE_BRACELIGHT, set.fonts_and_colors().bracelight_style());
  SetStyle(text_, wxSTC_STYLE_BRACEBAD, set.fonts_and_colors().bracebad_style());
  SetStyle(text_, wxSTC_STYLE_CONTROLCHAR, set.fonts_and_colors().controlchar_style());
  SetStyle(text_, wxSTC_STYLE_INDENTGUIDE, set.fonts_and_colors().indentguide_style());
  SetStyle(text_, wxSTC_STYLE_CALLTIP, set.fonts_and_colors().calltip_style());

  SetStyle(text_, STYLE_ANNOTATION_ERROR, set.fonts_and_colors().annotation_error_style());
  SetStyle(text_, STYLE_ANNOTATION_WARNING, set.fonts_and_colors().annotation_warning_style());


  text_->SetEndAtLastLine(set.end_at_last_line());
  text_->SetVirtualSpaceOptions(C(set.virtual_space()));
  text_->SetUseVerticalScrollBar(set.vertical_scrollbar());
  text_->SetUseHorizontalScrollBar(set.horizontal_scrollbar());
  
  text_->SetWrapVisualFlags(C(set.wrap_visual_flags()));
  text_->SetWrapVisualFlagsLocation(C(set.wrap_visual_flags_location()));
  text_->SetWrapIndentMode(C(set.wrap_indent_mode()));
  text_->SetWrapStartIndent(set.wrap_start_indent());

  text_->AnnotationSetVisible(C(set.annotations()));


  text_->SetCaretLineBackground(C(set.fonts_and_colors().selected_line()));
  if (set.current_line_overdraw()) {
    text_->SetCaretLineBackAlpha(set.current_line_alpha());
  }
  else {
    text_->SetCaretLineBackAlpha(wxSTC_ALPHA_NOALPHA);
  }
  text_->SetCaretLineVisible(set.current_line_visible());
  // todo: set SCI_SETCARETLINEVISIBLEALWAYS to true, this will make it easier to change settings
}

void FileEdit::UpdateFilename() {
  if (filename_.IsEmpty()) {
    docname_ = "Untitled";
  }
  else {
    wxFileName fn(filename_);
    docname_ = fn.GetFullName();
  }

  if (filename_.IsEmpty() == false) {
    size_t index = notebook_->GetPageIndex(this);
    notebook_->SetPageToolTip(index, filename_);

    wxFileName fname(filename_);
    current_language_ = DetermineLanguage(fname.GetFullName());
    UpdateTextControl();
    UpdateTextControl(); // update colors again, doing it twice seems to be needed to apply the colors
  }
}

void FileEdit::UpdateTitle() {
  size_t index = notebook_->GetPageIndex(this);
  const wxString modified_star = text_->IsModified() ? "*" : "";
  notebook_->SetPageText(index, docname_ + modified_star);
}

bool FileEdit::ShouldBeSaved() {
  return text_->IsModified() || filename_.IsEmpty();
}

bool FileEdit::CanClose(bool canAbort) {
  if (ShouldBeSaved()) {
    // todo: refactor this into usable functions
    const int yew_no_flags = canAbort ? (wxYES_NO | wxCANCEL) : wxYES_NO;
    wxMessageDialog dlg(this, _(""), _("Save file?"), yew_no_flags | wxICON_QUESTION);

    const wxMessageDialogBase::ButtonLabel yesButton = _("&Save it");
    const wxMessageDialogBase::ButtonLabel noButton = _("&Discard changes");
    const wxMessageDialogBase::ButtonLabel cancelButton = _("&Abort");

    const bool labelChangeOk = canAbort
      ? dlg.SetYesNoCancelLabels(yesButton, noButton, cancelButton)
      : dlg.SetYesNoLabels(yesButton, noButton)
      ;

    if (labelChangeOk) {
      dlg.SetMessage("\"" + docname_ + "\" has changed since last time...");
    }
    else {
      dlg.SetMessage("\"" + docname_ + "\" has changed since last time, save it?");
    }

    const int answer = dlg.ShowModal();
    if (answer == wxID_YES) {
      return Save();
    }
    else if (answer == wxID_NO) {
      return true;
    }
    else {
      return false;
    }
  }
  return true;
}

FileEdit::~FileEdit() {
  text_->SetClientData(NULL);
}

wxBEGIN_EVENT_TABLE(FileEdit, wxControl)
  EVT_STC_MARGINCLICK(wxID_ANY, FileEdit::OnMarginClick)
  EVT_STC_CHARADDED(wxID_ANY, FileEdit::OnCharAdded)
  EVT_STC_UPDATEUI(wxID_ANY, FileEdit::OnUpdateUi)
  EVT_STC_CHANGE(wxID_ANY, FileEdit::OnChanged)
wxEND_EVENT_TABLE()

void FileEdit::OnMarginClick(wxStyledTextEvent& event)
{
  if (event.GetMargin() == ID_MARGIN_FOLDING)
  {
    int clicked_line = text_->LineFromPosition(event.GetPosition());
    int fold_level = text_->GetFoldLevel(clicked_line);

    if ((fold_level & wxSTC_FOLDLEVELHEADERFLAG) > 0)
    {
      text_->ToggleFold(clicked_line);
    }
  }
}

int CalculateIndentationChange(const wxString& str) {
  // todo: move indent calculation to language...
  int change = 0;
  for (size_t i = 0; i < str.Length(); ++i) {
    const wxUniChar c = str.at(i);
    if (c == '{') {
      change += 1;
    }
    else if (c == '}') {
      // todo: fix decrease indentation bug
      // change -= 1;
    }
  }
  return change;
}

void FileEdit::OnCharAdded(wxStyledTextEvent& event)
{
  UpdateTitle();

  int entered_character = event.GetKey(); // the key seems to be the char that was added


  if (entered_character == '{') {
    // todo: add setting for completing {}
    text_->InsertText(text_->GetCurrentPos(), "}");
  }
  else if (entered_character == '(') {
    // todo: setting for completing ()
    // todo: make completion of () smarter
    text_->InsertText(text_->GetCurrentPos(), ")");
  }
  else if (entered_character == '[') {
    // todo: setting for this completing []
    // todo: make completion of [] smarter
    text_->InsertText(text_->GetCurrentPos(), "]");
  }
  else if (entered_character == '\n' || entered_character == '\r')
  {
    // auto-indenting
    // todo: add settings for autoindenting(none, keep, smart) ...
    // loosely based on http://www.scintilla.org/ScintillaUsage.html and https://groups.google.com/forum/#!topic/scintilla-interest/vTwXwIBswSM
    const int current_line = text_->GetCurrentLine();
    const int line_start = text_->PositionFromLine(text_->GetCurrentLine()-1);
    const int line_end = text_->PositionFromLine(text_->GetCurrentLine());
    const wxString previous_line_content = text_->GetTextRange(line_start, line_end);
    const int indent_change = CalculateIndentationChange(previous_line_content);
    const int indent_change_in_spaces = indent_change * main_->settings().tabwidth();

    const int smart_indent = indent_change_in_spaces;

    const int indentation_in_spaces = current_line > 0
      ? std::max(0, text_->GetLineIndentation(current_line - 1) + smart_indent)
      : smart_indent;

    const int indentation_in_tabs = indentation_in_spaces / main_->settings().tabwidth();

    // if we use tabs, divide the number of character by the char width to get the actual width
    const int indentation_in_chars = main_->settings().usetabs()
      ? indentation_in_tabs
      : indentation_in_spaces;

    // adjust to remove weird spaces from indentation settings
    const int indentation_in_spaces_ajdusted = indentation_in_tabs * main_->settings().tabwidth();

    if (indentation_in_spaces_ajdusted != 0)
    {
      text_->SetLineIndentation(current_line, indentation_in_spaces_ajdusted);
      
      text_->GotoPos(text_->PositionFromLine(current_line) + indentation_in_chars);
    }
  }
}

void FileEdit::HighlightCurrentWord() {
  const int pos = text_->GetCurrentPos();

  // highlight current word
  const bool only_word_characters = true;
  const int start_position = text_->WordStartPosition(pos, only_word_characters);
  const int end_position = text_->WordEndPosition(pos, only_word_characters);

  // todo: don't highlight keywords?
  
  if (start_position != highligh_current_word_last_start_position_ || end_position != highligh_current_word_last_end_position_) {
    text_->SetIndicatorCurrent(ID_INDICATOR_SELECT_HIGHLIGHT);
    if (highligh_current_word_last_start_position_ != -1 && highligh_current_word_last_end_position_ != -1 && highligh_current_word_last_start_position_ != highligh_current_word_last_end_position_) {
      text_->IndicatorClearRange(0, text_->GetLength());
    }

    if (start_position != -1 && end_position != -1 && start_position != end_position) {
      const int length = end_position - start_position;
      assert(length > 0);

      const wxString hover = text_->GetRange(start_position, end_position);

      int search_point = 0;
      while (true) {
        int match_index = text_->FindText(search_point, text_->GetLength(), hover, wxSTC_FIND_WHOLEWORD | wxSTC_FIND_MATCHCASE);
        if (match_index == -1) {
          break;
        }
        text_->IndicatorFillRange(match_index, length);
        search_point = match_index + length;
      }
    }
    highligh_current_word_last_start_position_ = start_position;
    highligh_current_word_last_end_position_ = end_position;
  }
}

void FileEdit::OnUpdateUi(wxStyledTextEvent& event)
{
  const int type = event.GetUpdated();

  if (type & wxSTC_UPDATE_SELECTION) {
    const int pos = text_->GetCurrentPos();

    HighlightCurrentWord();

    // brace highlighting
    int otherBrace = text_->BraceMatch(pos);
    if (otherBrace != -1) {
      text_->BraceHighlight(pos, otherBrace);
    }
    else {
      text_->BraceHighlight(-1, -1);
    }
  }
}

void FileEdit::OnChanged(wxStyledTextEvent& event) {
  HighlightCurrentWord();
}
