#include <wx/wx.h>
#include <wx/stc/stc.h>

class MyApp: public wxApp
{
public:
virtual bool OnInit();
};

enum
{
  MARGIN_LINE_NUMBERS,
  MARGIN_FOLD
};

class MyFrame: public wxFrame
{
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	
private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

  void OnMarginClick(wxStyledTextEvent& event);
	wxDECLARE_EVENT_TABLE();

private:
  void setupEdit();
  wxStyledTextCtrl* text;
};

enum
{
	ID_Hello = 1
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_Hello, MyFrame::OnHello)
	EVT_MENU(wxID_EXIT, MyFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MyFrame *frame = new MyFrame("Pegasus - Rust IDE", wxPoint(50, 50), wxSize(450, 340) );
	frame->Show( true );
	return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( menuFile, "&File" );
	menuBar->Append( menuHelp, "&Help" );
	SetMenuBar( menuBar );
	CreateStatusBar();
	SetStatusText("");

  setupEdit();
}

void MyFrame::setupEdit() {
  text = new wxStyledTextCtrl(this, wxID_ANY);

  text->StyleClearAll();
  text->SetLexer(wxSTC_LEX_CPP);

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

  // text->SetText(source);

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

  text->Connect(wxEVT_STC_MARGINCLICK, wxStyledTextEventHandler(MyFrame::OnMarginClick), NULL, this);
}

/** Event callback when a margin is clicked, used here for code folding */
void MyFrame::OnMarginClick(wxStyledTextEvent& event)
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

void MyFrame::OnExit(wxCommandEvent& event)
{
	Close( true );
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox( "This is a wxWidgets' Hello world sample", "About Hello World", wxOK | wxICON_INFORMATION );
}

void MyFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}
