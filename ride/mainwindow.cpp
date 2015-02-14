#include "ride/wx.h"
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/filename.h>

#include "ride/mainwindow.h"
#include "ride/fileedit.h"

#include "ride/settingsdlg.h"

enum
{
  ID_FIRST = wxID_HIGHEST,
  ID_FILE_RIDE_SETTINGS,

  ID_EDIT_MATCH_BRACE,
  ID_EDIT_SELECT_BRACE,
  ID_EDIT_GOTO_LINE,
  ID_EDIT_SELECT_LINE,
  ID_EDIT_TOUPPER,
  ID_EDIT_TOLOWER,
  ID_EDIT_MOVELINESUP,
  ID_EDIT_MOVELINESDOWN,

  ID_PROJECT_NEW,
  ID_PROJECT_OPEN,
  ID_PROJECT_SETTINGS,
  ID_PROJECT_BUILD,
  ID_PROJECT_CLEAN,
  ID_PROJECT_REBUILD,
  ID_PROJECT_DOC,
  ID_PROJECT_RUN,
  ID_PROJECT_TEST,
  ID_PROJECT_BENCH,
  ID_PROJECT_UPDATE
};

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
  EVT_MENU(wxID_OPEN              , MainWindow::OnFileOpen)
  EVT_MENU(wxID_EXIT              , MainWindow::OnFileExit)
  EVT_MENU(ID_FILE_RIDE_SETTINGS  , MainWindow::OnFileShowSettings)
  EVT_MENU(wxID_SAVE              , MainWindow::OnFileSave)
  EVT_MENU(wxID_SAVEAS            , MainWindow::OnFileSaveAs)
  
  EVT_MENU(wxID_UNDO              , MainWindow::OnEditUndo)
  EVT_MENU(wxID_REDO              , MainWindow::OnEditRedo)
  EVT_MENU(wxID_CUT               , MainWindow::OnEditCut)
  EVT_MENU(wxID_COPY              , MainWindow::OnEditCopy)
  EVT_MENU(wxID_PASTE             , MainWindow::OnEditPaste)
  EVT_MENU(wxID_DUPLICATE         , MainWindow::OnEditDuplicate)
  EVT_MENU(wxID_DELETE            , MainWindow::OnEditDelete)
  EVT_MENU(wxID_FIND              , MainWindow::OnEditFind)
  EVT_MENU(wxID_REPLACE           , MainWindow::OnEditReplace)
  EVT_MENU(ID_EDIT_MATCH_BRACE    , MainWindow::OnEditMatchBrace)
  EVT_MENU(ID_EDIT_SELECT_BRACE   , MainWindow::OnEditSelectBrace)
  EVT_MENU(ID_EDIT_GOTO_LINE      , MainWindow::OnEditGotoLine)
  EVT_MENU(wxID_INDENT            , MainWindow::OnEditIndent)
  EVT_MENU(wxID_UNINDENT          , MainWindow::OnEditUnIndent)
  EVT_MENU(wxID_SELECTALL         , MainWindow::OnEditSelectAll)
  EVT_MENU(ID_EDIT_SELECT_LINE    , MainWindow::OnEditSelectLine)
  EVT_MENU(ID_EDIT_TOUPPER        , MainWindow::OnEditToUpper)
  EVT_MENU(ID_EDIT_TOLOWER        , MainWindow::OnEditToLower)
  EVT_MENU(ID_EDIT_MOVELINESUP    , MainWindow::OnEditMoveLinesUp)
  EVT_MENU(ID_EDIT_MOVELINESDOWN  , MainWindow::OnEditMoveLinesDown)
  
  EVT_MENU(ID_PROJECT_NEW         , MainWindow::OnProjectNew     )
  EVT_MENU(ID_PROJECT_OPEN        , MainWindow::OnProjectOpen    )
  EVT_MENU(ID_PROJECT_SETTINGS    , MainWindow::OnProjectSettings)
  EVT_MENU(ID_PROJECT_BUILD       , MainWindow::OnProjectBuild   )
  EVT_MENU(ID_PROJECT_CLEAN       , MainWindow::OnProjectClean   )
  EVT_MENU(ID_PROJECT_REBUILD     , MainWindow::OnProjectRebuild )
  EVT_MENU(ID_PROJECT_DOC         , MainWindow::OnProjectDoc     )
  EVT_MENU(ID_PROJECT_RUN         , MainWindow::OnProjectRun     )
  EVT_MENU(ID_PROJECT_TEST        , MainWindow::OnProjectTest    )
  EVT_MENU(ID_PROJECT_BENCH       , MainWindow::OnProjectBench   )
  EVT_MENU(ID_PROJECT_UPDATE      , MainWindow::OnProjectUpdate  )
  
  EVT_MENU(wxID_ABOUT             , MainWindow::OnAbout)
  
  EVT_CLOSE(MainWindow::OnClose)
  
  EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainWindow::OnNotebookPageClose)
  EVT_AUINOTEBOOK_PAGE_CLOSED(wxID_ANY, MainWindow::OnNotebookPageClosed)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
