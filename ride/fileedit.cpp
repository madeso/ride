#include "ride/wx.h"
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>

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

//////////////////////////////////////////////////////////////////////////

void FileEdit::Undo() {
  if (!text->CanUndo()) return;
  text->Undo();
  updateTitle();
}


void FileEdit::Redo() {
  if (!text->CanRedo()) return;
  text->Redo();
  updateTitle();
}


void FileEdit::Cut() {
  if (text->GetReadOnly() || (text->GetSelectionEnd() - text->GetSelectionStart() <= 0)) return;
  text->Cut();
  updateTitle();
}


void FileEdit::Copy() {
  if (text->GetSelectionEnd() - text->GetSelectionStart() <= 0) return;
  text->Copy();
}


void FileEdit::Paste() {
  if (!text->CanPaste()) return;
  text->Paste();
  updateTitle();
}


void FileEdit::Duplicate() {
  text->SelectionDuplicate();
  updateTitle();
}


void FileEdit::Delete() {
  if (text->GetReadOnly()) return;
  text->Clear();
  updateTitle();
}


void FileEdit::Find() {
  // todo: implement me
}


void FileEdit::Replace() {
  // todo: implement me
}


void FileEdit::MatchBrace() {
  int start_brace = text->GetCurrentPos();
  int other_brace = text->BraceMatch(start_brace);
  if (other_brace == -1) return;

  text->SetSelection(other_brace, other_brace);
}


void FileEdit::SelectBrace() {
  int start_brace = text->GetCurrentPos();
  int other_brace = text->BraceMatch(start_brace);
  if (other_brace == -1) return;

  if (other_brace < start_brace) {
    std::swap(start_brace, other_brace);
  }

  assert(start_brace < other_brace);
  text->SetSelection(start_brace, other_brace + 1);
}


void FileEdit::GotoLine() {
  const int total_plus_one = text->GetLineCount()+1;
  const wxString message = wxString::Format("Enter line number(1-%d)", total_plus_one);
  const long new_line_one_based = wxGetNumberFromUser(message, wxEmptyString, "Goto line", text->GetCurrentLine()+1, 1, total_plus_one);
  if (new_line_one_based == -1) return;
  const int pos = text->PositionFromLine(new_line_one_based-1);
  text->SetSelection(pos, pos);
  text->ScrollToLine(new_line_one_based - 1);
}


void FileEdit::Indent() {
  // todo: fix issue with replacing selection
  text->CmdKeyExecute(wxSTC_CMD_TAB);
  updateTitle();
}


void FileEdit::UnIndent() {
  // todo: fix issue with replacing selection
  text->CmdKeyExecute(wxSTC_CMD_DELETEBACK);
  updateTitle();
}


void FileEdit::SelectAll() {
  text->SetSelection(0, text->GetTextLength());
}


void FileEdit::SelectLine() {
  int lineStart = text->PositionFromLine(text->GetCurrentLine());
  int lineEnd = text->PositionFromLine(text->GetCurrentLine() + 1);
  text->SetSelection(lineStart, lineEnd);
}

void FileEdit::ToUpper() {
  text->UpperCase();
  updateTitle();
}

void FileEdit::ToLower() {
  text->LowerCase();
  updateTitle();
}

void FileEdit::MoveLinesUp() {
  text->MoveSelectedLinesUp();
  updateTitle();
}

void FileEdit::MoveLinesDown() {
  text->MoveSelectedLinesDown();
  updateTitle();
}

void FileEdit::ShowProperties() {
  // todo: implement me
}

//////////////////////////////////////////////////////////////////////////

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

FileEdit::FileEdit(wxAuiNotebook* anotebook, MainWindow* parent, const wxString& source, const wxString& file) : wxControl(parent, wxID_ANY), main(parent), notebook(anotebook), currentLanguage(NULL) {
  text = new wxStyledTextCtrl(this,  wxID_ANY, wxDefaultPosition, wxDefaultSize,
#ifndef __WXMAC__
    wxSUNKEN_BORDER |
#endif
    wxVSCROLL);

  filename = file;

  if (filename.IsEmpty()) {
    text->SetText(source);
  }
  else {
    text->LoadFile(filename);
  }

  m_LineNrMargin = text->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_999999"));

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(text, 1, wxEXPAND);
  SetSizer(sizer);

  notebook->AddPage(this, wxT(""), true);
  updateFilename();
  updateTitle();
}

bool FileEdit::Save() {
  if (shouldBeSaved() == false) return true;
  if (filename.IsEmpty()) return SaveAs();
  else return saveTo(filename);
}

