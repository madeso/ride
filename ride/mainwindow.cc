#include "ride/wx.h"
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/filename.h>
#include <wx/aboutdlg.h>
#include <wx/uri.h>
#include <wx/clipbrd.h>

#include "ride/mainwindow.h"
#include "ride/fileedit.h"

#include "ride/settingsdlg.h"
#include "ride/compilermessage.h"
#include "ride/projectexplorer.h"
#include "ride/createnewprojectdlg.h"
#include "ride/createnewfiledlg.h"

#include "ride/resources/icons.h"
#include "ride/wxutils.h"
#include "ride/runner.h"
#include "ride/quickopendlg.h"

#include "ride/cmdrunner.h"

#include "ride/outputcontrol.h"
#include "ride/findcontrol.h"

#include "ride/tab.h"
#include "ride/startpage.h"

FoundEdit FoundEdit::NOT_FOUND(0, NULL);

Tab* TabFromIndex(wxAuiNotebook* notebook, int tab_index) {
  wxWindow* window = notebook->GetPage(tab_index);
  void* data = window->GetClientData();
  assert(data);
  return reinterpret_cast<Tab*>(data);
}

FileEdit* NotebookFromIndexOrNull(wxAuiNotebook* notebook, int tab_index) {
  Tab* tab = TabFromIndex(notebook, tab_index);
  return tab->ToFileEdit();
}

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
  ID_EDIT_SHOW_PROPERTIES,
  ID_EDIT_OPEN_IN_ONLINE_DOCUMENTATION,
  ID_EDIT_SHOW_AUTOCOMPLETE,

  ID_PROJECT_NEW,
  ID_PROJECT_OPEN,
  ID_PROJECT_SETTINGS,
  ID_PROJECT_BUILD,
  ID_PROJECT_SELECT_ACTIVE_BUILD,
  ID_PROJECT_CLEAN,
  ID_PROJECT_REBUILD,
  ID_PROJECT_DOC,
  ID_PROJECT_RUN,
  ID_PROJECT_TEST,
  ID_PROJECT_BENCH,
  ID_PROJECT_UPDATE,

  ID_FIND_IN_FILES,
  ID_REPLACE_IN_FILES,

  ID_VIEW_RESTORE_WINDOWS,
  ID_VIEW_SAVE_LAYOUT,
  ID_VIEW_LOAD_LAYOUT,
  ID_VIEW_SHOW_FINDRESULT,
  ID_VIEW_SHOW_START,
  ID_VIEW_SHOW_PROJECT,
  ID_VIEW_SHOW_BUILD,
  ID_VIEW_SHOW_COMPILE,

  ID_QUICK_OPEN
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
  EVT_MENU(ID_EDIT_OPEN_IN_ONLINE_DOCUMENTATION, MainWindow::OnEditOpenInOnlineDocumentation)
  EVT_MENU(ID_EDIT_SHOW_PROPERTIES, MainWindow::OnEditShowProperties)
  EVT_MENU(ID_EDIT_SHOW_AUTOCOMPLETE, MainWindow::OnEditShowAutocomplete)
  
  EVT_MENU(ID_PROJECT_NEW                , MainWindow::OnProjectNew              )
  EVT_MENU(ID_PROJECT_OPEN               , MainWindow::OnProjectOpen             )
  EVT_MENU(ID_PROJECT_SETTINGS           , MainWindow::OnProjectSettings         )
  EVT_MENU(ID_PROJECT_BUILD              , MainWindow::OnProjectBuild            )
  EVT_MENU(ID_PROJECT_SELECT_ACTIVE_BUILD, MainWindow::OnProjectSelectActiveBuild)
  EVT_MENU(ID_PROJECT_CLEAN              , MainWindow::OnProjectClean            )
  EVT_MENU(ID_PROJECT_REBUILD            , MainWindow::OnProjectRebuild          )
  EVT_MENU(ID_PROJECT_DOC                , MainWindow::OnProjectDoc              )
  EVT_MENU(ID_PROJECT_RUN                , MainWindow::OnProjectRun              )
  EVT_MENU(ID_PROJECT_TEST               , MainWindow::OnProjectTest             )
  EVT_MENU(ID_PROJECT_BENCH              , MainWindow::OnProjectBench            )
  EVT_MENU(ID_PROJECT_UPDATE             , MainWindow::OnProjectUpdate           )
  EVT_MENU(wxID_NEW                      , MainWindow::OnProjectFileNew          )
  EVT_MENU(ID_QUICK_OPEN                 , MainWindow::OnProjectQuickOpen        )
  EVT_MENU(ID_FIND_IN_FILES              , MainWindow::OnProjectFindInFiles      )
  EVT_MENU(ID_REPLACE_IN_FILES           , MainWindow::OnProjectReplaceInFiles   )
  
  EVT_MENU(ID_VIEW_RESTORE_WINDOWS,MainWindow::OnViewRestoreWindows )
  EVT_MENU(ID_VIEW_SAVE_LAYOUT,    MainWindow::OnViewSaveLayout )
  EVT_MENU(ID_VIEW_LOAD_LAYOUT,    MainWindow::OnViewLoadLayout )
  EVT_MENU(ID_VIEW_SHOW_FINDRESULT,     MainWindow::OnViewShowFindResult )
  EVT_MENU(ID_VIEW_SHOW_START, MainWindow::OnViewShowStart)
  EVT_MENU(ID_VIEW_SHOW_BUILD , MainWindow::OnViewShowBuild)
  EVT_MENU(ID_VIEW_SHOW_COMPILE, MainWindow::OnViewShowCompile)
  EVT_MENU(ID_VIEW_SHOW_PROJECT , MainWindow::OnViewShowProject)
  
  EVT_MENU(wxID_ABOUT             , MainWindow::OnAbout)
  
  EVT_CLOSE(MainWindow::OnClose)
  EVT_ACTIVATE(MainWindow::OnActivated)
  
  EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainWindow::OnNotebookPageClose)
  EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_ANY, MainWindow::OnNotebookPageChanged)

  EVT_MENU_OPEN(MainWindow::OnMenuOpen)
