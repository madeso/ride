#include "ride/wx.h"
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/uri.h>
#include <wx/numdlg.h> 

#include <vector>
#include <cassert>

#include "ride/compilermessage.h"
#include "ride/filepropertiesdlg.h"
#include "ride/mainwindow.h"
#include "ride/fileedit.h"
#include "ride/wxutils.h"
#include "ride/finddlg.h"

//////////////////////////////////////////////////////////////////////////

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

wxString FileEdit::GetLanguageName() {
  if (current_language_) return current_language_->language_name();
  else return "<none>";
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


void FileEdit::Find(wxStyledTextCtrl* output) {
  // todo: implement file find
  ShowFindDlg(this, text_->GetSelectedText(), filename_, "", output);
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

void FileEdit::OpenInOnlineDocumentation() {
  const bool only_word_characters = true;

  const wxString selected_text = text_->GetSelectedText();

  const int pos = text_->GetCurrentPos();
  const int start_position = text_->WordStartPosition(pos, only_word_characters);
  const int end_position = text_->WordEndPosition(pos, only_word_characters);
  const wxString current_word = text_->GetRange(start_position, end_position);

  const wxString current_selection_or_word = selected_text.Length() == 0 ? current_word : selected_text;
  // todo: html encode the current_selection_or_word since if it's &dog it should be probably be encoded as &amp;dog
  const wxString url_to_open = wxString::Format("http://doc.rust-lang.org/std/?search=%s", current_selection_or_word);
  wxLaunchDefaultBrowser(wxURI(url_to_open).BuildURI());
}

void FileEdit::ShowProperties() {
  FilePropertiesDlg dlg(this, text_);
  dlg.ShowModal();
}

//////////////////////////////////////////////////////////////////////////

const wxString& FileEdit::filename() const {
  return filename_;
}

void FileEdit::FileHasBeenRenamed(const wxString& new_path) {
  filename_ = new_path;
  UpdateFilename();
  UpdateTitle();
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

void FromTextOffsetToLineCol(wxStyledTextCtrl* text, int index, int* line, int* col) {
  if (index == -1) {
    *line = -1;
    *col = -1;
  }
  else {
    *line = text->LineFromPosition(index); 
    int start = text->PositionFromLine(*line - 1);
    *col = index - start+1;
  }
}

void FileEdit::GetSelection(int* start_line, int* start_index, int* end_line, int* end_index) {
  assert(start_line);
  assert(start_index);
  assert(end_line);
  assert(end_index);
  long from = 0;
  long to = 0;
  text_->GetSelection(&from, &to);
  FromTextOffsetToLineCol(text_, from, start_line, start_index);
  FromTextOffsetToLineCol(text_, to, end_line, end_index);
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
  text_->IndicatorClearRange(0, text_->GetLength());

  text_->SetIndicatorCurrent(ID_INDICATOR_ERROR);
  text_->IndicatorClearRange(0, text_->GetLength());
}

void FileEdit::AddCompilerMessage(const CompilerMessage& mess) {
  assert(filename_ == mess.file());
  const bool is_error = mess.type() == CompilerMessage::TYPE_ERROR;
  const bool is_warning = mess.type() == CompilerMessage::TYPE_WARNING;

  if (main_->settings().show_compiler_messages_as_annotations()
    &&  (is_error || is_warning) ) {
    const int style = is_error ? STYLE_ANNOTATION_ERROR : STYLE_ANNOTATION_WARNING;
    const int line = mess.start_line() -1;
    const wxString type = is_error ? "Error: " : "Warning: ";
    const wxString value = type + mess.message();
    const wxString old_text = text_->AnnotationGetText(line); // todo: setup multicoloring/styling
    const wxString ann = old_text.IsEmpty() ? value : old_text + "\n" + value;
    text_->AnnotationSetText(line, ann);
    text_->AnnotationSetStyle(line, style);

    const bool on_single_line = mess.start_line() == mess.end_line();
    if (main_->settings().show_multiline_indicators() || on_single_line) {
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

wxDateTime GetFileDetectionTime(const wxString file) {
  wxFileName file_name(file);
  return file_name.GetModificationTime();
}

FileEdit::FileEdit(wxAuiNotebook* anotebook, MainWindow* parent, const wxString& file)
  : wxControl(parent, wxID_ANY)
  , main_(parent)
  , notebook_(anotebook)
  , current_language_(NULL) 
  , highlight_current_word_last_start_position_(-1)
  , highlight_current_word_last_end_position_(-1)
{
  assert(false == file.IsEmpty());
  text_ = new wxStyledTextCtrl(this,  wxID_ANY, wxDefaultPosition, wxDefaultSize,
#ifndef __WXMAC__
    wxSUNKEN_BORDER |
#endif
    wxVSCROLL);

  filename_ = file;
  LoadFile();

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(text_, 1, wxEXPAND);
  SetSizer(sizer);

  notebook_->AddPage(this, wxT(""), true);
  UpdateFilename();
  UpdateTitle();
}

void FileEdit::LoadFile() {
  text_->LoadFile(filename_);
  UpdateFileTime();
}

void FileEdit::UpdateFileTime() {
  last_modification_time_ = GetFileDetectionTime(filename_);
}

class TrueFalse {
public:
  TrueFalse(bool* capture) : capture_(capture) {
    assert(capture_);
    assert(*capture_ == false); 
    *capture_ = true;
  }
  ~TrueFalse() {
    assert(capture_);
    assert(*capture_ == true);
    *capture_ = false;
  }

  bool* capture_;
};

void FileEdit::ReloadFileIfNeeded() {
  // basic check for infinite activation->question loop
  static bool inside = false;
  if (inside) return;
  TrueFalse inside_capture(&inside);

  const bool exist = wxFileName(filename_).FileExists();
  if (exist == false) {
    if (DialogResult::YES == ShowYesNo(this, "Close file", "Close file", "Keep open", filename_ + " has been removed", filename_ + " has been removed, close it?")) {
      size_t index = notebook_->GetPageIndex(this);
      const bool file_closed = notebook_->DeletePage(index);
      if (false == file_closed) {
        ShowError(this, "Unable to close file", "Error");
      }
    }
    return;
  }

  wxDateTime latest_file_time = GetFileDetectionTime(filename_);
  if (last_modification_time_ != latest_file_time) {
    // ask to reload or not?
    if (ShowYesNo(this, "File modified!", "Reload the file", "Keep my changes",
      wxString::Format("%s\nThis file has been modified by another program.", filename_),
      wxString::Format("%s\nThis file has been modified by another program.\nDo you want to reload it?", filename_)
      ) == DialogResult::YES) {
      LoadFile();
    }
    else {
      // if the user said "keep changes", we will remember that modification and
      // not ask again for this "change" by remembering the modification time.
      UpdateFileTime();
    }
  }
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
  UpdateFileTime();
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

void SetIndicator(wxStyledTextCtrl* text, int index, const ride::Indicator& indicator, const ride::IndicatorStyle indicator_style) {
  text->IndicatorSetUnder(index, indicator.under());
  text->IndicatorSetAlpha(index, indicator.alpha());
  text->IndicatorSetOutlineAlpha(index, indicator.outline_alpha());
  text->IndicatorSetForeground(index, C(indicator.foreground()));
  text->IndicatorSetStyle(index, C(indicator_style));
}

wxString b2s01(bool b) {
  if (b) return _("1");
  else return _("0");
}

wxString FileEdit::CalculateDocumentName() const {
  if (filename_.IsEmpty()) {
    return "Untitled";
  }
  else {
    wxFileName fn(filename_);
    return fn.GetFullName();
  }
}

void SetupScintillaCurrentLine(wxStyledTextCtrl* text_ctrl, const ride::Settings& set) {
  text_ctrl->SetCaretLineBackground(C(set.fonts_and_colors().selected_line()));
  if (set.current_line_overdraw()) {
    text_ctrl->SetCaretLineBackAlpha(set.current_line_alpha());
  }
  else {
    text_ctrl->SetCaretLineBackAlpha(wxSTC_ALPHA_NOALPHA);
  }
  text_ctrl->SetCaretLineVisible(set.current_line_visible());
  // todo: set SCI_SETCARETLINEVISIBLEALWAYS to true, this will make it easier to change settings
}

void SetupScintillaDefaultStyles(wxStyledTextCtrl* text_ctrl, const ride::Settings& set) {
  SetStyle(text_ctrl, wxSTC_STYLE_DEFAULT, set.fonts_and_colors().default_style());
  SetStyle(text_ctrl, wxSTC_STYLE_LINENUMBER, set.fonts_and_colors().line_number_style());
  SetStyle(text_ctrl, wxSTC_STYLE_BRACELIGHT, set.fonts_and_colors().bracelight_style());
  SetStyle(text_ctrl, wxSTC_STYLE_BRACEBAD, set.fonts_and_colors().bracebad_style());
  SetStyle(text_ctrl, wxSTC_STYLE_CONTROLCHAR, set.fonts_and_colors().controlchar_style());
  SetStyle(text_ctrl, wxSTC_STYLE_INDENTGUIDE, set.fonts_and_colors().indentguide_style());
  SetStyle(text_ctrl, wxSTC_STYLE_CALLTIP, set.fonts_and_colors().calltip_style());

  SetStyle(text_ctrl, STYLE_ANNOTATION_ERROR, set.fonts_and_colors().annotation_error_style());
  SetStyle(text_ctrl, STYLE_ANNOTATION_WARNING, set.fonts_and_colors().annotation_warning_style());
}

int C(ride::MarkerSymbol sym) {
  switch (sym) {
      case ride::MARKERSYMBOL_CIRCLE    : return wxSTC_MARK_CIRCLE    ;
      case ride::MARKERSYMBOL_ROUNDRECT : return wxSTC_MARK_ROUNDRECT ;
      case ride::MARKERSYMBOL_ARROW     : return wxSTC_MARK_ARROW     ;
      case ride::MARKERSYMBOL_SMALLRECT : return wxSTC_MARK_SMALLRECT ;
      case ride::MARKERSYMBOL_SHORTARROW: return wxSTC_MARK_SHORTARROW; 
      case ride::MARKERSYMBOL_EMPTY     : return wxSTC_MARK_EMPTY     ;
      case ride::MARKERSYMBOL_ARROWDOWN : return wxSTC_MARK_ARROWDOWN ;
      case ride::MARKERSYMBOL_MINUS     : return wxSTC_MARK_MINUS     ;
      case ride::MARKERSYMBOL_PLUS      : return wxSTC_MARK_PLUS      ;
      default:
        assert(false && "Invalid marker symbol");
        return wxSTC_MARK_EMPTY;
  }
}

void SetMarker(wxStyledTextCtrl* text_ctrl, int number, ride::MarkerSymbol mark_symbol, ride::Color foreground, ride::Color background ) {
  text_ctrl->MarkerDefine(number, C(mark_symbol), C(foreground), C(background));
}

void SetupLineMargin(wxStyledTextCtrl* text_ctrl, const ride::Settings& set) {
  // calculate the maximum number a the line margin could contain
  const int number_of_lines = text_ctrl->GetNumberOfLines();

  const size_t length = wxString::Format("%d", number_of_lines).Length();
  const wxString maximum_line_numbers = wxString(length, '9');

  int line_margin_width_ = text_ctrl->TextWidth(wxSTC_STYLE_LINENUMBER, wxString::Format("_%s", maximum_line_numbers));
  text_ctrl->SetMarginWidth(ID_MARGIN_LINENUMBER, set.linenumberenable() ? line_margin_width_ : 0);
}

void SetupScintilla(wxStyledTextCtrl* text_ctrl, const ride::Settings& set, Language* language) {
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

  text_ctrl->SetFoldMarginColour(true, C(set.fonts_and_colors().fold_margin_low()));
  text_ctrl->SetFoldMarginHiColour(true, C(set.fonts_and_colors().fold_margin_hi()));

  text_ctrl->SetMarginWidth(ID_MARGIN_FOLDING, set.foldenable() ? FOLDING_WIDTH : 0);
  text_ctrl->SetMarginSensitive(ID_MARGIN_FOLDING, set.foldenable());
  text_ctrl->SetFoldFlags(C(set.foldflags()));

#define FRONT_AND_BACK(x) x##_foreground(), x##_background()
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDEREND,     set.folderend    () , FRONT_AND_BACK(set.fonts_and_colors().folderend    )      );
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDEROPENMID, set.folderopenmid() , FRONT_AND_BACK(set.fonts_and_colors().folderopenmid)      );
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDERMIDTAIL, set.foldermidtail() , FRONT_AND_BACK(set.fonts_and_colors().foldermidtail)      );
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDERTAIL,    set.foldertail   () , FRONT_AND_BACK(set.fonts_and_colors().foldertail   )      );
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDERSUB,     set.foldersub    () , FRONT_AND_BACK(set.fonts_and_colors().foldersub    )      );
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDER,        set.folder       () , FRONT_AND_BACK(set.fonts_and_colors().folder       )      );
  SetMarker(text_ctrl, wxSTC_MARKNUM_FOLDEROPEN,    set.folderopen   () , FRONT_AND_BACK(set.fonts_and_colors().folderopen   )      );
#undef FRONT_AND_BACK

  // set spaces and indention
  text_ctrl->SetTabWidth(set.tabwidth());
  text_ctrl->SetUseTabs(set.usetabs());
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
  text_ctrl->SetXCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);
  text_ctrl->SetYCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);

  text_ctrl->SetAdditionalSelectionTyping(true);
  text_ctrl->SetAdditionalCaretsBlink(true);
  text_ctrl->SetAdditionalCaretsVisible(true);

  SetIndicator(text_ctrl, ID_INDICATOR_ERROR,            set.fonts_and_colors().indicator_error()           , set.indicator_error());
  SetIndicator(text_ctrl, ID_INDICATOR_WARNING,          set.fonts_and_colors().indicator_warning()         , set.indicator_warning());
  SetIndicator(text_ctrl, ID_INDICATOR_SEARCH_HIGHLIGHT, set.fonts_and_colors().indicator_search_highlight(), set.indicator_search_highlight());
  SetIndicator(text_ctrl, ID_INDICATOR_SELECT_HIGHLIGHT, set.fonts_and_colors().indicator_select_highlight(), set.indicator_select_highlight());

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

