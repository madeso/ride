// Copyright (2015) Gustav

#include "ride/mainwindow.h"

#include <ride/wx.h>

#include <wx/aboutdlg.h>
#include <wx/clipbrd.h>
#include <wx/filename.h>
#include <wx/uri.h>
#include <wx/aui/aui.h>
#include <wx/stc/stc.h>

#include <vector>
#include <map>

#include "ride/resources/icons.h"

#include "ride/cmdrunner.h"
#include "ride/compilermessage.h"
#include "ride/createnewfiledlg.h"
#include "ride/createnewprojectdlg.h"
#include "ride/fileedit.h"
#include "ride/findresultscontrol.h"
#include "ride/outputcontrol.h"
#include "ride/projectexplorer.h"
#include "ride/quickopendlg.h"
#include "ride/runner.h"
#include "ride/settingsdlg.h"
#include "ride/startpage.h"
#include "ride/tab.h"
#include "ride/wxutils.h"
#include "ride/auix.h"
#include "ride/switcherdlg.h"

FoundEdit FoundEdit::NOT_FOUND(0, NULL);

bool operator==(const OpenDocument& lhs, const OpenDocument& rhs) {
  return lhs.id == rhs.id && lhs.path == rhs.path;
}

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

Tab* GetSelectedTabOrNull(wxAuiNotebook* notebook) {
  const int selected_tab_index = notebook->GetSelection();
  if (selected_tab_index == -1) {
    return NULL;
  }
  return TabFromIndex(notebook, selected_tab_index);
}

OpenDocument OpenDocumentFromTab(Tab* tab) {
  StartPageTab* start = tab->ToStartPage();
  if (start) {
    return OpenDocument("start", "", "");
  }

  FileEdit* edit = tab->ToFileEdit();
  if (edit) {
    return OpenDocument(edit->filename(), edit->filename(),
                        edit->GetLanguageName());
  }

  return OpenDocument("", "", "");
}

enum {
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
  ID_PROJECT_SELECT_ACTIVE_RUN,
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

  ID_TAB_NEXT,
  ID_TAB_PREV,

  ID_QUICK_OPEN
};