wxEND_EVENT_TABLE()

void MainWindow::OnNotebookPageChanged(wxAuiNotebookEvent& evt) {
  wxString file_name = wxEmptyString;

  FileEdit* selected_file = GetSelectedEditorNull();
  if (selected_file) {
    file_name = selected_file->filename();
    selected_file->SetFocus();
    selected_file->UpdateStatusText();
  }

  project_explorer_->HighlightOpenFile(file_name);
}

const wxString& MainWindow::root_folder() const {
  assert(project_);
  return project_->root_folder();
}

void MainWindow::OnActivated(wxActivateEvent& event) {
  if (event.GetActive()) {
    ReloadFilesIfNeeded();

    // last focus is here since alt-tab seems to really screw with the focusing
    if (last_focus_) {
      last_focus_->SetFocus();
      last_focus_->SetFocusFromKbd();
    }
  }
  else {
    last_focus_ = FindFocus();
  }
  project_explorer_->UpdateFolderStructure();
}

//////////////////////////////////////////////////////////////////////////

const wxString PANE_FIND_1 = "pane_findres1";
const wxString PANE_BUILD = "pane_output";
const wxString PANE_COMPILE = "pane_compile";
const wxString PANE_PROJECT = "pane_project";


struct AddMenuItem {
  wxMenuItem* item;
  operator wxMenuItem*() {
    return item;
  }

  AddMenuItem(wxMenu* menu, int id, const wxString& title = wxEmptyString, const wxString& help = wxEmptyString, const char** xpm = NULL) {
    item = new wxMenuItem(NULL, id, title, help);
    if (xpm) {
      // it's important to set the icon before adding the item
      // otherwise it will silently fail on some wxWidgets versions
      wxBitmap bitmap(xpm, wxBITMAP_TYPE_XPM);
      item->SetBitmap(bitmap);
    }
    menu->Append(item);
  }

  AddMenuItem& Checkable() {
    item->SetCheckable(true);
    return *this;
  }
};

void MainWindow::OnMenuOpen(wxMenuEvent& event) {
  UpdateMenuItemView();
}

const int AUI_OPTIONS = 0
| wxAUI_MGR_ALLOW_FLOATING // Allow a pane to be undocked to take the form of a wxMiniFrame. 
| wxAUI_MGR_ALLOW_ACTIVE_PANE // Change the color of the title bar of the pane when it is activated. 
| wxAUI_MGR_TRANSPARENT_DRAG // Make the pane transparent during its movement. 
| wxAUI_MGR_TRANSPARENT_HINT // The possible location for docking is indicated by a translucent area. 
// | wxAUI_MGR_VENETIAN_BLINDS_HINT // The possible location for docking is indicated by gradually appearing partially transparent hint. 
// | wxAUI_MGR_RECTANGLE_HINT // The possible location for docking is indicated by a rectangular outline. 
// | wxAUI_MGR_HINT_FADE // The translucent area where the pane could be docked appears gradually. 
| wxAUI_MGR_NO_VENETIAN_BLINDS_FADE // Used in complement of wxAUI_MGR_VENETIAN_BLINDS_HINT to show the docking hint immediately. 
| wxAUI_MGR_LIVE_RESIZE // When a docked pane is resized, its content is refreshed in live (instead of moving the border alone and refreshing the content at the end). 
;

void MainWindow::SetStatusText(const wxString& text, StatusBarWidgets widget) {
  assert(statusbar_);
  statusbar_->SetStatusText(text, widget);
}

