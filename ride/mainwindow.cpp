#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>

#include "ride/mainwindow.h"
#include "ride/fileedit.h"

enum
{
  ID_Hello = 1
};

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(ID_Hello, MainWindow::OnHello)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)

EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainWindow::OnNotebookPageClose)
EVT_AUINOTEBOOK_PAGE_CLOSED(wxID_ANY, MainWindow::OnNotebookPageClosed)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
  aui.SetManagedWindow(this);

  wxMenu *menuFile = new wxMenu;
  menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
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

  new FileEdit(notebook, this, "", "");
  new FileEdit(notebook, this, "hello world", "C:\\hello.txt");
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

MainWindow::~MainWindow() {
  aui.UnInit();
}

void MainWindow::OnExit(wxCommandEvent& event)
{
  Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
  wxMessageBox("This is a wxWidgets' Hello world sample", "About Hello World", wxOK | wxICON_INFORMATION);
}

void MainWindow::OnHello(wxCommandEvent& event)
{
  wxLogMessage("Hello world from wxWidgets!");
}

void MainWindow::OnNotebookPageClose(wxAuiNotebookEvent& evt) {
  wxWindow* window = notebook->GetPage(evt.GetSelection());
  if (window->IsKindOf(CLASSINFO(FileEdit))) {
    FileEdit* edit = reinterpret_cast<FileEdit*>(window);
    if (edit->canClose() == false) {
      evt.Veto();
    }
  }
}

void MainWindow::OnNotebookPageClosed(wxAuiNotebookEvent& evt) {
}