void MainWindow::BindEvents() {
  Bind(wxEVT_MENU, &MainWindow::OnFileOpen, this, wxID_OPEN);
  Bind(wxEVT_MENU, &MainWindow::OnFileExit, this, wxID_EXIT);
  Bind(wxEVT_MENU, &MainWindow::OnFileShowSettings, this,
       ID_FILE_RIDE_SETTINGS);
  Bind(wxEVT_MENU, &MainWindow::OnFileSave, this, wxID_SAVE);
  Bind(wxEVT_MENU, &MainWindow::OnFileSaveAs, this, wxID_SAVEAS);
  Bind(wxEVT_MENU, &MainWindow::OnEditUndo, this, wxID_UNDO);
  Bind(wxEVT_MENU, &MainWindow::OnEditRedo, this, wxID_REDO);
  Bind(wxEVT_MENU, &MainWindow::OnEditCut, this, wxID_CUT);
  Bind(wxEVT_MENU, &MainWindow::OnEditCopy, this, wxID_COPY);
  Bind(wxEVT_MENU, &MainWindow::OnEditPaste, this, wxID_PASTE);
  Bind(wxEVT_MENU, &MainWindow::OnEditDuplicate, this, wxID_DUPLICATE);
  Bind(wxEVT_MENU, &MainWindow::OnEditDelete, this, wxID_DELETE);
  Bind(wxEVT_MENU, &MainWindow::OnEditFind, this, wxID_FIND);
  Bind(wxEVT_MENU, &MainWindow::OnEditReplace, this, wxID_REPLACE);
  Bind(wxEVT_MENU, &MainWindow::OnEditMatchBrace, this, ID_EDIT_MATCH_BRACE);
  Bind(wxEVT_MENU, &MainWindow::OnEditSelectBrace, this, ID_EDIT_SELECT_BRACE);
  Bind(wxEVT_MENU, &MainWindow::OnEditGotoLine, this, ID_EDIT_GOTO_LINE);
  Bind(wxEVT_MENU, &MainWindow::OnEditIndent, this, wxID_INDENT);
  Bind(wxEVT_MENU, &MainWindow::OnEditUnIndent, this, wxID_UNINDENT);
  Bind(wxEVT_MENU, &MainWindow::OnEditSelectAll, this, wxID_SELECTALL);
  Bind(wxEVT_MENU, &MainWindow::OnEditSelectLine, this, ID_EDIT_SELECT_LINE);
  Bind(wxEVT_MENU, &MainWindow::OnEditToUpper, this, ID_EDIT_TOUPPER);
  Bind(wxEVT_MENU, &MainWindow::OnEditToLower, this, ID_EDIT_TOLOWER);
  Bind(wxEVT_MENU, &MainWindow::OnEditMoveLinesUp, this, ID_EDIT_MOVELINESUP);
  Bind(wxEVT_MENU, &MainWindow::OnEditMoveLinesDown, this,
       ID_EDIT_MOVELINESDOWN);
  Bind(wxEVT_MENU, &MainWindow::OnEditOpenInOnlineDocumentation, this,
       ID_EDIT_OPEN_IN_ONLINE_DOCUMENTATION);
  Bind(wxEVT_MENU, &MainWindow::OnEditShowProperties, this,
       ID_EDIT_SHOW_PROPERTIES);
  Bind(wxEVT_MENU, &MainWindow::OnEditShowAutocomplete, this,
       ID_EDIT_SHOW_AUTOCOMPLETE);
  Bind(wxEVT_MENU, &MainWindow::OnProjectNew, this, ID_PROJECT_NEW);
  Bind(wxEVT_MENU, &MainWindow::OnProjectOpen, this, ID_PROJECT_OPEN);
  Bind(wxEVT_MENU, &MainWindow::OnProjectSettings, this, ID_PROJECT_SETTINGS);
  Bind(wxEVT_MENU, &MainWindow::OnProjectBuild, this, ID_PROJECT_BUILD);
  Bind(wxEVT_MENU, &MainWindow::OnProjectSelectActiveBuild, this,
       ID_PROJECT_SELECT_ACTIVE_BUILD);
  Bind(wxEVT_MENU, &MainWindow::OnProjectClean, this, ID_PROJECT_CLEAN);
  Bind(wxEVT_MENU, &MainWindow::OnProjectRebuild, this, ID_PROJECT_REBUILD);
  Bind(wxEVT_MENU, &MainWindow::OnProjectDoc, this, ID_PROJECT_DOC);
  Bind(wxEVT_MENU, &MainWindow::OnProjectRun, this, ID_PROJECT_RUN);
  Bind(wxEVT_MENU, &MainWindow::OnProjectSelectActiveRun, this,
       ID_PROJECT_SELECT_ACTIVE_RUN);
  Bind(wxEVT_MENU, &MainWindow::OnProjectTest, this, ID_PROJECT_TEST);
  Bind(wxEVT_MENU, &MainWindow::OnProjectBench, this, ID_PROJECT_BENCH);
  Bind(wxEVT_MENU, &MainWindow::OnProjectUpdate, this, ID_PROJECT_UPDATE);
  Bind(wxEVT_MENU, &MainWindow::OnProjectFileNew, this, wxID_NEW);
  Bind(wxEVT_MENU, &MainWindow::OnProjectQuickOpen, this, ID_QUICK_OPEN);
  Bind(wxEVT_MENU, &MainWindow::OnProjectFindInFiles, this, ID_FIND_IN_FILES);
  Bind(wxEVT_MENU, &MainWindow::OnProjectReplaceInFiles, this,
       ID_REPLACE_IN_FILES);
  Bind(wxEVT_MENU, &MainWindow::OnViewRestoreWindows, this,
       ID_VIEW_RESTORE_WINDOWS);
  Bind(wxEVT_MENU, &MainWindow::OnViewSaveLayout, this, ID_VIEW_SAVE_LAYOUT);
  Bind(wxEVT_MENU, &MainWindow::OnViewLoadLayout, this, ID_VIEW_LOAD_LAYOUT);
  Bind(wxEVT_MENU, &MainWindow::OnViewShowFindResult, this,
       ID_VIEW_SHOW_FINDRESULT);
  Bind(wxEVT_MENU, &MainWindow::OnViewShowStart, this, ID_VIEW_SHOW_START);
  Bind(wxEVT_MENU, &MainWindow::OnViewShowBuild, this, ID_VIEW_SHOW_BUILD);
  Bind(wxEVT_MENU, &MainWindow::OnViewShowCompile, this, ID_VIEW_SHOW_COMPILE);
  Bind(wxEVT_MENU, &MainWindow::OnViewShowProject, this, ID_VIEW_SHOW_PROJECT);

  Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);

  Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnClose, this);
  Bind(wxEVT_ACTIVATE, &MainWindow::OnActivated, this);
  Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &MainWindow::OnNotebookPageClose, this);
  Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED, &MainWindow::OnNotebookPageChanged,
       this);
  Bind(wxEVT_MENU_OPEN, &MainWindow::OnMenuOpen, this);

  Bind(wxEVT_NAVIGATION_KEY, &MainWindow::OnNotebookNavigation, this);
  Bind(wxEVT_MENU, &MainWindow::OnTabNext, this, ID_TAB_NEXT);
  Bind(wxEVT_MENU, &MainWindow::OnTabPrev, this, ID_TAB_PREV);

  std::vector<wxAcceleratorEntry> entries;
  entries.push_back(wxAcceleratorEntry(wxACCEL_RAW_CTRL, WXK_TAB, ID_TAB_NEXT));
  entries.push_back(wxAcceleratorEntry(wxACCEL_RAW_CTRL | wxACCEL_SHIFT,
                                       WXK_TAB, ID_TAB_PREV));
  wxAcceleratorTable acc(entries.size(), &entries[0]);
  SetAcceleratorTable(acc);
}

void MainWindow::OnNotebookPageChanged(wxAuiNotebookEvent& event) {
  wxString file_name = wxEmptyString;

  Tab* tab = GetSelectedTabOrNull(notebook_);

  FileEdit* selected_file = GetSelectedEditorNull();
  if (selected_file) {
    file_name = selected_file->filename();
    selected_file->SetFocus();
    selected_file->UpdateStatusText();
  }

  if (tab) {
    OpenDocument file = OpenDocumentFromTab(tab);
    auto found = std::find(mru_.begin(), mru_.end(), file);
    if (found != mru_.end()) {
      mru_.erase(found);
    }
    mru_.push_back(file);
  }

  project_explorer_->HighlightOpenFile(file_name);
}

