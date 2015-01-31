#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>

#include "ride/fileedit.h"

enum
{
  MARGIN_LINE_NUMBERS,
  MARGIN_FOLD
};

FileEdit::FileEdit(wxAuiNotebook* anotebook, wxWindow* parent, const wxString& source, const wxString& file) : wxControl(parent, wxID_ANY), notebook(anotebook), dirty(false) {
  text = new wxStyledTextCtrl(this, wxID_ANY);

  filename = file;

  text->StyleClearAll();
  text->SetLexer(wxSTC_LEX_CPP);

  wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));
  text->SetFont(font);

  text->SetMarginWidth(MARGIN_LINE_NUMBERS, 50);
  text->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
  text->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
  text->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

  // ---- Enable code folding
  text->SetMarginType(MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
  text->SetMarginWidth(MARGIN_FOLD, 15);
  text->SetMarginMask(MARGIN_FOLD, wxSTC_MASK_FOLDERS);
  text->StyleSetBackground(MARGIN_FOLD, wxColor(200, 200, 200));
  text->SetMarginSensitive(MARGIN_FOLD, true);

  // Properties found from http://www.scintilla.org/SciTEDoc.html
  text->SetProperty(wxT("fold"), wxT("1"));
  text->SetProperty(wxT("fold.comment"), wxT("1"));
  text->SetProperty(wxT("fold.compact"), wxT("1"));

  wxColor grey(100, 100, 100);
  text->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW);
  text->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, grey);
  text->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, grey);

  text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN);
  text->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, grey);
  text->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, grey);

  text->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
  text->MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB, grey);
  text->MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, grey);

  text->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW);
  text->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, grey);
  text->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, _T("WHITE"));

  text->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN);
  text->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, grey);
  text->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, _T("WHITE"));

  text->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
  text->MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL, grey);
  text->MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, grey);

  text->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);
  text->MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL, grey);
  text->MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, grey);
  // ---- End of code folding part

  text->SetWrapMode(wxSTC_WRAP_WORD); // other choice is wxSCI_WRAP_NONE

  if (filename.IsEmpty()) {
    text->SetText(source);
  }
  else {
    text->LoadFile(filename);
  }
  dirty = false;

  text->StyleSetForeground(wxSTC_C_STRING, wxColour(150, 0, 0));
  text->StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColour(165, 105, 0));
  text->StyleSetForeground(wxSTC_C_IDENTIFIER, wxColour(40, 0, 60));
  text->StyleSetForeground(wxSTC_C_NUMBER, wxColour(0, 150, 0));
  text->StyleSetForeground(wxSTC_C_CHARACTER, wxColour(150, 0, 0));
  text->StyleSetForeground(wxSTC_C_WORD, wxColour(0, 0, 150));
  text->StyleSetForeground(wxSTC_C_WORD2, wxColour(0, 150, 0));
  text->StyleSetForeground(wxSTC_C_COMMENT, wxColour(150, 150, 150));
  text->StyleSetForeground(wxSTC_C_COMMENTLINE, wxColour(150, 150, 150));
  text->StyleSetForeground(wxSTC_C_COMMENTDOC, wxColour(150, 150, 150));
  text->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColour(0, 0, 200));
  text->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0, 0, 200));
  text->StyleSetBold(wxSTC_C_WORD, true);
  text->StyleSetBold(wxSTC_C_WORD2, true);
  text->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);

  // a sample list of keywords, I haven't included them all to keep it short...
  text->SetKeyWords(0, wxT("return for while break continue"));
  text->SetKeyWords(1, wxT("const int float void char double"));

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
  }
}

void FileEdit::updateTitle() {
  size_t index = notebook->GetPageIndex(this);
  const wxString changestar = dirty ? "*" : "";
  notebook->SetPageText(index, docname+ changestar);
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
  if (event.GetMargin() == MARGIN_FOLD)
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