MainWindow::MainWindow(const wxString& app_name, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, app_name, pos, size)
, aui_(NULL, AUI_OPTIONS)
, findres_window_(NULL)
, app_name_(app_name)
, last_focus_(NULL)
, statusbar_(NULL)
{
  project_.reset(new Project(this, wxEmptyString));
#ifdef _WIN32
  SetIcon(wxICON(aaaaa_logo));
#else
  // todo: set icon
#endif
  aui_.SetManagedWindow(this);

  LoadSettings(this, settings_);

  //////////////////////////////////////////////////////////////////////////
  wxMenu *menu_file = new wxMenu;
  AddMenuItem(menu_file, wxID_OPEN, "&Open...\tCtrl-O", "Open a file");
  AddMenuItem(menu_file, wxID_SAVE, "&Save...\tCtrl-S", "Save the file");
  AddMenuItem(menu_file, wxID_SAVEAS, "Save &as...\tCtrl-Shift-S", "Save the file as a new file");
  menu_file->AppendSeparator();
  AddMenuItem(menu_file, ID_FILE_RIDE_SETTINGS, "S&ettings...", "Change the settings of RIDE", file_settings_xpm);
  menu_file->AppendSeparator();
  AddMenuItem(menu_file, wxID_EXIT, "", "", file_quit_xpm);

  //////////////////////////////////////////////////////////////////////////
  wxMenu *menu_edit = new wxMenu;
  AddMenuItem(menu_edit, wxID_UNDO, "Undo\tCtrl-Z", "", edit_undo_xpm);
  AddMenuItem(menu_edit, wxID_REDO, "Redo\tCtrl-Shift-Z", "", edit_redo_xpm);
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, wxID_CUT, "Cut\tCtrl-X", "", edit_cut_xpm);
  AddMenuItem(menu_edit, wxID_COPY, "Copy\tCtrl-C", "", edit_copy_xpm);
  AddMenuItem(menu_edit, wxID_PASTE, "Paste\tCtrl-V", "", edit_paste_xpm);
  AddMenuItem(menu_edit, wxID_DUPLICATE, "Duplicate selection or line\tCtrl-D", "");
  AddMenuItem(menu_edit, wxID_DELETE, "Delete\tDel", "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, wxID_FIND, "Find\tCtrl-F", "");
  // menu_edit, wxID_OPEN, "Find next\tF3", "");
  AddMenuItem(menu_edit, wxID_REPLACE, "Replace\tCtrl-H", "");
  // menu-edit, wxID_OPEN, "Replace again\tShift-F4", "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_MATCH_BRACE, "Match brace\tCtrl-M", "");
  AddMenuItem(menu_edit, ID_EDIT_SELECT_BRACE, "Select to matching brace\tCtrl-Shift-M", "");
  AddMenuItem(menu_edit, ID_EDIT_GOTO_LINE, "Goto line\tCtrl-G", "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, wxID_INDENT, "Increase indent\tTab", "", edit_tab_add_xpm);
  AddMenuItem(menu_edit, wxID_UNINDENT, "Reduce indent\tShift-Tab", "", edit_tab_remove_xpm);
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, wxID_SELECTALL, "Select all\tCtrl-A", "");
  AddMenuItem(menu_edit, ID_EDIT_SELECT_LINE, "Select line\tCtrl-L", "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_TOUPPER, "Make UPPERCASE\tCtrl-Shift-U", "", edit_make_uppercase_xpm);
  AddMenuItem(menu_edit, ID_EDIT_TOLOWER, "Make lowercase\tCtrl-U", "", edit_make_lowercase_xpm);
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_MOVELINESUP, "Move selected lines up\tAlt-Up", "");
  AddMenuItem(menu_edit, ID_EDIT_MOVELINESDOWN, "Move selected lines down\tAlt-Down", "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_OPEN_IN_ONLINE_DOCUMENTATION, "Open type in online documentation\tCtrl-'", ""); // todo: get a better shortcut
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_SHOW_AUTOCOMPLETE, "Auto complete\tCtrl-Space", "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_SHOW_PROPERTIES, "File properties\tAlt-Enter", "", edit_file_properties_xpm);

  //////////////////////////////////////////////////////////////////////////

  wxMenu *menu_project = new wxMenu;
  AddMenuItem(menu_project, ID_PROJECT_NEW, "New project...", "Create a new cargo project");
  AddMenuItem(menu_project, ID_PROJECT_OPEN, "Open project...\tCtrl-Shift-O", "Open a existing cargo or ride project");
  AddMenuItem(menu_project, ID_PROJECT_SETTINGS, "Project settings...", "Change the ride project settings", project_settings_xpm);
  menu_project->AppendSeparator();
  AddMenuItem(menu_project, ID_PROJECT_BUILD, "Build\tCtrl-B", "Compile the current project", project_build_xpm);
  AddMenuItem(menu_project, ID_PROJECT_SELECT_ACTIVE_BUILD, "Select active build...\tCtrl-Shift-B", "Select to build debug, release etc");
  AddMenuItem(menu_project, ID_PROJECT_CLEAN, "Clean", "Remove the target directory", project_clean_xpm);
  AddMenuItem(menu_project, ID_PROJECT_REBUILD, "Rebuild", "Clean + Build", project_rebuild_xpm);
  AddMenuItem(menu_project, ID_PROJECT_DOC, "Doc", "Build this project's and its dependencies' documentation", project_doc_xpm);
  AddMenuItem(menu_project, ID_PROJECT_RUN, "Run\tF5", "Build and execute src/main.rs", project_run_xpm);
  AddMenuItem(menu_project, ID_PROJECT_TEST, "Test", "Run the tests", project_test_xpm);
  AddMenuItem(menu_project, ID_PROJECT_BENCH, "Bench", "Run the benchmarks", project_bench_xpm);
  AddMenuItem(menu_project, ID_PROJECT_UPDATE, "Update", "Update dependencies listed in Cargo.lock", project_update_xpm);
  menu_project->AppendSeparator();
  AddMenuItem(menu_project, wxID_NEW, "", "", file_normal_xpm);
  AddMenuItem(menu_project, ID_QUICK_OPEN, "Open file in project...\tShift-Alt-O", "Quickly open a file from the project");
  AddMenuItem(menu_project, ID_FIND_IN_FILES, "Find in files...\tCtrl-Shift-F", "Find in files");
  AddMenuItem(menu_project, ID_REPLACE_IN_FILES, "Replace in files...\tCtrl-Shift-H", "Replace in files");

  //////////////////////////////////////////////////////////////////////////

  wxMenu *menu_view = new wxMenu;
  AddMenuItem(menu_view, ID_VIEW_RESTORE_WINDOWS, "Restore window layout", "");
  AddMenuItem(menu_view, ID_VIEW_SAVE_LAYOUT, "Save layout", "");
  AddMenuItem(menu_view, ID_VIEW_LOAD_LAYOUT, "Load layout", "");
  menu_view->AppendSeparator();

  // shortcuts stolen from qt creator: https://wiki.qt.io/Qt_Creator_Keyboard_Shortcuts
  menuItemViewProject_ = AddMenuItem(menu_view, ID_VIEW_SHOW_PROJECT, "&Project pane\tAlt-0", "").Checkable();
  AddMenuItem(menu_view, ID_VIEW_SHOW_START, "&Start page", "");
  // build issues
  menuItemViewFind_ = AddMenuItem(menu_view, ID_VIEW_SHOW_FINDRESULT, "Find &result pane\tAlt-2", "").Checkable();
  // app output
  menuItemViewBuild_ = AddMenuItem(menu_view, ID_VIEW_SHOW_BUILD, "&Build pane\tAlt-4", "").Checkable();
  menuItemViewCompile_ = AddMenuItem(menu_view, ID_VIEW_SHOW_COMPILE, "&Compile pane", "").Checkable();

  //////////////////////////////////////////////////////////////////////////
  wxMenu *menu_help = new wxMenu;
  menu_help->Append(wxID_ABOUT);

  //////////////////////////////////////////////////////////////////////////
  wxMenuBar *menu_bar = new wxMenuBar;
  menu_bar->Append(menu_file, "&File");
  menu_bar->Append(menu_edit, "&Edit");
  menu_bar->Append(menu_view, "&View");
  menu_bar->Append(menu_project, "&Project");
  menu_bar->Append(menu_help, "&Help");
  SetMenuBar(menu_bar);
  

  statusbar_ = CreateStatusBar(STATUSBAR_MAXCOUNT);
  const int small_width = 50;
  int widths[STATUSBAR_MAXCOUNT] = { 0, };
  widths[STATUSBAR_GENERAL] = -1;
  widths[STATUSBAR_BUILD_CONF] = small_width;
  widths[STATUSBAR_RUN_CONF] = small_width;
  widths[STATUSBAR_LINE] = small_width;
  widths[STATUSBAR_COL] = small_width;
  widths[STATUSBAR_CH] = small_width;
  widths[STATUSBAR_INS] = 22;
  statusbar_->SetStatusWidths(STATUSBAR_MAXCOUNT, widths);

  CreateNotebook();

  // output
  build_output_.Create(this, aui_, PANE_BUILD, "Build");
  compiler_output_.Create(this, aui_, PANE_COMPILE, "Compile");

  findres_window_ = new FindControl(this);
  findres_window_->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxHSCROLL);
  findres_window_->UpdateStyle();
  findres_window_->UpdateStyle();
  aui_.AddPane(findres_window_, wxAuiPaneInfo().Name(PANE_FIND_1).Caption("Find result").Bottom().CloseButton(true));

  // project explorer
  project_explorer_ = new ProjectExplorer(this);
  aui_.AddPane(project_explorer_, wxAuiPaneInfo().Name(PANE_PROJECT).Caption("Project").Left().CloseButton(true));

  new StartPageTab(notebook_, this);

  aui_.Update();
  UpdateTitle();

  windows_locations_ = aui_.SavePerspective();

  RestoreSession();
  UpdateMenuItemView();

  project_->SetMainStatusbarText();
}