const wxString& MainWindow::root_folder() const {
  assert(project_);
  return project_->root_folder();
}

void MainWindow::OnActivated(wxActivateEvent& event) {
  if (closing_) return;
  if (event.GetActive()) {
    ReloadFilesIfNeeded();

    // this is here since alt-tab seems to really screw with the focusing
    // so let's fix this by auto selecting the active notebook editor
    Tab* tab = GetSelectedTabOrNull(notebook_);
    if (tab) {
      wxControl* focus = tab->ToControl();
      if (focus) {
        focus->SetFocus();
        focus->SetFocusFromKbd();
      }
    }
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
  operator wxMenuItem*() { return item; }

  AddMenuItem(wxMenu* menu, int id, const wxString& title = wxEmptyString,
              const wxString& help = wxEmptyString, const char** xpm = NULL) {
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

void MainWindow::OnMenuOpen(wxMenuEvent& event) { UpdateMenuItemView(); }

const int AUI_OPTIONS =
    0 |
    // Allow a pane to be undocked to take the form of a wxMiniFrame.
    wxAUI_MGR_ALLOW_FLOATING |
    // Change the color of the title bar of the pane when it is activated.
    wxAUI_MGR_ALLOW_ACTIVE_PANE |
    // Make the pane transparent during its movement.
    wxAUI_MGR_TRANSPARENT_DRAG |
    // The possible location for docking is indicated by a translucent area.
    wxAUI_MGR_TRANSPARENT_HINT |
    // The possible location for docking is indicated by gradually appearing
    // partially transparent hint.
    // wxAUI_MGR_VENETIAN_BLINDS_HINT |
    // The possible location for docking is indicated by a rectangular outline.
    // wxAUI_MGR_RECTANGLE_HINT |
    // The translucent area where the pane could be docked appears gradually.
    // wxAUI_MGR_HINT_FADE |
    // Used in complement of wxAUI_MGR_VENETIAN_BLINDS_HINT to show the docking
    // hint immediately.
    wxAUI_MGR_NO_VENETIAN_BLINDS_FADE |
    // When a docked pane is resized, its content is refreshed in live (instead
    // of moving the border alone and refreshing the content at the end).
    wxAUI_MGR_LIVE_RESIZE;

void MainWindow::SetStatusBarText(const wxString& text,
                                  StatusBarWidgets widget) {
  assert(statusbar_);
  statusbar_->SetStatusText(text, widget);
}

MainWindow::MainWindow(const wxString& app_name, const wxPoint& pos,
                       const wxSize& size)
    : wxFrame(NULL, wxID_ANY, app_name, pos, size),
      closing_(false),
      aui_(NULL, AUI_OPTIONS),
      findres_window_(NULL),
      app_name_(app_name),
      statusbar_(NULL) {
  BindEvents();
  project_.reset(new Project(this, wxEmptyString));
#ifdef _WIN32
  SetIcon(wxICON(aaaaa_logo));
#else
// todo: set icon
#endif
  aui_.SetManagedWindow(this);

  LoadSettings(this, &settings_);

  //////////////////////////////////////////////////////////////////////////
  wxMenu* menu_file = new wxMenu;
  AddMenuItem(menu_file, wxID_OPEN, "&Open...\tCtrl-O", "Open a file");
  AddMenuItem(menu_file, wxID_SAVE, "&Save...\tCtrl-S", "Save the file");
  AddMenuItem(menu_file, wxID_SAVEAS, "Save &as...\tCtrl-Shift-S",
              "Save the file as a new file");
  menu_file->AppendSeparator();
  AddMenuItem(menu_file, ID_FILE_RIDE_SETTINGS, "S&ettings...",
              "Change the settings of RIDE", file_settings_xpm);
  menu_file->AppendSeparator();
  AddMenuItem(menu_file, wxID_EXIT, "", "", file_quit_xpm);

  //////////////////////////////////////////////////////////////////////////
  wxMenu* menu_edit = new wxMenu;
  AddMenuItem(menu_edit, wxID_UNDO, "Undo\tCtrl-Z", "", edit_undo_xpm);
  AddMenuItem(menu_edit, wxID_REDO, "Redo\tCtrl-Shift-Z", "", edit_redo_xpm);
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, wxID_CUT, "Cut\tCtrl-X", "", edit_cut_xpm);
  AddMenuItem(menu_edit, wxID_COPY, "Copy\tCtrl-C", "", edit_copy_xpm);
  AddMenuItem(menu_edit, wxID_PASTE, "Paste\tCtrl-V", "", edit_paste_xpm);
  AddMenuItem(menu_edit, wxID_DUPLICATE, "Duplicate selection or line\tCtrl-D",
              "");
  AddMenuItem(menu_edit, wxID_DELETE, "Delete\tDel", "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, wxID_FIND, "Find\tCtrl-F", "");
  // menu_edit, wxID_OPEN, "Find next\tF3", "");
  AddMenuItem(menu_edit, wxID_REPLACE, "Replace\tCtrl-H", "");
  // menu-edit, wxID_OPEN, "Replace again\tShift-F4", "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_MATCH_BRACE, "Match brace\tCtrl-M", "");
  AddMenuItem(menu_edit, ID_EDIT_SELECT_BRACE,
              "Select to matching brace\tCtrl-Shift-M", "");
  AddMenuItem(menu_edit, ID_EDIT_GOTO_LINE, "Goto line\tCtrl-G", "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, wxID_INDENT, "Increase indent\tTab", "",
              edit_tab_add_xpm);
  AddMenuItem(menu_edit, wxID_UNINDENT, "Reduce indent\tShift-Tab", "",
              edit_tab_remove_xpm);
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, wxID_SELECTALL, "Select all\tCtrl-A", "");
  AddMenuItem(menu_edit, ID_EDIT_SELECT_LINE, "Select line\tCtrl-L", "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_TOUPPER, "Make UPPERCASE\tCtrl-Shift-U", "",
              edit_make_uppercase_xpm);
  AddMenuItem(menu_edit, ID_EDIT_TOLOWER, "Make lowercase\tCtrl-U", "",
              edit_make_lowercase_xpm);
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_MOVELINESUP, "Move selected lines up\tAlt-Up",
              "");
  AddMenuItem(menu_edit, ID_EDIT_MOVELINESDOWN,
              "Move selected lines down\tAlt-Down", "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_OPEN_IN_ONLINE_DOCUMENTATION,
              "Open type in online documentation\tCtrl-'",
              "");  // todo: get a better shortcut
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_SHOW_AUTOCOMPLETE, "Auto complete\tCtrl-Space",
              "");
  menu_edit->AppendSeparator();
  AddMenuItem(menu_edit, ID_EDIT_SHOW_PROPERTIES, "File properties\tAlt-Enter",
              "", edit_file_properties_xpm);

  //////////////////////////////////////////////////////////////////////////

  wxMenu* menu_project = new wxMenu;
  AddMenuItem(menu_project, ID_PROJECT_NEW, "New project...",
              "Create a new cargo project");
  AddMenuItem(menu_project, ID_PROJECT_OPEN, "Open project...\tCtrl-Shift-O",
              "Open a existing cargo or ride project");
  AddMenuItem(menu_project, ID_PROJECT_SETTINGS, "Project settings...",
              "Change the ride project settings", project_settings_xpm);
  menu_project->AppendSeparator();
  AddMenuItem(menu_project, ID_PROJECT_BUILD, "Build\tCtrl-B",
              "Compile the current project", project_build_xpm);
  AddMenuItem(menu_project, ID_PROJECT_SELECT_ACTIVE_BUILD,
              "Select active build...\tCtrl-Shift-B",
              "Select the active build configuration");
  AddMenuItem(menu_project, ID_PROJECT_CLEAN, "Clean",
              "Remove the target directory", project_clean_xpm);
  AddMenuItem(menu_project, ID_PROJECT_REBUILD, "Rebuild", "Clean + Build",
              project_rebuild_xpm);
  AddMenuItem(menu_project, ID_PROJECT_DOC, "Doc",
              "Build this project's and its dependencies' documentation",
              project_doc_xpm);
  AddMenuItem(menu_project, ID_PROJECT_RUN, "Run\tF5",
              "Build and execute src/main.rs", project_run_xpm);
  AddMenuItem(menu_project, ID_PROJECT_SELECT_ACTIVE_RUN,
              "Select active run...\tShift-F5",
              "Select the active run configuration");
  AddMenuItem(menu_project, ID_PROJECT_TEST, "Test", "Run the tests",
              project_test_xpm);
  AddMenuItem(menu_project, ID_PROJECT_BENCH, "Bench", "Run the benchmarks",
              project_bench_xpm);
  AddMenuItem(menu_project, ID_PROJECT_UPDATE, "Update",
              "Update dependencies listed in Cargo.lock", project_update_xpm);
  menu_project->AppendSeparator();
  AddMenuItem(menu_project, wxID_NEW, "", "", file_normal_xpm);
  AddMenuItem(menu_project, ID_QUICK_OPEN,
              "Open file in project...\tShift-Alt-O",
              "Quickly open a file from the project");
  AddMenuItem(menu_project, ID_FIND_IN_FILES, "Find in files...\tCtrl-Shift-F",
              "Find in files");
  AddMenuItem(menu_project, ID_REPLACE_IN_FILES,
              "Replace in files...\tCtrl-Shift-H", "Replace in files");

  //////////////////////////////////////////////////////////////////////////

  wxMenu* menu_view = new wxMenu;
  AddMenuItem(menu_view, ID_VIEW_RESTORE_WINDOWS, "Restore window layout", "");
  AddMenuItem(menu_view, ID_VIEW_SAVE_LAYOUT, "Save layout", "");
  AddMenuItem(menu_view, ID_VIEW_LOAD_LAYOUT, "Load layout", "");
  menu_view->AppendSeparator();

  // shortcuts stolen from qt creator:
  // https://wiki.qt.io/Qt_Creator_Keyboard_Shortcuts
  menuItemViewProject_ = AddMenuItem(menu_view, ID_VIEW_SHOW_PROJECT,
                                     "&Project pane\tAlt-0", "").Checkable();
  AddMenuItem(menu_view, ID_VIEW_SHOW_START, "&Start page", "");
  // build issues
  menuItemViewFind_ = AddMenuItem(menu_view, ID_VIEW_SHOW_FINDRESULT,
                                  "Find &result pane\tAlt-2", "").Checkable();
  // app output
  menuItemViewBuild_ = AddMenuItem(menu_view, ID_VIEW_SHOW_BUILD,
                                   "&Build pane\tAlt-4", "").Checkable();
  menuItemViewCompile_ = AddMenuItem(menu_view, ID_VIEW_SHOW_COMPILE,
                                     "&Compile pane", "").Checkable();

  //////////////////////////////////////////////////////////////////////////
  wxMenu* menu_help = new wxMenu;
  menu_help->Append(wxID_ABOUT);

  //////////////////////////////////////////////////////////////////////////
  wxMenuBar* menu_bar = new wxMenuBar;
  menu_bar->Append(menu_file, "&File");
  menu_bar->Append(menu_edit, "&Edit");
  menu_bar->Append(menu_view, "&View");
  menu_bar->Append(menu_project, "&Project");
  menu_bar->Append(menu_help, "&Help");
  SetMenuBar(menu_bar);

  int sbstyle =
      wxSTB_ELLIPSIZE_END | wxSTB_SHOW_TIPS | wxFULL_REPAINT_ON_RESIZE;
  statusbar_ = new StatusBarGeneric(this, wxID_ANY, sbstyle);
  SetStatusBar(reinterpret_cast<wxStatusBar*>(statusbar_));
  statusbar_->SetFieldsCount(STATUSBAR_MAXCOUNT);

  const int small_width = 60;
  int widths[STATUSBAR_MAXCOUNT] = {
      0,
  };
  widths[STATUSBAR_GENERAL] = -1;
  widths[STATUSBAR_BUILD_CONF] = small_width;
  widths[STATUSBAR_RUN_CONF] = small_width;
  widths[STATUSBAR_LINE] = small_width;
  widths[STATUSBAR_COL] = small_width;
  widths[STATUSBAR_CH] = small_width;
  widths[STATUSBAR_INS] = small_width;
  statusbar_->SetStatusWidths(STATUSBAR_MAXCOUNT, widths);

  int styles[STATUSBAR_MAXCOUNT] = {
      0,
  };
  for (int i = 0; i < STATUSBAR_MAXCOUNT; ++i) {
    styles[i] = wxSB_NORMAL;
  }
  statusbar_->SetStatusStyles(STATUSBAR_MAXCOUNT, styles);

  CreateNotebook();

  // output
  build_output_.Create(this, &aui_, PANE_BUILD, "Build");
  compiler_output_.Create(this, &aui_, PANE_COMPILE, "Compile");

  findres_window_ = new FindResultsControl(this);
  findres_window_->UpdateStyle();
  findres_window_->UpdateStyle();
  aui_.AddPane(findres_window_, wxAuiPaneInfo()
                                    .Name(PANE_FIND_1)
                                    .Caption("Find result")
                                    .Bottom()
                                    .CloseButton(true));

  // project explorer
  project_explorer_ = new ProjectExplorer(this);
  aui_.AddPane(project_explorer_, wxAuiPaneInfo()
                                      .Name(PANE_PROJECT)
                                      .Caption("Project")
                                      .Left()
                                      .CloseButton(true));

  new StartPageTab(notebook_, this);

  UpdateTheme();

  notebook_->Update();
  aui_.Update();

  aui_.Update();
  UpdateTitle();

  windows_locations_ = aui_.SavePerspective();

  RestoreSession();
  UpdateMenuItemView();

  project_->SetMainStatusbarText();
}

