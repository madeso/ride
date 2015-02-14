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
  ID_SHOW_SETTINGS,

  ID_MATCH_BRACE, ID_SELECT_BRACE, ID_GOTO_LINE, ID_SELECT_LINE,
  ID_TOUPPER, ID_TOLOWER
};

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(wxID_OPEN, MainWindow::OnOpen)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_MENU(ID_SHOW_SETTINGS, MainWindow::ShowSettings)

EVT_MENU(wxID_SAVE, MainWindow::OnSave)
EVT_MENU(wxID_SAVEAS, MainWindow::OnSaveAs)

EVT_MENU(wxID_UNDO, MainWindow::OnUndo)
EVT_MENU(wxID_REDO, MainWindow::OnRedo)
EVT_MENU(wxID_CUT, MainWindow::OnCut)
EVT_MENU(wxID_COPY, MainWindow::OnCopy)
EVT_MENU(wxID_PASTE, MainWindow::OnPaste)
EVT_MENU(wxID_DUPLICATE, MainWindow::OnDuplicate)
EVT_MENU(wxID_DELETE, MainWindow::OnDelete)
EVT_MENU(wxID_FIND, MainWindow::OnFind)
EVT_MENU(wxID_REPLACE, MainWindow::OnReplace)
EVT_MENU(ID_MATCH_BRACE, MainWindow::OnMatchBrace)
EVT_MENU(ID_SELECT_BRACE, MainWindow::OnSelectBrace)
EVT_MENU(ID_GOTO_LINE, MainWindow::OnGotoLine)
EVT_MENU(wxID_INDENT, MainWindow::OnIndent)
EVT_MENU(wxID_UNINDENT, MainWindow::OnUnIndent)
EVT_MENU(wxID_SELECTALL, MainWindow::OnSelectAll)
EVT_MENU(ID_SELECT_LINE, MainWindow::OnSelectLine)
EVT_MENU(ID_TOUPPER, MainWindow::OnToUpper)
EVT_MENU(ID_TOLOWER, MainWindow::OnToLower)

EVT_CLOSE(MainWindow::OnClose)

EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainWindow::OnNotebookPageClose)
EVT_AUINOTEBOOK_PAGE_CLOSED(wxID_ANY, MainWindow::OnNotebookPageClosed)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
  aui.SetManagedWindow(this);

  LoadSettings(settings);

  //////////////////////////////////////////////////////////////////////////
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open a file");
  menuFile->Append(wxID_SAVE, "&Save...\tCtrl-S", "Save the file");
  menuFile->Append(wxID_SAVEAS, "Save &as...\tCtrl-Shift-S", "Save the file as a new file");
  menuFile->AppendSeparator();
  menuFile->Append(ID_SHOW_SETTINGS, "S&ettings...", "Change the settings of RIDE");
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
  menuEdit->Append(wxID_DUPLICATE, "Duplicate line\tCtrl-D", "");
  menuEdit->Append(wxID_DELETE, "Delete\tDel", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_FIND, "Find\tCtrl-F", "");
  // menuEdit->Append(wxID_OPEN, "Find next\tF3", "");
  menuEdit->Append(wxID_REPLACE, "Replace\tCtrl-H", "");
  // menuEdit->Append(wxID_OPEN, "Replace again\tShift-F4", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(ID_MATCH_BRACE, "Match brace\tCtrl-M", "");
  menuEdit->Append(ID_SELECT_BRACE, "Select to matching brace\tCtrl-Shift-M", "");
  menuEdit->Append(ID_GOTO_LINE, "Goto line\tCtrl-G", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_INDENT, "Increase indent\tTab", "");
  menuEdit->Append(wxID_UNINDENT, "Reduce indent\tShift-Tab", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(wxID_SELECTALL, "Select all\tCtrl-A", "");
  menuEdit->Append(ID_SELECT_LINE, "Select line\tCtrl-L", "");
  menuEdit->AppendSeparator();
  menuEdit->Append(ID_TOUPPER, "TO UPPERCASE\tCtrl-Alt-U", "");
  menuEdit->Append(ID_TOLOWER, "to lowercase\tCtrl-Alt-L", "");

  //////////////////////////////////////////////////////////////////////////
  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);

  //////////////////////////////////////////////////////////////////////////
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuEdit, "&Edit");
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

void MainWindow::ShowSettings(wxCommandEvent& event) {
  SettingsDlg settingsdlg(this, this);
  settingsdlg.ShowModal();
}

#define MEM_FUN(X) \
  void MainWindow::On ## X(wxCommandEvent& event) {\
    FileEdit* selected = getSelectedEditorNull();\
    if (selected == NULL) return;\
    selected->X();\
  }

MEM_FUN(Save)
MEM_FUN(SaveAs)
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

#undef MEM_FUN
