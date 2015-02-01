#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>

#include <vector>

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

FileEdit::FileEdit(wxAuiNotebook* anotebook, MainWindow* parent, const wxString& source, const wxString& file) : wxControl(parent, wxID_ANY), main(parent), notebook(anotebook), dirty(false) {
  text = new wxStyledTextCtrl(this, wxID_ANY);

  filename = file;

  wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));
  text->SetFont(font);
  text->SetWrapMode(wxSTC_WRAP_WORD); // other choice is wxSCI_WRAP_NONE


  // Use all the bits in the style byte as styles, not indicators.
  text->SetStyleBits(8);

  // default font for all styles
  text->SetViewEOL(main->getSettings().displayEOLEnable);
  text->SetIndentationGuides(main->getSettings().indentGuideEnable);
  text->SetEdgeMode(main->getSettings().longLineOnEnable ?
  wxSTC_EDGE_LINE : wxSTC_EDGE_NONE);
  text->SetViewWhiteSpace(main->getSettings().whiteSpaceEnable ?
  wxSTC_WS_VISIBLEALWAYS : wxSTC_WS_INVISIBLE);
  text->SetOvertype(main->getSettings().overTypeInitial);
  text->SetReadOnly(false);
  text->SetWrapMode(main->getSettings().wrapModeInitial ?
  wxSTC_WRAP_WORD : wxSTC_WRAP_NONE);
  // wxFont font(10, wxMODERN, wxNORMAL, wxNORMAL);
  text->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
  text->StyleSetForeground(wxSTC_STYLE_DEFAULT, *wxBLACK);
  text->StyleSetBackground(wxSTC_STYLE_DEFAULT, *wxWHITE);
  text->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(wxT("DARK GREY")));
  text->StyleSetBackground(wxSTC_STYLE_LINENUMBER, *wxWHITE);
  text->StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColour(wxT("DARK GREY")));
  // InitializePrefs(DEFAULT_LANGUAGE);

  // set visibility
  text->SetVisiblePolicy(wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1);
  text->SetXCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);
  text->SetYCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);

  /*
  text->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_DOTDOTDOT, wxT("BLACK"), wxT("BLACK"));
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN, wxT("BLACK"), wxT("BLACK"));
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY, wxT("BLACK"), wxT("BLACK"));
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_DOTDOTDOT, wxT("BLACK"), wxT("WHITE"));
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, wxT("BLACK"), wxT("WHITE"));
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY, wxT("BLACK"), wxT("BLACK"));
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY, wxT("BLACK"), wxT("BLACK"));
  */

  const wxColor grey(100, 100, 100);
  const wxColor white(255, 255, 255);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW, grey, white);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, grey, white);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY, grey, grey);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY, grey, grey);

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

int FileEdit::DeterminePrefs(const wxString &filename) {
  for (size_t languageNr = 0; languageNr < main->getLanguages().size(); languageNr++) {
    const LanguageInfo* curInfo = &main->getLanguages()[languageNr];
    wxString filepattern = curInfo->filepattern;
    filepattern.Lower();
    while (!filepattern.empty()) {
      wxString cur = filepattern.BeforeFirst(';');
      if ((cur == filename) ||
        (cur == (filename.BeforeLast('.') + wxT(".*"))) ||
        (cur == (wxT("*.") + filename.AfterLast('.')))) {
        return languageNr;
      }
      filepattern = filepattern.AfterFirst(';');
    }
  }
  return -1;
}