, project(wxEmptyString)
{
  aui.SetManagedWindow(this);

  LoadSettings(settings);

  //////////////////////////////////////////////////////////////////////////
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open a file");
  menuFile->Append(wxID_SAVE, "&Save...\tCtrl-S", "Save the file");
  menuFile->Append(wxID_SAVEAS, "Save &as...\tCtrl-Shift-S", "Save the file as a new file");
  menuFile->AppendSeparator();
  menuFile->Append(ID_FILE_RIDE_SETTINGS, "S&ettings...", "Change the settings of RIDE");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  //////////////////////////////////////////////////////////////////////////
  wxMenu *menuEdit = new wxMenu;
  menuEdit->Append(wxID_UNDO, "Undo\tCtrl-Z", "");
  menuEdit->Append(wxID_REDO, "Redo\tCtrl-Shift-Z", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_CUT, "Cut\tCtrl-X", "");
  menuEdit->Append(wxID_COPY, "Copy\tCtrl-C", "");
  menuEdit->Append(wxID_PASTE, "Paste\tCtrl-V", "");
  menuEdit->Append(wxID_DUPLICATE, "Duplicate selection or line\tCtrl-D", "");
  menuEdit->Append(wxID_DELETE, "Delete\tDel", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_FIND, "Find\tCtrl-F", "");
  // menuEdit->Append(wxID_OPEN, "Find next\tF3", "");
  menuEdit->Append(wxID_REPLACE, "Replace\tCtrl-H", "");
  // menuEdit->Append(wxID_OPEN, "Replace again\tShift-F4", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(ID_EDIT_MATCH_BRACE, "Match brace\tCtrl-M", "");
  menuEdit->Append(ID_EDIT_SELECT_BRACE, "Select to matching brace\tCtrl-Shift-M", "");
  menuEdit->Append(ID_EDIT_GOTO_LINE, "Goto line\tCtrl-G", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_INDENT, "Increase indent\tTab", "");
  menuEdit->Append(wxID_UNINDENT, "Reduce indent\tShift-Tab", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_SELECTALL, "Select all\tCtrl-A", "");
  menuEdit->Append(ID_EDIT_SELECT_LINE, "Select line\tCtrl-L", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(ID_EDIT_TOUPPER, "Make UPPERCASE\tCtrl-Shift-U", "");
  menuEdit->Append(ID_EDIT_TOLOWER, "Make lowercase\tCtrl-U", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(ID_EDIT_MOVELINESUP, "Move selected lines up\tAlt-Up", "");
  menuEdit->Append(ID_EDIT_MOVELINESDOWN, "Move selected lines down\tAlt-Down", "");

  //////////////////////////////////////////////////////////////////////////

  wxMenu *menuProject = new wxMenu;
  menuProject->Append(ID_PROJECT_NEW, "New project...", "Create a new cargo project");
  menuProject->Append(ID_PROJECT_OPEN, "Open project...", "Open a existing cargo or ride project");
  menuProject->Append(ID_PROJECT_SETTINGS, "Project settings...", "Change the ride project settings");
  menuProject->AppendSeparator();
  menuProject->Append(ID_PROJECT_BUILD, "Build\tCtrl-B", "Compile the current project");
  menuProject->Append(ID_PROJECT_CLEAN, "Clean", "Remove the target directory");
  menuProject->Append(ID_PROJECT_REBUILD, "Rebuild\tCtrl-Shift-B", "Clean + Build");
  menuProject->Append(ID_PROJECT_DOC, "Doc", "Build this project's and its dependencies' documentation");
  menuProject->Append(ID_PROJECT_RUN, "Run\tF5", "Build and execute src/main.rs");
  menuProject->Append(ID_PROJECT_TEST, "Test", "Run the tests");
  menuProject->Append(ID_PROJECT_BENCH, "Bench", "Run the benchmarks");
  menuProject->Append(ID_PROJECT_UPDATE, "Update", "Update dependencies listed in Cargo.lock");

  //////////////////////////////////////////////////////////////////////////
  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);

  //////////////////////////////////////////////////////////////////////////
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuEdit, "&Edit");
  menuBar->Append(menuProject, "&Project");
  menuBar->Append(menuHelp, "&Help");
  SetMenuBar(menuBar);
  CreateStatusBar();
  SetStatusText("");
  createNotebook();

  aui.Update();
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

  aui.AddPane(notebook, wxAuiPaneInfo().Name(wxT("notebook_content")).PaneBorder(false).CloseButton(false).Movable(false) );
}