void MainWindow::UpdateTheme() {
  const ride::FontsAndColors& c = settings_.fonts_and_colors();

  wxAuiDockArt* dock_art = new wxAuiDefaultDockArt();
  dock_art->SetColor(wxAUI_DOCKART_BACKGROUND_COLOUR, C(c.dock_background()));
  dock_art->SetColor(wxAUI_DOCKART_SASH_COLOUR, C(c.dock_sash()));
  dock_art->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR,
                     C(c.dock_active_caption()));
  dock_art->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR,
                     C(c.dock_active_caption_gradient()));
  dock_art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,
                     C(c.dock_inactive_caption()));
  dock_art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR,
                     C(c.dock_inactive_caption_gradient()));
  dock_art->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR,
                     C(c.dock_active_caption_text()));
  dock_art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR,
                     C(c.dock_inactive_caption_text()));
  dock_art->SetColor(wxAUI_DOCKART_BORDER_COLOUR, C(c.dock_border()));
  dock_art->SetColor(wxAUI_DOCKART_GRIPPER_COLOUR, C(c.dock_gripper()));
  aui_.SetArtProvider(dock_art);

  // we have to create a new tab art each time as wx copies it around
  // like crazy and out new values are not set if we just change a member
  AuiGenericTabArt* tab_art = new AuiGenericTabArt();
  // tab_art->SetColour(wxColor(255, 0, 0, 0));
  // tab_art->SetActiveColour(wxColor(255, 0, 0));
  tab_art->set_backgroundColor(C(c.tab_background()));
  tab_art->set_activeTabBackground(C(c.tab_active_tab()));
  tab_art->set_inactiveTabBackground(C(c.tab_inactive_tab()));
  tab_art->set_activeBorderColor(C(c.tab_active_border()));
  tab_art->set_inactiveBorderColor(C(c.tab_inactive_border()));
  tab_art->set_activeTabText(C(c.tab_active_text()));
  tab_art->set_inactiveTabText(C(c.tab_inactive_text()));
  notebook_->SetArtProvider(tab_art);
  notebook_->GetAuiManager().GetArtProvider()->SetColour(
      wxAUI_DOCKART_BORDER_COLOUR, C(c.tab_border()));
  notebook_->GetAuiManager().GetArtProvider()->SetColour(
      wxAUI_DOCKART_SASH_COLOUR, C(c.tab_sash()));

  // TODO(Gustav) Add style
  statusbar_->set_highlight(C(c.statusbar_highlight()));
  statusbar_->set_shadow(C(c.statusbar_shadow()));
  statusbar_->SetForegroundColour(C(c.statusbar_foreground()));
  statusbar_->SetBackgroundColour(C(c.statusbar_background()));
  statusbar_->InitColours();

  this->SetForegroundColour(wxColor(255, 0, 0));
  this->SetBackgroundColour(wxColor(255, 0, 0));

  aui_.Update();    // we changed the tab art, update all the sizes
  this->Refresh();  // and then force a repaint
}