void FileEdit::UpdateTextControl() {
  const ride::Settings& set = main_->settings();
  SetupScintilla(text_, set, current_language_);
}

void FileEdit::UpdateFilename() {
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
  notebook_->SetPageText(index, CalculateDocumentName() + modified_star);
}

bool FileEdit::ShouldBeSaved() {
  return text_->IsModified() || filename_.IsEmpty();
}

bool FileEdit::CanClose(bool can_abort) {
  if (ShouldBeSaved()) {
    const wxString caption = "Save file?";
    const wxMessageDialogBase::ButtonLabel yes_button = _("&Save it");
    const wxMessageDialogBase::ButtonLabel no_button = _("&Discard changes");
    const wxMessageDialogBase::ButtonLabel cancel_button = _("&Abort");
    const wxString title_ok = "\"" + CalculateDocumentName() + "\" has changed since last time...";
    const wxString title_error = "\"" + CalculateDocumentName() + "\" has changed since last time, save it?";

    const DialogResult answer = can_abort
      ? ShowYesNoCancel(this, caption, yes_button, no_button, cancel_button, title_ok, title_error)
      : ShowYesNo(this, caption, yes_button, no_button, title_ok, title_error);

    
    if (answer == DialogResult::YES) {
      return Save();
    }
    else if (answer == DialogResult::NO) {
      return true;
    }
    else {
      assert(answer == DialogResult::CANCEL);
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
    if (c == '}') {
      // handles cases like {} <newline here>
      // but not } <newline here>
      if (change > 0) {
        change -= 1;
      }
    }

    // decrease is below...
  }
  return change;
}

void FileEdit::OnCharAdded(wxStyledTextEvent& event)
{
  int entered_character = event.GetKey(); // the key seems to be the char that was added

  if (entered_character == '{') {
    if (main_->settings().autocomplete_curly_braces()) {
      text_->InsertText(text_->GetCurrentPos(), "}");
    }
  }
  else if (entered_character == '(') {
    // todo: make completion of () smarter
    if (main_->settings().autocomplete_parentheses()) {
      text_->InsertText(text_->GetCurrentPos(), ")");
    }
  }
  else if (entered_character == '[') {
    // todo: make completion of [] smarter
    if (main_->settings().autocomplete_brackets()) {
      text_->InsertText(text_->GetCurrentPos(), "]");
    }
  }
  else if (entered_character == '\n' || entered_character == '\r')
  {
    // fixing the line margin width since we may need to expand it 
    // going from line 99 to 100
    SetupLineMargin(text_, main_->settings());

    // auto-indenting
    // loosely based on http://www.scintilla.org/ScintillaUsage.html and https://groups.google.com/forum/#!topic/scintilla-interest/vTwXwIBswSM
    const int current_line = text_->GetCurrentLine();
    const int line_start = text_->PositionFromLine(text_->GetCurrentLine()-1);
    const int line_end = text_->PositionFromLine(text_->GetCurrentLine());
    const int next_end = text_->PositionFromLine(text_->GetCurrentLine()+1);
    const wxString previous_line_content = text_->GetTextRange(line_start, line_end);
    const wxString current_line_content = text_->GetTextRange(line_end, next_end);

    const int indent_change = main_->settings().auto_indentation() == ride::AUTOINDENTATION_SMART ?
      CalculateIndentationChange(previous_line_content) : 0;
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

    if (main_->settings().auto_indentation() != ride::AUTOINDENTATION_NONE) {
      if (indentation_in_spaces_ajdusted != 0) {
        text_->SetLineIndentation(current_line, indentation_in_spaces_ajdusted);
        text_->GotoPos(text_->PositionFromLine(current_line) + indentation_in_chars);
      }

      if (current_line_content.StartsWith("}")) {
        // handle case like { USER_ENTER_NEWLINE_HERE }
        // add a newline after this and...
        text_->InsertText(text_->GetCurrentPos(), "\n");

        // ...set that newline - 1 indentation of the current row
        text_->SetLineIndentation(current_line+1, indentation_in_spaces_ajdusted-main_->settings().tabwidth());
      }
    }
  }
}

void FileEdit::HighlightCurrentWord() {
  const int current_position = text_->GetCurrentPos();

  // highlight current word
  const bool only_word_characters = true;
  const int current_start_position = text_->WordStartPosition(current_position, only_word_characters);
  const int current_end_position = text_->WordEndPosition(current_position, only_word_characters);

  if (current_start_position != highlight_current_word_last_start_position_ || current_end_position != highlight_current_word_last_end_position_) {
    text_->SetIndicatorCurrent(ID_INDICATOR_SELECT_HIGHLIGHT);

    // clear old highlight
    if (highlight_current_word_last_start_position_ != -1 && highlight_current_word_last_end_position_ != -1 && highlight_current_word_last_start_position_ != highlight_current_word_last_end_position_) {
      text_->IndicatorClearRange(0, text_->GetLength());
    }

    if (current_start_position != -1 && current_end_position != -1 && current_start_position != current_end_position) {
      const int length = current_end_position - current_start_position;
      assert(length > 0);

      const wxString current_text = text_->GetRange(current_start_position, current_end_position);

      const bool highlight_keyword = main_->settings().highlight_word_also_highlight_keywords();
      const bool is_keyword = highlight_keyword ? false
        : (current_language_ ? current_language_->IsKeyword(current_text) : false);

      if (is_keyword == false) {
        // search through the entire document for this text and highlight it
        int search_point = 0;
        while (true) {
          int match_position = text_->FindText(search_point, text_->GetLength(), current_text, NULL, wxSTC_FIND_WHOLEWORD | wxSTC_FIND_MATCHCASE);
          if (match_position == -1) {
            break;
          }
          bool highlight_match = false;
          if (match_position == current_start_position) {
            // todo: set highlight_this to true depending on the the setting to highlight the current word or not...
          }
          else {
            highlight_match = true;
          }

          if (highlight_match) {
            text_->IndicatorFillRange(match_position, length);
          }
          search_point = match_position + length;
        }
      }
    }
    highlight_current_word_last_start_position_ = current_start_position;
    highlight_current_word_last_end_position_ = current_end_position;
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
  UpdateTitle();
  HighlightCurrentWord();
}