void MainWindow::OnViewRestoreWindows(wxCommandEvent& event){
  aui_.LoadPerspective(windows_locations_);
}

void MainWindow::OnViewSaveLayout(wxCommandEvent& event){
  // todo...
}

void MainWindow::OnViewLoadLayout(wxCommandEvent& event){
  // todo...
}

void ShowHideAui(wxAuiManager* aui, const wxString& name) {
  assert(aui);
  wxAuiPaneInfo& pane = aui->GetPane(name);
  assert(pane.IsValid() && "This function should only take valid pane names!");
  if (pane.IsShown()) {
    pane.Hide();
  }
  else {
    pane.Show();
  }
  aui->Update();
}

void UpdateMenuItemBasedOnPane(wxAuiManager* aui, wxMenuItem* item, const wxString& name) {
  assert(aui);
  wxAuiPaneInfo& pane = aui->GetPane(name);
  assert(pane.IsValid() && "This function should only take valid pane names!");
  item->Check(pane.IsShown());
}

void ShowPane(wxAuiManager* aui, const wxString& name) {
  assert(aui);
  wxAuiPaneInfo& pane = aui->GetPane(name);
  assert(pane.IsValid() && "This function should only take valid pane names!");
  if (false == pane.IsShown()) {
    pane.Show();
    aui->Update();
  }
}

