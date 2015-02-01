#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>

#include <vector>
#include <cassert>

#include "ride/mainwindow.h"
#include "ride/fileedit.h"

enum
{
  m_LineNrID = 0,
  m_DividerID = 1,
  m_FoldingID = 2,
  m_FoldingMargin = 16
};

const int ANNOTATION_STYLE = wxSTC_STYLE_LASTPREDEFINED + 1;

int C(EdgeStyle::Type e) {
  switch (e) {
  case EdgeStyle::BACKGROUND:
    return wxSTC_EDGE_BACKGROUND;
  case EdgeStyle::LINE:
    return wxSTC_EDGE_LINE;
  case EdgeStyle::NONE:
    return wxSTC_EDGE_NONE;
  default:
    assert(false && "Invalid edge style");
    return wxSTC_EDGE_NONE;
  }
}

int C(ViewWhitespace::Type e) {
  switch (e) {
  case ViewWhitespace::ALWAYS:
    return wxSTC_WS_VISIBLEALWAYS;
  case ViewWhitespace::AFTER_IDENT:
    return wxSTC_WS_VISIBLEAFTERINDENT;
  case ViewWhitespace::HIDDEN:
    return wxSTC_WS_INVISIBLE;
  default:
    assert(false && "Invalid whitespace style");
    return wxSTC_WS_INVISIBLE;
  }
}

int C(WrapMode::Type e) {
  switch (e) {
  case WrapMode::CHAR:
    return wxSTC_WRAP_CHAR;
  case WrapMode::WORD:
    return wxSTC_WRAP_WORD;
  case WrapMode::NONE:
    return wxSTC_WRAP_NONE;
  default:
    assert(false && "Invalid wrap mode");
    return wxSTC_WRAP_NONE;
  }
}

int C(FoldFlags f) {
  int ret = 0;

  if (f.LINEBEFORE_EXPANDED) {
    ret |= wxSTC_FOLDFLAG_LINEBEFORE_EXPANDED;
  }
  if (f.LINEBEFORE_CONTRACTED) {
    ret |= wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED;
  }
  if (f.LINEAFTER_EXPANDED) {
    ret |= wxSTC_FOLDFLAG_LINEAFTER_EXPANDED;
  }
  if (f.LINEAFTER_CONTRACTED) {
    ret |= wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED;
  }
  if (f.LEVELNUMBERS) {
    ret |= wxSTC_FOLDFLAG_LEVELNUMBERS;
  }

  return ret;
}

FileEdit::FileEdit(wxAuiNotebook* anotebook, MainWindow* parent, const wxString& source, const wxString& file) : wxControl(parent, wxID_ANY), main(parent), notebook(anotebook), dirty(false), currentLanguage(NULL) {
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
  dirty = false;

  m_LineNrMargin = text->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_999999"));

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(text, 1, wxEXPAND);
  SetSizer(sizer);

  notebook->AddPage(this, wxT(""), true);
  updateFilename();
  updateTitle();
}

bool FileEdit::save() {
  if (shouldBeSaved() == false) return true;
  if (filename.IsEmpty()) return saveAs();
  else return saveTo(filename);
}

bool FileEdit::saveAs() {
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
  dirty = false;
  updateFilename();
  updateTitle();
  return true;
}

unsigned int UntitledCount = 0;

void SetStyle(wxStyledTextCtrl* text, int id, const Style& style) {
  if (style.foreground != wxNullColour) {
    text->StyleSetForeground(id, style.foreground);
  }
  if (style.background != wxNullColour) {
    text->StyleSetBackground(id, style.background);
  }
  wxFont temp = style.font;
  text->StyleSetFont(id, temp);
}

wxString b2s01(bool b) {
  if (b) return _("1");
  else return _("0");
}

