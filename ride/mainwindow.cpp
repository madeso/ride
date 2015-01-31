#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/filename.h>

#include "ride/mainwindow.h"
#include "ride/fileedit.h"

enum
{
  ID_Random = 1
};

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(wxID_OPEN, MainWindow::OnOpen)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)

EVT_MENU(wxID_SAVE, MainWindow::OnSave)
EVT_MENU(wxID_SAVEAS, MainWindow::OnSaveAs)

EVT_CLOSE(OnClose)

EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainWindow::OnNotebookPageClose)
EVT_AUINOTEBOOK_PAGE_CLOSED(wxID_ANY, MainWindow::OnNotebookPageClosed)
wxEND_EVENT_TABLE()

enum {
  mySTC_TYPE_DEFAULT,
  mySTC_TYPE_COMMENT,
  mySTC_TYPE_COMMENT_LINE,
  mySTC_TYPE_COMMENT_DOC,
  mySTC_TYPE_NUMBER,
  mySTC_TYPE_WORD1,
  mySTC_TYPE_STRING,
  mySTC_TYPE_CHARACTER,
  mySTC_TYPE_UUID,
  mySTC_TYPE_PREPROCESSOR,
  mySTC_TYPE_OPERATOR,
  mySTC_TYPE_IDENTIFIER,
  mySTC_TYPE_STRING_EOL,
  mySTC_TYPE_REGEX,
  mySTC_TYPE_COMMENT_SPECIAL,
  mySTC_TYPE_WORD2,
  mySTC_TYPE_WORD3,
  mySTC_TYPE_ERROR
};

void SetupLanguages(std::vector<LanguageInfo>& languages, std::vector<StyleInfo>& styles) {
  LanguageInfo info;
  info.name = "C++";
  info.filepattern = "*.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma";
  info.lexer = wxSTC_LEX_CPP;

  const wxString CppWordlist1 =
    "asm auto bool break case catch char class const const_cast "
    "continue default delete do double dynamic_cast else enum explicit "
    "export extern false float for friend goto if inline int long "
    "mutable namespace new operator private protected public register "
    "reinterpret_cast return short signed sizeof static static_cast "
    "struct switch template this throw true try typedef typeid "
    "typename union unsigned using virtual void volatile wchar_t "
    "while";
  const wxString CppWordlist2 =
    "file";
  const wxString CppWordlist3 =
    "a addindex addtogroup anchor arg attention author b brief bug c "
    "class code date def defgroup deprecated dontinclude e em endcode "
    "endhtmlonly endif endlatexonly endlink endverbatim enum example "
    "exception f$ f[ f] file fn hideinitializer htmlinclude "
    "htmlonly if image include ingroup internal invariant interface "
    "latexonly li line link mainpage name namespace nosubgrouping note "
    "overload p page par param post pre ref relates remarks return "
    "retval sa section see showinitializer since skip skipline struct "
    "subsection test throw todo typedef union until var verbatim "
    "verbinclude version warning weakgroup $ @ \"\" & < > # { }";

  info.styles.push_back(StyleLink(mySTC_TYPE_DEFAULT));
  info.styles.push_back(StyleLink(mySTC_TYPE_COMMENT));
  info.styles.push_back(StyleLink(mySTC_TYPE_COMMENT_LINE));
  info.styles.push_back(StyleLink(mySTC_TYPE_COMMENT_DOC));
  info.styles.push_back(StyleLink(mySTC_TYPE_NUMBER));
  info.styles.push_back(StyleLink(mySTC_TYPE_WORD1, CppWordlist1)); // KEYWORDS
  info.styles.push_back(StyleLink(mySTC_TYPE_STRING));
  info.styles.push_back(StyleLink(mySTC_TYPE_CHARACTER));
  info.styles.push_back(StyleLink(mySTC_TYPE_UUID));
  info.styles.push_back(StyleLink(mySTC_TYPE_PREPROCESSOR));
  info.styles.push_back(StyleLink(mySTC_TYPE_OPERATOR));
  info.styles.push_back(StyleLink(mySTC_TYPE_IDENTIFIER));
  info.styles.push_back(StyleLink(mySTC_TYPE_STRING_EOL));
  info.styles.push_back(StyleLink(mySTC_TYPE_DEFAULT)); // VERBATIM
  info.styles.push_back(StyleLink(mySTC_TYPE_REGEX));
  info.styles.push_back(StyleLink(mySTC_TYPE_COMMENT_SPECIAL)); // DOXY
  info.styles.push_back(StyleLink(mySTC_TYPE_WORD2, CppWordlist2)); // EXTRA WORDS
  info.styles.push_back(StyleLink(mySTC_TYPE_WORD3, CppWordlist3)); // DOXY KEYWORDS
  info.styles.push_back(StyleLink(mySTC_TYPE_ERROR)); // KEYWORDS ERROR
  info.folds = static_cast<FoldType>(mySTC_FOLD_COMMENT | mySTC_FOLD_COMPACT | mySTC_FOLD_PREPROC);
  languages.push_back(info);

  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_DEFAULT,
  styles.push_back( StyleInfo("", 10, wxColour(0,255,0), false) ); // mySTC_TYPE_COMMENT,
  styles.push_back( StyleInfo("", 10, wxColour(0,255,0), false) ); // mySTC_TYPE_COMMENT_LINE,
  styles.push_back( StyleInfo("", 10, wxColour(0,255,255), false) ); // mySTC_TYPE_COMMENT_DOC,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_NUMBER,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,255), true) ); // mySTC_TYPE_WORD1,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_STRING,
  styles.push_back( StyleInfo("", 10, wxColour(50,50,50), false) ); // mySTC_TYPE_CHARACTER,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_UUID,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_PREPROCESSOR,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_OPERATOR,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_IDENTIFIER,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_STRING_EOL,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_DEFAULT,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_REGEX,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_COMMENT_SPECIAL,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_WORD2,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_WORD3,
  styles.push_back( StyleInfo("", 10, wxColour(0,0,0), false) ); // mySTC_TYPE_ERROR
}

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
  aui.SetManagedWindow(this);

  wxMenu *menuFile = new wxMenu;
  menuFile->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open a file");
  menuFile->Append(wxID_SAVE, "&Save...\tCtrl-S", "Save the file");
  menuFile->Append(wxID_SAVEAS, "Save &as...\tCtrl-Shift-S", "Save the file as a new file");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);
  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");
  SetMenuBar(menuBar);
  CreateStatusBar();
  SetStatusText("");
  createNotebook();
  aui.Update();

  SetupLanguages(languages, styles);
}