void MainWindow::OnViewRestoreWindows(wxCommandEvent& event) {
  aui_.LoadPerspective(windows_locations_);
}

void MainWindow::OnViewSaveLayout(wxCommandEvent& event) {
  // TODO(Gustav) Implement
  wxMessageBox("IMPLEMENT ME");
}

void MainWindow::OnViewLoadLayout(wxCommandEvent& event) {
  // TODO(Gustav) Implement
  wxMessageBox("IMPLEMENT ME");
}

void ShowHideAui(wxAuiManager* aui, const wxString& name) {
  assert(aui);
  wxAuiPaneInfo& pane = aui->GetPane(name);
  assert(pane.IsValid() && "This function should only take valid pane names!");
  if (pane.IsShown()) {
    pane.Hide();
  } else {
    pane.Show();
  }
  aui->Update();
}

void UpdateMenuItemBasedOnPane(wxAuiManager* aui, wxMenuItem* item,
                               const wxString& name) {
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

void MainWindow::OnViewShowStart(wxCommandEvent& event) {
  // TODO(Gustav) Implement
  wxMessageBox("IMPLEMENT ME");
}

void MainWindow::OnViewShowFindResult(wxCommandEvent& event) {
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

template <typename ToolbarCheck>
std::vector<SwitcherItem> ListPanes(wxAuiManager* aui) {
  std::vector<SwitcherItem> toolbars;
  for (size_t pane_index = 0; pane_index < aui->GetAllPanes().GetCount();
       pane_index++) {
    wxAuiPaneInfo& info = aui->GetAllPanes()[pane_index];

    wxString name = info.name;
    wxString caption = info.caption;

    wxToolBar* toolBar = wxDynamicCast(info.window, wxToolBar);

    if (!caption.IsEmpty() && ToolbarCheck::Check(toolBar)) {
      // We'll use the item 'id' to store the notebook selection, or -1 if not a
      // page
      toolbars.push_back(SwitcherItem(caption, name, -1).set_window(toolBar));
    }
  }
  return toolbars;
}

struct NonNullToolbar {
  static bool Check(wxToolBar* toolbar) { return toolbar != NULL; }
};

struct NullToolbar {
  static bool Check(wxToolBar* toolbar) { return toolbar == NULL; }
};

void AddGroup(const std::vector<SwitcherItem>& toolbars,
              SwitcherItemList* items, const wxString& title) {
  if (toolbars.empty() == false) {
    items->AddGroup(SwitcherGroup(title, toolbars));
  }
}

void MainWindow::OnTab(bool forward) {
  SwitcherItemList items;

  // Add the main windows and toolbars, in two separate columns

  const std::vector<SwitcherItem> windows = ListPanes<NullToolbar>(&aui_);
  AddGroup(windows, &items, _("Main Windows"));

  const std::vector<SwitcherItem> toolbars = ListPanes<NonNullToolbar>(&aui_);
  AddGroup(toolbars, &items, _("Toolbars"));

  // Now add the wxAuiNotebook pages

  SwitcherGroup& files = items.AddGroup(SwitcherGroup(_("Active Files")));

  struct TabData {
    wxString name;
    wxWindow* win;
    wxBitmap bitmap;
    int index;
  };
  std::map<wxString, TabData> tabdata;
  for (int page = 0; page < notebook_->GetPageCount(); ++page) {
    Tab* tab = TabFromIndex(notebook_, page);
    OpenDocument doc = OpenDocumentFromTab(tab);
    TabData data;
    data.name = notebook_->GetPageText(page);
    data.win = notebook_->GetPage(page);
    data.bitmap = notebook_->GetPageBitmap(page);
    data.index = page;
    tabdata.insert(std::make_pair(doc.id, data));
  }

  for (auto iter = mru_.rbegin(); iter != mru_.rend(); ++iter) {
    const OpenDocument& document = *iter;
    const auto found = tabdata.find(document.id);
    if (found != tabdata.end()) {
      const TabData& data = found->second;
      files.AddItem(SwitcherItem(data.name, data.name, data.index, data.bitmap))
          .set_window(data.win)
          .set_description(document.description)
          .set_path(document.path);
    }
  }

  const SwitcherIndex focus = items.GetIndexForFocus();

  SwitcherStyle style;
  SwitcherDlg dlg(items, focus, style, this);

  dlg.AdvanceToNextSelection(forward);

  int ans = dlg.ShowModal();

  if (ans == wxID_OK && dlg.GetSelection() != SWITCHER_NOT_FOUND) {
    SwitcherItem& item = items.GetItem(dlg.GetSelection());

    if (item.id() == -1) {
      wxAuiPaneInfo& info = aui_.GetPane(item.name());
      info.Show();
      aui_.Update();
      info.window->SetFocus();
    } else {
      wxAuiNotebook* nb =
          wxDynamicCast(item.window()->GetParent(), wxAuiNotebook);
      wxWindow* win = item.window();
      if (nb) {
        nb->SetSelection(item.id());
        win->SetFocus();
      }
    }
  }
}

void MainWindow::OnNotebookNavigation(wxNavigationKeyEvent& nav) {  // NOLINT
  OnTab(nav.GetDirection());
}

void MainWindow::OnTabNext(wxCommandEvent& event) { OnTab(true); }

void MainWindow::OnTabPrev(wxCommandEvent& event) { OnTab(false); }

void CreateNewFile(const wxString& project_root, MainWindow* main,
                   ProjectExplorer* project_explorer) {
  if (project_root == wxEmptyString) {
    ShowError(main, "Unable to create file, no project open.",
              "Unable to create");
    return;
  }
  CreateNewFileDlgHandler dlg(main, project_root,
                              project_explorer->GetRelativePathOfSelected());
  if (false == dlg.ShowModal()) {
    return;
  }

  {
    wxFile file(dlg.file_path(), wxFile::write);
    file.Write(dlg.template_source());
    // file is written here
  }
  project_explorer->UpdateFolderStructure();
  main->OpenFile(dlg.file_path());
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

  notebook_ = new wxAuiNotebook();
  notebook_->Create(this, wxID_ANY, wxPoint(client_size.x, client_size.y),
                    wxSize(430, 200),
                    wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE |
                        wxBORDER_NONE | wxAUI_NB_WINDOWLIST_BUTTON);

  aui_.AddPane(notebook_, wxAuiPaneInfo()
                              .Name(wxT("notebook_content"))
                              .PaneBorder(false)
                              .CloseButton(false)
                              .Movable(false)
                              .CenterPane());
}

const ride::Settings& MainWindow::settings() const { return settings_; }

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

void MainWindow::OnFileExit(wxCommandEvent& event) { Close(true); }

void MainWindow::OnAbout(wxCommandEvent& event) {
  wxAboutDialogInfo about_info;
  about_info.SetName(app_name_);

  // aboutInfo.SetVersion(RIDE_VERSION_STRING);
  about_info.SetDescription(
      _("Ride is a Rust IDE. It's named after concatenating R from rust and "
        "IDE."));
  // aboutInfo.SetCopyright("(C) 1992-2010");
  about_info.SetWebSite("https://github.com/madeso/ride");
  wxAboutBox(about_info, this);

  // todo: this about box has too much collapsible items, use another advanced
  // about box instead?
}

void MainWindow::OnFileOpen(wxCommandEvent& event) {
  wxFileDialog open_file(this, _("Open file"), "", "",
                         languages_.GetFilePattern(),
                         wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
  if (open_file.ShowModal() == wxID_CANCEL) return;

  wxArrayString paths_to_open;
  open_file.GetPaths(paths_to_open);
  for (wxArrayString::iterator path = paths_to_open.begin();
       path != paths_to_open.end(); ++path) {
    OpenFile(*path);
  }
}

FileEdit* MainWindow::AddAllCompilerMessages(FileEdit* file_edit) {
  build_output_.AddAllCompilerMessages(file_edit);
  compiler_output_.AddAllCompilerMessages(file_edit);
  return file_edit;
}

void MainWindow::FileHasBeenRenamed(const wxString& old_path,
                                    const wxString& new_path) {
  auto found_edit = GetEditFromFileName(old_path);
  if (false == found_edit) {
    return;
  }
  FileEdit* edit = found_edit.edit;
  assert(edit);
  edit->FileHasBeenRenamed(new_path);
  // todo: update compiler errors to point to new file...
}

FileEdit* MainWindow::OpenFile(const wxString& file, int start_line,
                               int start_index, int end_line, int end_index) {
  wxFileName file_name(file);

  const wxString full_path = file_name.GetFullPath();

  if (false == file_name.Exists()) {
    ShowError(this, wxString::Format("Unable to open '%s'", full_path),
              "Unable to open file!");
    return NULL;
  }

  FoundEdit found_edit = GetEditFromFileName(full_path);
  if (found_edit) {
    notebook_->SetSelection(found_edit.index);
  };
  FileEdit* found_edit_or_new =
      found_edit.edit != NULL ? found_edit.edit
                              : AddAllCompilerMessages(new FileEdit(
                                    notebook_, this, full_path, &languages_));
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

void MainWindow::OnNotebookPageClose(wxAuiNotebookEvent& event) {
  FileEdit* edit = NotebookFromIndexOrNull(notebook_, event.GetSelection());
  if (edit) {
    if (edit->CanClose(true) == false) {
      event.Veto();
    }
  }
}

FoundEdit MainWindow::GetEditFromFileName(const wxString& file) {
  for (unsigned int tab_index = 0; tab_index < notebook_->GetPageCount();
       ++tab_index) {
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

void MainWindow::OnClose(wxCloseEvent& event) {
  if (closing_) return;
  closing_ = true;

  for (unsigned int tab_index = 0; tab_index < notebook_->GetPageCount();
       ++tab_index) {
    FileEdit* edit = NotebookFromIndexOrNull(notebook_, tab_index);
    if (edit) {
      const bool canAbort = event.CanVeto();
      if (edit->CanClose(canAbort) == false) {
        event.Veto();
        return;
      }
    }
  }

  SaveSession();

  event.Skip();
}

void MainWindow::set_settings(const ride::Settings& settings) {
  assert(this);
  this->settings_ = settings;
  UpdateAllEdits();
  UpdateTheme();
  project_explorer_->UpdateColors();
}

void MainWindow::ProjectSettingsHasChanged() { UpdateAllEdits(); }

void MainWindow::UpdateAllEdits() {
  for (unsigned int tab_index = 0; tab_index < notebook_->GetPageCount();
       ++tab_index) {
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
  ShowSettingsDlg(this, this);
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
#define MEM_FUN(X)                                     \
  void MainWindow::OnEdit##X(wxCommandEvent& event) {  \
    FileEdit* selected_edit = GetSelectedEditorNull(); \
    if (selected_edit == NULL) return;                 \
    selected_edit->X();                                \
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
  const wxString new_title =
      project_->root_folder().IsEmpty()
          ? app_name_
          // todo: only display project folder name instead of the whole path?
          : wxString::Format("%s - %s", project_->root_folder(), app_name_);
  this->SetTitle(new_title);
}

void MainWindow::OnProjectNew(wxCommandEvent& event) {
  // todo: implement creation of new project
  CreateNewProjectDlgHandler dlg(this);
  if (false == dlg.ShowModal()) {
    return;
  }
  // run cargo new

  wxString output;
  if (CmdRunner::Run(dlg.project_folder(), dlg.cargo_command_line(), &output) ==
      false) {
    ShowError(this, output, "Unable to create project!");
    return;
  }

  // open project
  if (false == OpenProject(dlg.target())) {
    ShowError(this, "Unable to open cargo project", "Unable to open");
  }

  UpdateTitle();
}

void MainWindow::OnProjectOpen(wxCommandEvent& event) {
  wxFileDialog open_project_dialog(this, _("Open project"), "", "",
                                   "Cargo files|Cargo.toml",
                                   wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (open_project_dialog.ShowModal() == wxID_CANCEL) return;
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

  // don't load the cargo file, load the whole folder instead as cargo files
  // should be named in a specific way!
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
  if (false == ShowQuickOpenDlg(this, project_->root_folder(),
                                project_explorer_->GetFiles(), &selected)) {
    return;
  }
  for (const auto& file : selected) {
    OpenFile(file);
  }
}

#define MEM_FUN(X)                                       \
  void MainWindow::OnProject##X(wxCommandEvent& event) { \
    ShowBuildWindow();                                   \
    project_->X();                                       \
  }

MEM_FUN(Settings)
MEM_FUN(Build)
MEM_FUN(SelectActiveBuild)
MEM_FUN(Clean)
MEM_FUN(Rebuild)
MEM_FUN(Doc)
MEM_FUN(Run)
MEM_FUN(SelectActiveRun)
MEM_FUN(Test)
MEM_FUN(Bench)
MEM_FUN(Update)
#undef MEM_FUN

ride::WindowState GetState(wxFrame* main) {
  if (main->IsMaximized())
    return ride::WINDOWSTATE_MAXIMIZED;
  else if (main->IsIconized())
    return ride::WINDOWSTATE_ICONIZED;
  else
    return ride::WINDOWSTATE_NORMAL;
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

  for (unsigned int tab_index = 0; tab_index < notebook_->GetPageCount();
       ++tab_index) {
    FileEdit* edit = NotebookFromIndexOrNull(notebook_, tab_index);
    if (edit) {
      int start_line = 0;
      int start_index = 0;
      int end_line = 0;
      int end_index = 0;
      edit->GetSelection(&start_line, &start_index, &end_line, &end_index);
      auto* f = session.mutable_files()->Add();
      f->set_path(edit->filename());
      f->set_start_line(start_line);
      f->set_start_index(start_index);
      f->set_end_line(end_line);
      f->set_end_index(end_index);
    }
  }

  ::SaveSession(this, session);
}

void MainWindow::RestoreSession() {
  ride::Session session;
  ::LoadSession(this, &session);
  if (session.has_window_x()) {
    // if we have set the window x, we assume we have set them all
    SetSize(session.window_x(), session.window_y(), session.window_width(),
            session.window_height());
  }

  if (session.state() == ride::WINDOWSTATE_MAXIMIZED) Maximize();

  // if we quit in a iconized/minimized state... should we restore to the same
  // state
  // or to the normal state...?
  else if (session.state() == ride::WINDOWSTATE_ICONIZED)
    Iconize();

  wxString cargo_file = session.project();
  if (cargo_file.IsEmpty() == false) {
    OpenProject(cargo_file);
  }

  for (auto f : session.files()) {
    OpenFile(f.path(), f.start_line(), f.start_index(), f.end_line(),
             f.end_index());
  }

  if (session.has_aui_perspective()) {
    aui_.LoadPerspective(session.aui_perspective());
  }
}