const ride::Settings& MainWindow::getSettings() const {
  return settings;
}

MainWindow::~MainWindow() {
  aui.UnInit();
}

void MainWindow::OnFileExit(wxCommandEvent& event)
{
  Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
  wxMessageBox("Ride is a Rust IDE. It's named after concatenating R from rust and IDE.\nFor more information: https://github.com/madeso/ride", "About Ride", wxOK | wxICON_INFORMATION);
}

void MainWindow::OnFileOpen(wxCommandEvent& event)
{
  wxFileDialog
    openFileDialog(this, _("Open file"), "", "",
    FILE_PATTERN, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE );
  if (openFileDialog.ShowModal() == wxID_CANCEL)
    return;

  wxArrayString paths;
  openFileDialog.GetPaths(paths);
  for (wxArrayString::iterator path = paths.begin(); path != paths.end(); ++path) {
    wxFileName w(*path);
    w.Normalize();
    new FileEdit(notebook, this, "", w.GetFullPath());
  }
}

FileEdit* MainWindow::getSelectedEditorNull() {
  const int selected = notebook->GetSelection();
  if (selected == -1) {
    return NULL;
  }
  wxWindow* window = notebook->GetPage(selected);
  if (window->IsKindOf(CLASSINFO(FileEdit))) {
    FileEdit* edit = reinterpret_cast<FileEdit*>(window);
    return edit;
  }
  else {
    return NULL;
  }
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

  // shutdown protobuf now, to avoid spewing out memory leaks...
  google::protobuf::ShutdownProtobufLibrary();
  evt.Skip();
}

void MainWindow::setSettings(const ride::Settings& settings) {
  assert(this);
  this->settings = settings;
  updateAllEdits();
}

void MainWindow::OnNotebookPageClosed(wxAuiNotebookEvent& evt) {
}

void MainWindow::updateAllEdits() {
  for (unsigned int i = 0; i < notebook->GetPageCount(); ++i) {
    wxWindow* window = notebook->GetPage(i);
    if (window->IsKindOf(CLASSINFO(FileEdit))) {
      FileEdit* edit = reinterpret_cast<FileEdit*>(window);
      edit->UpdateTextControl();
    }
  }
}

void MainWindow::OnFileShowSettings(wxCommandEvent& event) {
  SettingsDlg settingsdlg(this, this);
  settingsdlg.ShowModal();
}

void MainWindow::OnFileSave(wxCommandEvent& event) {
  FileEdit* selected = getSelectedEditorNull();
  if (selected == NULL) return;
  selected->Save();
}

void MainWindow::OnFileSaveAs(wxCommandEvent& event) {
  FileEdit* selected = getSelectedEditorNull();
  if (selected == NULL) return;
  selected->SaveAs();
}

//////////////////////////////////////////////////////////////////////////
#define MEM_FUN(X) \
  void MainWindow::OnEdit ## X(wxCommandEvent& event) {\
    FileEdit* selected = getSelectedEditorNull();\
    if (selected == NULL) return;\
    selected->X();\
  }

MEM_FUN(Undo)
MEM_FUN(Redo)
MEM_FUN(Cut)
MEM_FUN(Copy)
MEM_FUN(Paste)
MEM_FUN(Duplicate)
MEM_FUN(Delete)
MEM_FUN(Find)
MEM_FUN(Replace)
MEM_FUN(MatchBrace)
MEM_FUN(SelectBrace)
MEM_FUN(GotoLine)
MEM_FUN(Indent)
MEM_FUN(UnIndent)
MEM_FUN(SelectAll)
MEM_FUN(SelectLine)
MEM_FUN(ToUpper)
MEM_FUN(ToLower)
MEM_FUN(MoveLinesUp)
MEM_FUN(MoveLinesDown)
#undef MEM_FUN
//////////////////////////////////////////////////////////////////////////

void MainWindow::OnProjectNew(wxCommandEvent& event) {
}

void MainWindow::OnProjectOpen(wxCommandEvent& event) {
  wxFileDialog
    openFileDialog(this, _("Open project"), "", "",
    "Cargo files|Cargo.toml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (openFileDialog.ShowModal() == wxID_CANCEL)
    return;
  wxFileName file(openFileDialog.GetPath());
  file.Normalize();
  file.RemoveLastDir();
  project = Project(file.GetPath());
}

#define MEM_FUN(X) \
  void MainWindow::OnProject ## X(wxCommandEvent& event) {\
    project. ## X ();\
  }

MEM_FUN(Settings)
MEM_FUN(Build   )
MEM_FUN(Clean   )
MEM_FUN(Rebuild )
MEM_FUN(Doc     )
MEM_FUN(Run     )
MEM_FUN(Test    )
MEM_FUN(Bench   )
MEM_FUN(Update  )
#undef MEM_FUN