void MainWindow::UpdateMenuItemView() {
  UpdateMenuItemBasedOnPane(&aui_, menuItemViewFind_, PANE_FIND_1);
  UpdateMenuItemBasedOnPane(&aui_, menuItemViewProject_, PANE_PROJECT);
  UpdateMenuItemBasedOnPane(&aui_, menuItemViewBuild_, PANE_BUILD);
  UpdateMenuItemBasedOnPane(&aui_, menuItemViewCompile_, PANE_COMPILE);
}

void MainWindow::ShowFindWindow() {
  ShowPane(&aui_, PANE_FIND_1);
  aui_.Update();
}

void MainWindow::ShowBuildWindow() {
  ShowPane(&aui_, PANE_BUILD);
  aui_.Update();
}

void MainWindow::ShowCompileWindow() {
  ShowPane(&aui_, PANE_COMPILE);
  aui_.Update();
}

void MainWindow::OnViewShowStart(wxCommandEvent& event){
  // TODO
  wxMessageBox("IMPLEMENT ME");
}

void MainWindow::OnViewShowFindResult(wxCommandEvent& event){
  ShowHideAui(&aui_, PANE_FIND_1);
  UpdateMenuItemView();
}

void MainWindow::OnViewShowBuild(wxCommandEvent& event) {
  ShowHideAui(&aui_, PANE_BUILD);
  UpdateMenuItemView();
}

void MainWindow::OnViewShowCompile(wxCommandEvent& event) {
  ShowHideAui(&aui_, PANE_COMPILE);
  UpdateMenuItemView();
}

void MainWindow::OnViewShowProject(wxCommandEvent& event) {
  ShowHideAui(&aui_, PANE_PROJECT);
  UpdateMenuItemView();
}


void CreateNewFile(const wxString& project_root, MainWindow* main, ProjectExplorer* project_explorer) {
  if (project_root == wxEmptyString) {
    ShowError(main, "Unable to create file, no project open.", "Unable to create");
    return;
  }
  CreateNewFileDlg dlg(main, project_root, project_explorer->GetRelativePathOfSelected());
  if (wxOK != dlg.ShowModal()) {
    return;
  }

  {
    wxFile file(dlg.GetFilePath(), wxFile::write);
    file.Write(dlg.GetTemplateSource());
    // file is written here
  }
  project_explorer->UpdateFolderStructure();
  main->OpenFile(dlg.GetFilePath());
}


void MainWindow::OnProjectFileNew(wxCommandEvent& event) {
  CreateNewFile(project_->root_folder(), this, project_explorer_);
}

void MainWindow::OpenCompilerMessage(const CompilerMessage& message) {
  OpenFile(message.file(), message.start_line(), message.start_index(),
    message.end_line(), message.end_index());
}

void MainWindow::CreateNotebook() {
  wxSize client_size = GetClientSize();

  notebook_ = new wxAuiNotebook(this, wxID_ANY,
    wxPoint(client_size.x, client_size.y),
    wxSize(430, 200),
    wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);

  aui_.AddPane(notebook_, wxAuiPaneInfo().Name(wxT("notebook_content")).PaneBorder(false).CloseButton(false).Movable(false).CenterPane());
}