bool FileEdit::UpdateTextControl() {
  // initialize styles
  text->StyleClearAll();
  wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));
  const wxColor grey(100, 100, 100);
  const wxColor white(255, 255, 255);
  const wxColor darkgray = wxColour(20, 20, 20);

  // setup language color
  assert(currentLanguage);
  currentLanguage->style(text, main->getSettings());

  // setup style colors and font
  SetStyle(text, wxSTC_STYLE_DEFAULT, Style(font, darkgray, white));
  text->StyleSetForeground(wxSTC_STYLE_DEFAULT, darkgray);
  SetStyle(text, wxSTC_STYLE_BRACELIGHT, Style(font, darkgray, white));
  SetStyle(text, wxSTC_STYLE_BRACEBAD, Style(font, darkgray, white));
  SetStyle(text, wxSTC_STYLE_CONTROLCHAR, Style(font, darkgray, white));
  SetStyle(text, wxSTC_STYLE_INDENTGUIDE, Style(font, darkgray, white));
  SetStyle(text, wxSTC_STYLE_CALLTIP, Style(font, darkgray, white));

  //////////////////////////////////////////////////////////////////////////

  // set margin for line numbers
  text->SetMarginType(m_LineNrID, wxSTC_MARGIN_NUMBER);
  text->SetMarginWidth(m_LineNrID, main->getSettings().lineNumberEnable ? m_LineNrMargin : 0);

  // set margin as unused
  text->SetMarginType(m_DividerID, wxSTC_MARGIN_SYMBOL);
  text->SetMarginWidth(m_DividerID, 15);
  text->SetMarginSensitive(m_DividerID, false);

  // folding settings
  // todo: move to settings
  text->SetMarginType(m_FoldingID, wxSTC_MARGIN_SYMBOL);
  text->SetMarginMask(m_FoldingID, wxSTC_MASK_FOLDERS);
  // text->StyleSetBackground(m_FoldingID, wxColor(200, 200, 200));
  text->SetMarginWidth(m_FoldingID, 15);
  text->SetMarginSensitive(m_FoldingID, true);
  text->SetFoldMarginColour(true, wxColor(200, 200, 200));
  text->SetFoldMarginHiColour(true, wxColor(200, 200, 200));
  text->SetMarginWidth(m_FoldingID, main->getSettings().foldEnable ? m_FoldingMargin : 0);
  text->SetMarginSensitive(m_FoldingID, main->getSettings().foldEnable);
  text->SetFoldFlags(C(main->getSettings().foldflags));
  text->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW, grey, white);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, grey, white);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY, grey, grey);

  // set spaces and indention
  text->SetTabWidth(main->getSettings().tabWidth);
  text->SetUseTabs(main->getSettings().useTabs);
  text->SetTabIndents(main->getSettings().tabIndents);
  text->SetBackSpaceUnIndents(main->getSettings().backspaceUnindents);
  text->SetIndent(main->getSettings().tabWidth);

  text->SetFont(font);

  text->SetViewEOL(main->getSettings().displayEOLEnable);
  text->SetIndentationGuides(main->getSettings().indentGuideEnable);
  text->SetEdgeMode(C(main->getSettings().edgeStyle));
  text->SetEdgeColour(main->getSettings().edgeColor);
  text->SetEdgeColumn(main->getSettings().edgeColumn);
  text->SetViewWhiteSpace(C(main->getSettings().whitespace));
  text->SetOvertype(false);
  text->SetReadOnly(false);
  text->SetWrapMode(C(main->getSettings().wordWrap));

  // set visibility
  // todo: investigate this
  text->SetVisiblePolicy(wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1);
  text->SetXCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);
  text->SetYCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);

  text->SetCaretLineVisible(true);

  return true;
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
  }
}

void FileEdit::updateTitle() {
  size_t index = notebook->GetPageIndex(this);
  const wxString changestar = dirty ? "*" : "";
  notebook->SetPageText(index, docname + changestar);
}

bool FileEdit::shouldBeSaved() {
  return dirty || filename.IsEmpty();
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
      return save();
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
  EVT_STC_MARGINCLICK(wxID_ANY, OnMarginClick)
  EVT_STC_CHARADDED(wxID_ANY, OnTextChanged)
wxEND_EVENT_TABLE()

void FileEdit::OnMarginClick(wxStyledTextEvent& event)
{
  if (event.GetMargin() == m_FoldingID)
  {
    int lineClick = text->LineFromPosition(event.GetPosition());
    int levelClick = text->GetFoldLevel(lineClick);

    if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0)
    {
      text->ToggleFold(lineClick);
    }
  }
}

void FileEdit::OnTextChanged(wxStyledTextEvent& event)
{
  if (dirty == false) {
    dirty = true;
    updateTitle();
  }
}