bool FileEdit::InitializePrefs(int index) {
  // initialize styles
  text->StyleClearAll();

  if (index >= main->getLanguages().size()){
    return false;
  }
  if (index < 0) {
    return false;
  }

  LanguageInfo const* curInfo = &main->getLanguages()[index];

  // set lexer and language
  text->SetLexer(curInfo->lexer);

  // set margin for line numbers
  text->SetMarginType(m_LineNrID, wxSTC_MARGIN_NUMBER);
  text->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(wxT("DARK GREY")));
  text->StyleSetBackground(wxSTC_STYLE_LINENUMBER, *wxWHITE);
  text->SetMarginWidth(m_LineNrID, main->getSettings().lineNumberEnable ? m_LineNrMargin : 0); // start out not visible

  // default fonts for all styles!
  for (size_t Nr = 0; Nr < wxSTC_STYLE_LASTPREDEFINED; Nr++) {
    wxFont font(10, wxMODERN, wxNORMAL, wxNORMAL);
    text->StyleSetFont(Nr, font);
  }

  // set common styles
  text->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColour(wxT("DARK GREY")));
  text->StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColour(wxT("DARK GREY")));

  // initialize settings
  if (main->getSettings().syntaxEnable) {
    int keywordnr = 0;
    for (size_t Nr = 0; Nr < curInfo->styles.size(); Nr++) {
      if (curInfo->styles[Nr].type == -1) continue;
      const StyleLink& stylelink = curInfo->styles[Nr];
      const StyleInfo &curType = main->getStyles()[stylelink.type];
      wxFont font(curType.fontsize, wxMODERN, wxNORMAL, wxNORMAL, false,
        curType.fontname);
      text->StyleSetFont(Nr, font);
      if (curType.hasforeground) {
        text->StyleSetForeground(Nr, curType.foreground);
      }
      if (curType.hasbackground) {
        text->StyleSetBackground(Nr, curType.background);
      }
      text->StyleSetBold(Nr, curType.bold);
      text->StyleSetItalic(Nr, curType.italic);
      text->StyleSetUnderline(Nr, curType.underline);
      text->StyleSetVisible(Nr, curType.visible);
      /*
#define wxSTC_CASE_MIXED 0
#define wxSTC_CASE_UPPER 1
#define wxSTC_CASE_LOWER 2
      */
      text->StyleSetCase(Nr, wxSTC_CASE_MIXED);
      if (stylelink.hasWords) {
        text->SetKeyWords(keywordnr, stylelink.words);
        keywordnr += 1;
      }
    }
  }

  // set margin as unused
  text->SetMarginType(m_DividerID, wxSTC_MARGIN_SYMBOL);
  text->SetMarginWidth(m_DividerID, 15);
  text->SetMarginSensitive(m_DividerID, false);

  // folding
  text->SetMarginType(m_FoldingID, wxSTC_MARGIN_SYMBOL);
  text->SetMarginMask(m_FoldingID, wxSTC_MASK_FOLDERS);
  // text->StyleSetBackground(m_FoldingID, wxColor(200, 200, 200));
  text->SetMarginWidth(m_FoldingID, 15);
  text->SetMarginSensitive(m_FoldingID, true);
  // text->SetFoldMarginColour(true, wxColor(200, 200, 200));

  text->SetProperty(wxT("fold"), wxT("1"));
  text->SetProperty(wxT("fold.comment"), wxT("1"));
  text->SetProperty(wxT("fold.compact"), wxT("1"));

  if (main->getSettings().foldEnable) {
    text->SetMarginWidth(m_FoldingID, curInfo->folds != 0 ? m_FoldingMargin : 0);
    text->SetMarginSensitive(m_FoldingID, curInfo->folds != 0);
    text->SetProperty(wxT("fold"), curInfo->folds != 0 ? wxT("1") : wxT("0"));
    text->SetProperty(wxT("fold.comment"),
      (curInfo->folds & mySTC_FOLD_COMMENT) > 0 ? wxT("1") : wxT("0"));
    text->SetProperty(wxT("fold.compact"),
      (curInfo->folds & mySTC_FOLD_COMPACT) > 0 ? wxT("1") : wxT("0"));
    text->SetProperty(wxT("fold.preprocessor"),
      (curInfo->folds & mySTC_FOLD_PREPROC) > 0 ? wxT("1") : wxT("0"));
    text->SetProperty(wxT("fold.html"),
      (curInfo->folds & mySTC_FOLD_HTML) > 0 ? wxT("1") : wxT("0"));
    text->SetProperty(wxT("fold.html.preprocessor"),
      (curInfo->folds & mySTC_FOLD_HTMLPREP) > 0 ? wxT("1") : wxT("0"));
    text->SetProperty(wxT("fold.comment.python"),
      (curInfo->folds & mySTC_FOLD_COMMENTPY) > 0 ? wxT("1") : wxT("0"));
    text->SetProperty(wxT("fold.quotes.python"),
      (curInfo->folds & mySTC_FOLD_QUOTESPY) > 0 ? wxT("1") : wxT("0"));
  }
  text->SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED |
    wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

  // set spaces and indention
  text->SetTabWidth(4);
  text->SetUseTabs(false);
  text->SetTabIndents(true);
  text->SetBackSpaceUnIndents(true);
  text->SetIndent(main->getSettings().indentEnable ? 4 : 0);

  // others
  text->SetViewEOL(main->getSettings().displayEOLEnable);
  text->SetIndentationGuides(main->getSettings().indentGuideEnable);
  text->SetEdgeColumn(80);
  text->SetEdgeMode(main->getSettings().longLineOnEnable ? wxSTC_EDGE_LINE : wxSTC_EDGE_NONE);
  text->SetViewWhiteSpace(main->getSettings().whiteSpaceEnable ?
  wxSTC_WS_VISIBLEALWAYS : wxSTC_WS_INVISIBLE);
  text->SetOvertype(main->getSettings().overTypeInitial);
  text->SetReadOnly(false);
  text->SetWrapMode(main->getSettings().wrapModeInitial ? wxSTC_WRAP_WORD : wxSTC_WRAP_NONE);

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
    InitializePrefs(DeterminePrefs(fname.GetFullName()));
  }
}

void FileEdit::updateTitle() {
  size_t index = notebook->GetPageIndex(this);
  const wxString changestar = dirty ? "*" : "";
  notebook->SetPageText(index, docname + changestar);
}

void FileEdit::OnSize(wxSizeEvent& event) {
  int x = text->GetClientSize().x +
    (main->getSettings().lineNumberEnable ? m_LineNrMargin : 0) +
    (main->getSettings().foldEnable ? m_FoldingMargin : 0);
  if (x > 0) text->SetScrollWidth(x);
  event.Skip();
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
  EVT_SIZE(OnSize)
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