const ride::Settings& MainWindow::settings() const {
  return settings_;
}

MainWindow::~MainWindow() {
  aui_.UnInit();

  // release the currently loaded project, allowing it to save proto files
  // before we kill the proto library below
  project_.reset();

  // shutdown protobuf now, to avoid spewing out memory leaks...
  google::protobuf::ShutdownProtobufLibrary();
}

void MainWindow::ReloadFilesIfNeeded() {
  for (unsigned int i = 0; i < notebook_->GetPageCount(); ++i) {
    FileEdit* edit = NotebookFromIndexOrNull(notebook_, i);
    if (edit) {
      edit->ReloadFileIfNeeded();
    }
  }
}

void MainWindow::OnFileExit(wxCommandEvent& event)
{
  Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
  wxAboutDialogInfo about_info;
  about_info.SetName(app_name_);

  // aboutInfo.SetVersion(RIDE_VERSION_STRING);
  about_info.SetDescription(_("Ride is a Rust IDE. It's named after concatenating R from rust and IDE."));
  // aboutInfo.SetCopyright("(C) 1992-2010");
  about_info.SetWebSite("https://github.com/madeso/ride");
  wxAboutBox(about_info, this);

  // todo: this about box has too much collapsible items, use another advanced about box instead?
}

void MainWindow::OnFileOpen(wxCommandEvent& event)
{
  wxFileDialog open_file(this, _("Open file"), "", "", GetFilePattern(), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE );
  if (open_file.ShowModal() == wxID_CANCEL)
    return;

  wxArrayString paths_to_open;
  open_file.GetPaths(paths_to_open);
  for (wxArrayString::iterator path = paths_to_open.begin(); path != paths_to_open.end(); ++path) {
    OpenFile(*path);
  }
}

FileEdit* MainWindow::AddAllCompilerMessages(FileEdit* file_edit) {
  build_output_.AddAllCompilerMessages(file_edit);
  compiler_output_.AddAllCompilerMessages(file_edit);
  return file_edit;
}

void MainWindow::FileHasBeenRenamed(const wxString& old_path, const wxString& new_path) {
  auto found_edit = GetEditFromFileName(old_path);
  if (false == found_edit) {
    return;
  }
  FileEdit* edit = found_edit.edit;
  assert(edit);
  edit->FileHasBeenRenamed(new_path);
  // todo: update compiler errors to point to new file...
}

FileEdit* MainWindow::OpenFile(const wxString& file, int start_line, int start_index, int end_line, int end_index) {
  wxFileName file_name(file);

  const wxString full_path = file_name.GetFullPath();

  if (false == file_name.Exists()) {
    ShowError(this, wxString::Format("Unable to open '%s'", full_path), "Unable to open file!");
    return NULL;
  }

  FoundEdit found_edit = GetEditFromFileName(full_path);
  if (found_edit) {
    notebook_->SetSelection(found_edit.index);
  };
  FileEdit* found_edit_or_new = found_edit.edit != NULL
    ? found_edit.edit
    : AddAllCompilerMessages(new FileEdit(notebook_, this, full_path))
    ;
  found_edit_or_new->SetSelection(start_line, start_index, end_line, end_index);
  found_edit_or_new->SetFocus();

  return found_edit_or_new;
}

FileEdit* MainWindow::GetFile(const wxString& file) {
  wxFileName file_name(file);
  file_name.Normalize();
  const wxString full_path = file_name.GetFullPath();

  FoundEdit found_edit = GetEditFromFileName(full_path);
  return found_edit.edit;
}

FileEdit* MainWindow::GetSelectedEditorNull() {
  const int selected_tab_index = notebook_->GetSelection();
  if (selected_tab_index == -1) {
    return NULL;
  }
  return NotebookFromIndexOrNull(notebook_, selected_tab_index);
}

void MainWindow::OnNotebookPageClose(wxAuiNotebookEvent& evt) {
  FileEdit* edit = NotebookFromIndexOrNull(notebook_, evt.GetSelection());
  if (edit) {
    if (edit->CanClose(true) == false) {
      evt.Veto();
    }
  }
}

FoundEdit MainWindow::GetEditFromFileName(const wxString& file) {
  for (unsigned int tab_index = 0; tab_index < notebook_->GetPageCount(); ++tab_index) {
    FileEdit* edit = NotebookFromIndexOrNull(notebook_, tab_index);
    if (edit) {
      if (edit->filename() == file) {
        return FoundEdit(tab_index, edit);
      }
    }
  }

  return FoundEdit::NOT_FOUND;
}

OutputDirector& MainWindow::build_output() {
  ShowBuildWindow();
  return build_output_;
}