bool FileEdit::SaveAs() {
  wxFileDialog saveFileDialog(this, _("Save file"), "", "",
    FILE_PATTERN, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return false;
  return saveTo(saveFileDialog.GetPath());
}

bool FileEdit::saveTo(const wxString& target) {
  if (false == text->SaveFile(target)) {
    return false;
  }
  filename = target;
  updateFilename();
  updateTitle();
  return true;
}

unsigned int UntitledCount = 0;

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
  text->StyleClearAll();
  wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));
  const wxColor grey(100, 100, 100);
  const wxColor white(255, 255, 255);
  const wxColor darkgray = wxColour(20, 20, 20);

  const ride::Settings& set = main->getSettings();

  // setup language color
  assert(currentLanguage);
  currentLanguage->style(text, set);

  //////////////////////////////////////////////////////////////////////////

  // set margin for line numbers
  text->SetMarginType(ID_MARGIN_LINENUMBER, wxSTC_MARGIN_NUMBER);
  text->SetMarginWidth(ID_MARGIN_LINENUMBER, set.linenumberenable() ? m_LineNrMargin : 0);

  // set margin as unused
  text->SetMarginType(ID_MARGIN_DIVIDER, wxSTC_MARGIN_SYMBOL);
  text->SetMarginWidth(ID_MARGIN_DIVIDER, 15);
  text->SetMarginSensitive(ID_MARGIN_DIVIDER, false);

  // folding settings
  // todo: move to settings
  text->SetMarginType(ID_MARGIN_FOLDING, wxSTC_MARGIN_SYMBOL);
  text->SetMarginMask(ID_MARGIN_FOLDING, wxSTC_MASK_FOLDERS);
  // text->StyleSetBackground(m_FoldingID, wxColor(200, 200, 200));
  text->SetMarginWidth(ID_MARGIN_FOLDING, 15);
  text->SetMarginSensitive(ID_MARGIN_FOLDING, true);
  text->SetFoldMarginColour(true, wxColor(200, 200, 200));
  text->SetFoldMarginHiColour(true, wxColor(200, 200, 200));
  text->SetMarginWidth(ID_MARGIN_FOLDING, set.foldenable() ? FOLDING_WIDTH : 0);
  text->SetMarginSensitive(ID_MARGIN_FOLDING, set.foldenable());
  text->SetFoldFlags(C(set.foldflags()));
  // todo: expose theese
  text->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW, grey, white);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, grey, white);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY, grey, grey);

  // set spaces and indention
  text->SetTabWidth(set.tabwidth());
  text->SetUseTabs(set.usetabs());
  text->SetTabIndents(set.tabindents());
  text->SetBackSpaceUnIndents(set.backspaceunindents());
  text->SetIndent(set.tabwidth());

  text->SetViewEOL(set.displayeolenable());
  text->SetIndentationGuides(set.indentguideenable());
  text->SetEdgeMode(C(set.edgestyle()));
  text->SetEdgeColour(C(set.fonts_and_colors().edgecolor()));
  text->SetEdgeColumn(set.edgecolumn());
  text->SetViewWhiteSpace(C(set.whitespace()));
  text->SetOvertype(false);
  text->SetReadOnly(false);
  text->SetWrapMode(C(set.wordwrap()));

  // set visibility
  // todo: investigate this
  text->SetVisiblePolicy(wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1);
  text->SetXCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);
  text->SetYCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);

  text->SetAdditionalSelectionTyping(true);
  text->SetAdditionalCaretsBlink(true);
  text->SetAdditionalCaretsVisible(true);

  SetIndicator(text, ID_INDICATOR_ERROR, set.indicator_error());
  SetIndicator(text, ID_INDICATOR_WARNING, set.indicator_warning());
  SetIndicator(text, ID_INDICATOR_SEARCH_HIGHLIGHT, set.indicator_search_highlight());
  SetIndicator(text, ID_INDICATOR_SELECT_HIGHLIGHT, set.indicator_select_highlight());

  // setup style colors and font
  SetStyle(text, wxSTC_STYLE_DEFAULT, set.fonts_and_colors().default_style());
  SetStyle(text, wxSTC_STYLE_BRACELIGHT, set.fonts_and_colors().bracelight_style());
  SetStyle(text, wxSTC_STYLE_BRACEBAD, set.fonts_and_colors().bracebad_style());
  SetStyle(text, wxSTC_STYLE_CONTROLCHAR, set.fonts_and_colors().controlchar_style());
  SetStyle(text, wxSTC_STYLE_INDENTGUIDE, set.fonts_and_colors().indentguide_style());
  SetStyle(text, wxSTC_STYLE_CALLTIP, set.fonts_and_colors().calltip_style());


  // settings for this
  text->SetEndAtLastLine(false);

  // todo: expose this
  text->SetCaretLineVisible(true);
}