void MainWindow::createNotebook() {
  wxSize client_size = GetClientSize();
  wxAuiNotebook* ctrl = new wxAuiNotebook(this, wxID_ANY,
    wxPoint(client_size.x, client_size.y),
    wxSize(430, 200),
    wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);
  ctrl->Freeze();
  ctrl->Thaw();
  notebook = ctrl;

  aui.AddPane(notebook, wxAuiPaneInfo().Name(wxT("notebook_content")).CenterPane().PaneBorder(true));
}

const Settings& MainWindow::getSettings() const {
  return settings;
}

const std::vector<LanguageInfo>& MainWindow::getLanguages() const {
  return languages;
}

const std::vector<StyleInfo>& MainWindow::getStyles() const {
  return styles;
}

MainWindow::~MainWindow() {
  aui.UnInit();
}

void MainWindow::OnExit(wxCommandEvent& event)
{
  Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
  wxMessageBox("Ride is a Rust IDE. It's named after concatenating R from rust and IDE.\nFor more information: https://github.com/madeso/ride", "About Ride", wxOK | wxICON_INFORMATION);
}

void MainWindow::OnOpen(wxCommandEvent& event)
{
  wxFileDialog
    openFileDialog(this, _("Open file"), "", "",
    FILE_PATTERN, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (openFileDialog.ShowModal() == wxID_CANCEL)
    return;
  
  wxFileName w(openFileDialog.GetPath());
  w.Normalize();
  new FileEdit(notebook, this, "", w.GetFullPath());
}

FileEdit* MainWindow::getSelectedEditorNull() {
  const int selected = notebook->GetSelection();
  wxWindow* window = notebook->GetPage(selected);
  if (window->IsKindOf(CLASSINFO(FileEdit))) {
    FileEdit* edit = reinterpret_cast<FileEdit*>(window);
    return edit;
  }
  else {
    return NULL;
  }
}

void MainWindow::OnSave(wxCommandEvent& event) {
  FileEdit* selected = getSelectedEditorNull();
  if (selected == NULL) return;
  selected->save();
}

void MainWindow::OnSaveAs(wxCommandEvent& event) {
  FileEdit* selected = getSelectedEditorNull();
  if (selected == NULL) return;
  selected->saveAs();
}

void MainWindow::OnNotebookPageClose(wxAuiNotebookEvent& evt) {
  wxWindow* window = notebook->GetPage(evt.GetSelection());
  if (window->IsKindOf(CLASSINFO(FileEdit))) {
    FileEdit* edit = reinterpret_cast<FileEdit*>(window);
    if (edit->canClose(true) == false) {
      evt.Veto();
    }
  }
}

void MainWindow::OnClose(wxCloseEvent& evt) {
  for (unsigned int i = 0; i < notebook->GetPageCount(); ++i) {
    wxWindow* window = notebook->GetPage(i);
    if (window->IsKindOf(CLASSINFO(FileEdit))) {
      FileEdit* edit = reinterpret_cast<FileEdit*>(window);
      const bool canAbort = evt.CanVeto();
      if (edit->canClose(canAbort) == false) {
        evt.Veto();
        return;
      }
    }
  }

  evt.Skip();
}

void MainWindow::OnNotebookPageClosed(wxAuiNotebookEvent& evt) {
}