OutputDirector& MainWindow::compiler_output() {
  ShowCompileWindow();
  return compiler_output_;
}

Project* MainWindow::project() {
  assert(project_.get());
  return project_.get();
}

void MainWindow::OnClose(wxCloseEvent& evt) {
  for (unsigned int tab_index = 0; tab_index < notebook_->GetPageCount(); ++tab_index) {
    FileEdit* edit = NotebookFromIndexOrNull(notebook_, tab_index);
    if (edit) {
      const bool canAbort = evt.CanVeto();
      if (edit->CanClose(canAbort) == false) {
        evt.Veto();
        return;
      }
    }
  }

  SaveSession();

  evt.Skip();
}

void MainWindow::set_settings(const ride::Settings& settings) {
  assert(this);
  this->settings_ = settings;
  UpdateAllEdits();
  project_explorer_->UpdateColors();
}

void MainWindow::UpdateAllEdits() {
  for (unsigned int tab_index = 0; tab_index < notebook_->GetPageCount(); ++tab_index) {
    FileEdit* edit = NotebookFromIndexOrNull(notebook_, tab_index);
    if (edit) {
      edit->UpdateTextControl();
    }
  }
  findres_window_->UpdateStyle();
  build_output_.UpdateStyles();
  compiler_output_.UpdateStyles();
}

void MainWindow::OnFileShowSettings(wxCommandEvent& event) {
  SettingsDlg settings_dialog(this, this);
  settings_dialog.ShowModal();
}

void MainWindow::OnFileSave(wxCommandEvent& event) {
  FileEdit* selected_edit = GetSelectedEditorNull();
  if (selected_edit == NULL) return;
  selected_edit->Save();
}

void MainWindow::OnFileSaveAs(wxCommandEvent& event) {
  FileEdit* selected_edit = GetSelectedEditorNull();
  if (selected_edit == NULL) return;
  selected_edit->SaveAs();
}

void MainWindow::OnEditFind(wxCommandEvent& event) {
  FileEdit* selected_edit = GetSelectedEditorNull();
  if (selected_edit == NULL) return;
  ShowFindWindow();
  selected_edit->Find(findres_window_, project_->root_folder());
}

void MainWindow::OnEditReplace(wxCommandEvent& event) {
  FileEdit* selected_edit = GetSelectedEditorNull();
  if (selected_edit == NULL) return;
  ShowFindWindow();
  selected_edit->Replace(findres_window_, project_->root_folder());
}

void MainWindow::OnProjectFindInFiles(wxCommandEvent& event) {
  FileEdit* selected_edit = GetSelectedEditorNull();
  if (selected_edit == NULL) return;
  ShowFindWindow();
  selected_edit->FindInFiles(findres_window_, project_->root_folder());
}

void MainWindow::OnProjectReplaceInFiles(wxCommandEvent& event) {
  FileEdit* selected_edit = GetSelectedEditorNull();
  if (selected_edit == NULL) return;
  ShowFindWindow();
  selected_edit->ReplaceInFiles(findres_window_, project_->root_folder());
}

//////////////////////////////////////////////////////////////////////////
#define MEM_FUN(X) \
  void MainWindow::OnEdit ## X(wxCommandEvent& event) {\
    FileEdit* selected_edit = GetSelectedEditorNull();\
    if (selected_edit == NULL) return;\
    selected_edit->X();\
  }

MEM_FUN(Undo)
MEM_FUN(Redo)
MEM_FUN(Cut)
MEM_FUN(Copy)
MEM_FUN(Paste)
MEM_FUN(Duplicate)
MEM_FUN(Delete)
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
MEM_FUN(OpenInOnlineDocumentation)
MEM_FUN(ShowProperties)
MEM_FUN(ShowAutocomplete)
#undef MEM_FUN
//////////////////////////////////////////////////////////////////////////

void MainWindow::UpdateTitle() {
  const wxString new_title = project_->root_folder().IsEmpty()
    ? app_name_
    // todo: only display project folder name instead of the whole path?
    : wxString::Format("%s - %s", project_->root_folder(), app_name_);
  this->SetTitle(new_title);
}

void MainWindow::OnProjectNew(wxCommandEvent& event) {
  // todo: implement creation of new project
  CreateNewProjectDlg dlg(this);
  if (wxID_OK != dlg.ShowModal()) {
    return;
  }
  // run cargo new

  wxString output;
  if (CmdRunner::Run(dlg.project_folder(), dlg.GenerateCargoCommandline(), &output) == false) {
    ShowError(this, output, "Unable to create project!");
    return;
  }

  // open project
  if (false == OpenProject(dlg.GetTarget())) {
    ShowError(this, "Unable to open cargo project", "Unable to open");
  }

  UpdateTitle();
}