void FileEdit::updateFilename() {
  if (filename.IsEmpty()) {
    docname = wxString::Format("Untitled %d", ++UntitledCount);
  }
  else {
    wxFileName fn(filename);
    docname = fn.GetFullName();
  }

  if (filename.IsEmpty() == false) {
    size_t index = notebook->GetPageIndex(this);
    notebook->SetPageToolTip(index, filename);

    wxFileName fname(filename);
    currentLanguage = DetermineLanguage(fname.GetFullName());
    UpdateTextControl();
    UpdateTextControl(); // update colors again, doing it twice seems to be needed to apply the colors
  }
}

void FileEdit::updateTitle() {
  size_t index = notebook->GetPageIndex(this);
  const wxString changestar = text->IsModified() ? "*" : "";
  notebook->SetPageText(index, docname + changestar);
}

bool FileEdit::shouldBeSaved() {
  return text->IsModified() || filename.IsEmpty();
}

bool FileEdit::canClose(bool canAbort) {
  if (shouldBeSaved()) {
    const int yesNoFlags = canAbort ? (wxYES_NO | wxCANCEL) : wxYES_NO;
    wxMessageDialog dlg(this, _(""), _("Save file?"), yesNoFlags | wxICON_QUESTION);

    const wxMessageDialogBase::ButtonLabel yesButton = _("&Save it");
    const wxMessageDialogBase::ButtonLabel noButton = _("&Discard changes");
    const wxMessageDialogBase::ButtonLabel cancelButton = _("&Abort");

    const bool labelChangeOk = canAbort
      ? dlg.SetYesNoCancelLabels(yesButton, noButton, cancelButton)
      : dlg.SetYesNoLabels(yesButton, noButton)
      ;

    if (labelChangeOk) {
      dlg.SetMessage("\"" + docname + "\" has changed since last time...");
    }
    else {
      dlg.SetMessage("\"" + docname + "\" has changed since last time, save it?");
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
  text->SetClientData(NULL);
}

wxBEGIN_EVENT_TABLE(FileEdit, wxControl)
EVT_STC_MARGINCLICK(wxID_ANY, FileEdit::OnMarginClick)
EVT_STC_CHARADDED(wxID_ANY, FileEdit::OnTextChanged)
// EVT_STC_MODIFIED(wxID_ANY, FileEdit::OnTextModified) // this was not a good idea for makeDiry calls, OnPaint seems to generate modified messages
wxEND_EVENT_TABLE()

void FileEdit::OnMarginClick(wxStyledTextEvent& event)
{
  if (event.GetMargin() == ID_MARGIN_FOLDING)
  {
    int lineClick = text->LineFromPosition(event.GetPosition());
    int levelClick = text->GetFoldLevel(lineClick);

    if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0)
    {
      text->ToggleFold(lineClick);
    }
  }
}

int GetIndentationChange(const wxString& str) {
  // todo: move to language...
  int change = 0;
  for (size_t i = 0; i < str.Length(); ++i) {
    const wxUniChar c = str.at(i);
    if (c == '{') {
      change += 1;
    }
    else if (c == '}') {
      change += 1;
    }
  }
  return change;
}

void FileEdit::OnTextChanged(wxStyledTextEvent& event)
{
  updateTitle();

  // auto-indenting
  // todo: add settings for this...
  // loosley based on http://www.scintilla.org/ScintillaUsage.html and https://groups.google.com/forum/#!topic/scintilla-interest/vTwXwIBswSM
  int chr = event.GetKey(); // the key seems to be the char that was added
  if (chr == '\n' || chr == '\r')
  {
    const int current_line = text->GetCurrentLine();

    const int line_start = text->PositionFromLine(text->GetCurrentLine()-1);
    const int line_end = text->PositionFromLine(text->GetCurrentLine());
    const wxString previous_line_content = text->GetTextRange(line_start, line_end);
    const int indent_change = GetIndentationChange(previous_line_content);
    const int indent_change_in_spaces = indent_change * main->getSettings().tabwidth();

    const int smart_indent = indent_change_in_spaces;

    const int indentation_in_spaces = current_line > 0
      ?  std::max(0, text->GetLineIndentation(current_line - 1) + smart_indent)
      : smart_indent;

    const int indentation_in_tabs = indentation_in_spaces / main->getSettings().tabwidth();

    // if we use tabs, divide the number of character by the char width to get the actual width
    const int indentation_in_chars = main->getSettings().usetabs()
      ? indentation_in_tabs
      : indentation_in_spaces;

    // adjust to remove weird spaces from indentation settings
    const int indentation_in_spaces_ajdusted = indentation_in_tabs * main->getSettings().tabwidth();

    if (indentation_in_spaces_ajdusted != 0)   // NOT in the beginning
    {
      text->SetLineIndentation(current_line, indentation_in_spaces_ajdusted);
      
      text->GotoPos(text->PositionFromLine(current_line) + indentation_in_chars);
    }
  }
}