void MainWindow::OnProjectOpen(wxCommandEvent& event) {
  wxFileDialog open_project_dialog(this, _("Open project"), "", "", "Cargo files|Cargo.toml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (open_project_dialog.ShowModal() == wxID_CANCEL)
    return;
  wxFileName cargo_file(open_project_dialog.GetPath());
  cargo_file.Normalize();

  const wxString full_path = cargo_file.GetFullPath();

  if (false == OpenProject(full_path)) {
    ShowError(this, "You didn't select a proper cargo file", "No cargo file");
  }
}

bool MainWindow::OpenProject(const wxString full_path) {
  const bool is_really_a_cargo_file = full_path.Lower().EndsWith("cargo.toml");
  if (is_really_a_cargo_file == false) {
    return false;
  }

  wxFileName cargo_file(full_path);

  if (false == cargo_file.Exists()) return false;

  // don't load the cargo file, load the whole folder instead as cargo files should be named in a specific way!
  const wxString project_folder = cargo_file.GetPathWithSep();
  project_.reset(new Project(this, project_folder));
  project_explorer_->SetFolder(project_folder);
  UpdateTitle();
  project_->SetMainStatusbarText();
  return true;
}

void MainWindow::SaveAllChangedProjectFiles() {
  for (unsigned int i = 0; i < notebook_->GetPageCount(); ++i) {
    FileEdit* edit = NotebookFromIndexOrNull(notebook_, i);
    if (edit) {
      if (project_->IsPartOfProject(edit->filename())) {
        edit->Save();
      }
    }
  }
}

void MainWindow::OnProjectQuickOpen(wxCommandEvent& event) {
  std::vector<wxString> selected;
  if (false == ShowQuickOpenDlg(this, project_->root_folder(), project_explorer_->GetFiles(), &selected)) {
    return;
  }
  for (const auto& file : selected) {
    OpenFile(file);
  }
}

#define MEM_FUN(X) \
  void MainWindow::OnProject ## X(wxCommandEvent& event) {\
    ShowBuildWindow();\
    project_-> X ();\
  }

MEM_FUN(Settings)
MEM_FUN(Build            )
MEM_FUN(SelectActiveBuild)
MEM_FUN(Clean            )
MEM_FUN(Rebuild          )
MEM_FUN(Doc              )
MEM_FUN(Run              )
MEM_FUN(Test             )
MEM_FUN(Bench            )
MEM_FUN(Update           )
#undef MEM_FUN

ride::WindowState GetState(wxFrame* main) {
  if (main->IsMaximized()) return ride::WINDOWSTATE_MAXIMIZED;
  else if (main->IsIconized()) return ride::WINDOWSTATE_ICONIZED;
  else return ride::WINDOWSTATE_NORMAL;
}

void MainWindow::SaveSession() {
  ride::Session session;
  const ride::WindowState state = GetState(this);
  if (state != ride::WINDOWSTATE_NORMAL) {
    Restore();
  }
  const wxPoint pos = GetPosition();
  const wxSize size = GetSize();
  const wxString perspective = aui_.SavePerspective();

  session.set_window_x(pos.x);
  session.set_window_y(pos.y);
  session.set_window_width(size.x);
  session.set_window_height(size.y);
  session.set_state(state);
  session.set_project(project_->GetCargoFile());

  session.set_aui_perspective(perspective);

  for (unsigned int tab_index = 0; tab_index < notebook_->GetPageCount(); ++tab_index) {
    FileEdit* edit = NotebookFromIndexOrNull(notebook_, tab_index);
    if (edit) {
      int start_line = 0;
      int start_index = 0; 
      int end_line = 0; 
      int end_index = 0;
      edit->GetSelection(&start_line, &start_index, &end_line, &end_index);
      auto* f = session.mutable_files()->Add();
      f->set_path(edit->filename());
      f->set_start_line ( start_line );
      f->set_start_index( start_index);
      f->set_end_line   ( end_line   );
      f->set_end_index  ( end_index  );
    }
  }

  ::SaveSession(this, session);
}

void MainWindow::RestoreSession() {
  ride::Session session;
  ::LoadSession(this, session);
  if (session.has_window_x()) {
    // if we have set the window x, we assume we have set them all
    SetSize(session.window_x(), session.window_y(), session.window_width(), session.window_height());
  }

  if (session.state() == ride::WINDOWSTATE_MAXIMIZED) Maximize();

  // if we quit in a iconized/minimized state... should we restore to the same state
  // or to the normal state...?
  else if (session.state() == ride::WINDOWSTATE_ICONIZED) Iconize();

  wxString cargo_file = session.project();
  if (cargo_file.IsEmpty() == false) {
    OpenProject(cargo_file);
  }
  
  for (auto f : session.files()) {
    OpenFile(f.path(), f.start_line(), f.start_index(), f.end_line(), f.end_index());
  }

  if (session.has_aui_perspective()) {
    aui_.LoadPerspective(session.aui_perspective());
  }
}
