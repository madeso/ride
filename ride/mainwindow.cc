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

FoundEdit FoundEdit::NOT_FOUND(0, NULL);

template<typename T>
T* NotebookFromIndexOrNull(wxAuiNotebook* notebook, int tab_index) {
  wxWindow* window = notebook->GetPage(tab_index);
  if (window->IsKindOf(CLASSINFO(T))) {
    return reinterpret_cast<T*>(window);
  }
  else {
    return NULL;
  }
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
  ID_PROJECT_UPDATE,

  ID_SEARCH_FOR_THIS_COMPILER_MESSAGE,
  ID_COPY_THIS_COMPILER_MESSAGE,
  ID_CLEAR_COMPILER_OUTPUT
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
  EVT_ACTIVATE(MainWindow::OnActivated)
  
  EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainWindow::OnNotebookPageClose)
wxEND_EVENT_TABLE()

void MainWindow::OnActivated(wxActivateEvent& event) {
  if (event.GetActive()) {
    ReloadFilesIfNeeded();
  }
}

wxPoint GetContextEventPosition(const wxContextMenuEvent& event) {
  wxPoint ret = event.GetPosition();
  // according to the documentation: http://docs.wxwidgets.org/trunk/classwx_context_menu_event.html#a291e3437b4bf913128ea14e511d161cb
  if (ret == wxDefaultPosition) {
    return wxGetMousePosition();
  }
  else {
    return ret;
  }
}

wxString ToShortString(const wxString& str, int max_length) {
  const wxString dots = "...";
  if (str.length() > max_length + dots.length()) {
    return str.Left(max_length) + dots;
  }
  else {
    return str;
  }
}

void AppendEnabled(wxMenu& menu, int id, const wxString text, bool enabled) {
  menu.Append(id, text);
  menu.Enable(id, enabled);
}

class OutputControl : public wxStyledTextCtrl {
public:
  OutputControl(MainWindow* main) : main_(main) {
  }

  void UpdateStyle() {
    this->StyleClearAll();
    this->SetReadOnly(true);

    const ride::Settings& set = main_->settings();
    SetupScintillaCurrentLine(this, set);
    SetupScintillaDefaultStyles(this, set);
    this->SetEndAtLastLine(set.end_at_last_line());
  }

  int context_positon;

  void OnContextMenu(wxContextMenuEvent& event) {
    const wxPoint mouse_point = GetContextEventPosition(event);
    const wxPoint client_point = ScreenToClient(mouse_point);
    context_positon = this->PositionFromPoint(client_point);

    const bool has_selected = this->GetSelectedText().IsEmpty() == false;
    const wxString line_content = GetContextLineContent();
    CompilerMessage compiler_message;
    const bool has_compiler_message = CompilerMessage::Parse(line_content, &compiler_message);
    const wxString message = has_compiler_message ? ToShortString(compiler_message.message(), 45) : "<none>";

    wxMenu menu;
    AppendEnabled(menu, wxID_COPY, "Copy", has_selected);
    AppendEnabled(menu, wxID_SELECTALL, "Select all", true);
    menu.AppendSeparator();
    AppendEnabled(menu, ID_SEARCH_FOR_THIS_COMPILER_MESSAGE, wxString::Format("Search for \"%s\" online", message), has_compiler_message);
    AppendEnabled(menu, ID_COPY_THIS_COMPILER_MESSAGE, wxString::Format("Copy \"%s\" to clipboard", message), has_compiler_message);
    menu.AppendSeparator();
    AppendEnabled(menu, ID_CLEAR_COMPILER_OUTPUT, "Clear output", true);
    


    PopupMenu(&menu);
  }

  void OnCopyThisCompilerMessage(wxCommandEvent& event) {
    const wxString line_content = GetContextLineContent();

    CompilerMessage message;
    if (CompilerMessage::Parse(line_content, &message)) {
      if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxTextDataObject(message.message()));
        wxTheClipboard->Close();
      }
    }
    else {
      wxMessageBox("Unable to get compiler message data", "No compiler message data", wxICON_WARNING, this);
    }
  }

  void OnClearCompilerOuput(wxCommandEvent& event) {
    ClearOutput();
  }

  void ClearOutput() {
    this->SetReadOnly(false);
    this->SetText(wxEmptyString);
    this->SetReadOnly(true);
  }

  void OnSelectAll(wxCommandEvent& event) {
    this->SelectAll();
  }

  void OnCopy(wxCommandEvent& event) {
    this->Copy();
  }

  const wxString GetContextLineContent() {
    long line_number = 0;
    long col = 0;
    const long index = context_positon;
    this->PositionToXY(index, &col, &line_number);
    if (line_number == -1) return wxEmptyString;
    const wxString line_content = GetLineText(line_number);
    return line_content;
  }

  void OnSearchForThisCompilerMessage(wxCommandEvent& event) {
    const wxString line_content = GetContextLineContent();

    CompilerMessage message;
    if (CompilerMessage::Parse(line_content, &message)) {
      wxString mess = message.message();
      mess.Replace("#", "%23");
      const wxString escaped_message = wxURI(mess).BuildURI();
      const wxString url_to_open = wxString::Format("http://www.google.com/search?q=%s", escaped_message);
      wxLaunchDefaultBrowser(url_to_open);
    }
    else {
      wxMessageBox("Unable to get compiler message data", "No compiler message data", wxICON_WARNING, this);
    }
  }

  void OnDoubleClick(wxMouseEvent& event) {
    long line_number = 0;
    long col = 0;
    long index = this->GetInsertionPoint();
    this->PositionToXY(index, &col, &line_number);
    if (line_number == -1) return;
    wxString line_content = GetLineText(line_number);

    CompilerMessage message;
    if (CompilerMessage::Parse(line_content, &message)) {
      main_->OpenCompilerMessage(message);
    }
  }

  MainWindow* main_;
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(OutputControl, wxStyledTextCtrl)
  EVT_LEFT_DCLICK(OutputControl::OnDoubleClick)
  EVT_CONTEXT_MENU(OutputControl::OnContextMenu)
  EVT_MENU(ID_SEARCH_FOR_THIS_COMPILER_MESSAGE, OutputControl::OnSearchForThisCompilerMessage)

  EVT_MENU(ID_COPY_THIS_COMPILER_MESSAGE, OutputControl::OnCopyThisCompilerMessage)
  EVT_MENU(ID_CLEAR_COMPILER_OUTPUT, OutputControl::OnClearCompilerOuput)
  EVT_MENU(wxID_SELECTALL, OutputControl::OnSelectAll)
  EVT_MENU(wxID_COPY, OutputControl::OnCopy)
wxEND_EVENT_TABLE()


MainWindow::MainWindow(const wxString& app_name, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, app_name, pos, size)
, output_window_(NULL)
, project_(this, wxEmptyString)
, app_name_(app_name)
{
  SetIcon(wxICON(aaaaa_logo));
  aui_.SetManagedWindow(this);

  LoadSettings(settings_);

  //////////////////////////////////////////////////////////////////////////
  wxMenu *menu_file = new wxMenu;
  menu_file->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open a file");
  menu_file->Append(wxID_SAVE, "&Save...\tCtrl-S", "Save the file");
  menu_file->Append(wxID_SAVEAS, "Save &as...\tCtrl-Shift-S", "Save the file as a new file");
  menu_file->AppendSeparator();
  menu_file->Append(ID_FILE_RIDE_SETTINGS, "S&ettings...", "Change the settings of RIDE");
  menu_file->AppendSeparator();
  menu_file->Append(wxID_EXIT);

  //////////////////////////////////////////////////////////////////////////
  wxMenu *menu_edit = new wxMenu;
  menu_edit->Append(wxID_UNDO, "Undo\tCtrl-Z", "");
  menu_edit->Append(wxID_REDO, "Redo\tCtrl-Shift-Z", "");
  menu_edit->AppendSeparator();
  menu_edit->Append(wxID_CUT, "Cut\tCtrl-X", "");
  menu_edit->Append(wxID_COPY, "Copy\tCtrl-C", "");
  menu_edit->Append(wxID_PASTE, "Paste\tCtrl-V", "");
  menu_edit->Append(wxID_DUPLICATE, "Duplicate selection or line\tCtrl-D", "");
  menu_edit->Append(wxID_DELETE, "Delete\tDel", "");
  menu_edit->AppendSeparator();
  menu_edit->Append(wxID_FIND, "Find\tCtrl-F", "");
  // menu_edit->Append(wxID_OPEN, "Find next\tF3", "");
  menu_edit->Append(wxID_REPLACE, "Replace\tCtrl-H", "");
  // menu-edit->Append(wxID_OPEN, "Replace again\tShift-F4", "");
  menu_edit->AppendSeparator();
  menu_edit->Append(ID_EDIT_MATCH_BRACE, "Match brace\tCtrl-M", "");
  menu_edit->Append(ID_EDIT_SELECT_BRACE, "Select to matching brace\tCtrl-Shift-M", "");
  menu_edit->Append(ID_EDIT_GOTO_LINE, "Goto line\tCtrl-G", "");
  menu_edit->AppendSeparator();
  menu_edit->Append(wxID_INDENT, "Increase indent\tTab", "");
  menu_edit->Append(wxID_UNINDENT, "Reduce indent\tShift-Tab", "");
  menu_edit->AppendSeparator();
  menu_edit->Append(wxID_SELECTALL, "Select all\tCtrl-A", "");
  menu_edit->Append(ID_EDIT_SELECT_LINE, "Select line\tCtrl-L", "");
  menu_edit->AppendSeparator();
  menu_edit->Append(ID_EDIT_TOUPPER, "Make UPPERCASE\tCtrl-Shift-U", "");
  menu_edit->Append(ID_EDIT_TOLOWER, "Make lowercase\tCtrl-U", "");
  menu_edit->AppendSeparator();
  menu_edit->Append(ID_EDIT_MOVELINESUP, "Move selected lines up\tAlt-Up", "");
  menu_edit->Append(ID_EDIT_MOVELINESDOWN, "Move selected lines down\tAlt-Down", "");
  menu_edit->AppendSeparator();
  menu_edit->Append(ID_EDIT_OPEN_IN_ONLINE_DOCUMENTATION, "Open type in online documentation\tCtrl-B", ""); // todo: get a better shortcut
  menu_edit->AppendSeparator();
  menu_edit->Append(ID_EDIT_SHOW_PROPERTIES, "File properties\tAlt-Enter", "");

  //////////////////////////////////////////////////////////////////////////

  wxMenu *menu_project = new wxMenu;
  menu_project->Append(ID_PROJECT_NEW, "New project...", "Create a new cargo project");
  menu_project->Append(ID_PROJECT_OPEN, "Open project...", "Open a existing cargo or ride project");
  menu_project->Append(ID_PROJECT_SETTINGS, "Project settings...", "Change the ride project settings");
  menu_project->AppendSeparator();
  menu_project->Append(ID_PROJECT_BUILD, "Build\tCtrl-B", "Compile the current project");
  menu_project->Append(ID_PROJECT_CLEAN, "Clean", "Remove the target directory");
  menu_project->Append(ID_PROJECT_REBUILD, "Rebuild\tCtrl-Shift-B", "Clean + Build");
  menu_project->Append(ID_PROJECT_DOC, "Doc", "Build this project's and its dependencies' documentation");
  menu_project->Append(ID_PROJECT_RUN, "Run\tF5", "Build and execute src/main.rs");
  menu_project->Append(ID_PROJECT_TEST, "Test", "Run the tests");
  menu_project->Append(ID_PROJECT_BENCH, "Bench", "Run the benchmarks");
  menu_project->Append(ID_PROJECT_UPDATE, "Update", "Update dependencies listed in Cargo.lock");

  //////////////////////////////////////////////////////////////////////////
  wxMenu *menu_help = new wxMenu;
  menu_help->Append(wxID_ABOUT);

  //////////////////////////////////////////////////////////////////////////
  wxMenuBar *menu_bar = new wxMenuBar;
  menu_bar->Append(menu_file, "&File");
  menu_bar->Append(menu_edit, "&Edit");
  menu_bar->Append(menu_project, "&Project");
  menu_bar->Append(menu_help, "&Help");
  SetMenuBar(menu_bar);
  CreateStatusBar();
  SetStatusText("");

  CreateNotebook();

  // output
  output_window_ = new OutputControl(this);
  output_window_->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxHSCROLL);
  output_window_->UpdateStyle();
  output_window_->UpdateStyle();
  aui_.AddPane(output_window_, wxAuiPaneInfo().Name("output").Caption("Output").Bottom().CloseButton(false));

  // project explorer
  project_explorer_ = new ProjectExplorer(this);
  aui_.AddPane(project_explorer_, wxAuiPaneInfo().Name("project").Caption("Project").Left().CloseButton(false));

  aui_.Update();
  UpdateTitle();
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

  aui_.AddPane(notebook_, wxAuiPaneInfo().Name(wxT("notebook_content")).PaneBorder(false).CloseButton(false).Movable(false).CenterPane() );
}

const ride::Settings& MainWindow::settings() const {
  return settings_;
}

MainWindow::~MainWindow() {
  aui_.UnInit();
}

void MainWindow::Clear() {
  // todo: this probably needs to happen in the gui thread instead of here... or does it?
  output_window_->ClearOutput();
  compiler_messages_.resize(0);

  for (unsigned int i = 0; i < notebook_->GetPageCount(); ++i) {
    FileEdit* edit = NotebookFromIndexOrNull<FileEdit>(notebook_, i);
    if (edit) {
      edit->ClearCompilerMessages();
    }
  }
}

void MainWindow::Append(const wxString& str) {
  // todo: this probably needs to happen in the gui thread instead of here... or does it?
  output_window_->SetReadOnly(false);
  output_window_->AppendText(str);
  output_window_->AppendText("\n");
  output_window_->SetReadOnly(true);

  CompilerMessage mess;
  if (CompilerMessage::Parse(str, &mess)) {
    AddCompilerMessage(mess);
  }
}

void MainWindow::AddCompilerMessage(const CompilerMessage& mess) {
  compiler_messages_.push_back(mess);
  for (unsigned int i = 0; i < notebook_->GetPageCount(); ++i) {
    FileEdit* edit = NotebookFromIndexOrNull<FileEdit>(notebook_, i);
    if (edit) {
      if (edit->filename() == mess.file()) {
        edit->AddCompilerMessage(mess);
      }
    }
  }
}

void MainWindow::ReloadFilesIfNeeded() {
  for (unsigned int i = 0; i < notebook_->GetPageCount(); ++i) {
    FileEdit* edit = NotebookFromIndexOrNull<FileEdit>(notebook_, i);
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
    OpenFile(*path, -1, -1, -1, -1);
  }
}

FileEdit* AddCompilerMessages(const std::vector<CompilerMessage>& messages, FileEdit* file_edit) {
  for (size_t i = 0; i < messages.size(); ++i) {
    const CompilerMessage message = messages[i];
    if (message.file() == file_edit->filename()) {
      file_edit->AddCompilerMessage(message);
    }
  }
  return file_edit;
}

void MainWindow::OpenFile(const wxString& file, int start_line, int start_index, int end_line, int end_index) {
  wxFileName file_name(file);
  file_name.Normalize();
  const wxString full_path = file_name.GetFullPath();

  FoundEdit found_edit = GetEditFromFileName(full_path);
  if (found_edit) {
    notebook_->SetSelection(found_edit.index);
  };
  FileEdit* found_edit_or_new = found_edit.edit != NULL
    ? found_edit.edit
    : AddCompilerMessages(compiler_messages_, new FileEdit(notebook_, this, full_path))
    ;
  found_edit_or_new->SetSelection(start_line, start_index, end_line, end_index);
  found_edit_or_new->Focus();
}

FileEdit* MainWindow::GetSelectedEditorNull() {
  const int selected_tab_index = notebook_->GetSelection();
  if (selected_tab_index == -1) {
    return NULL;
  }
  return NotebookFromIndexOrNull<FileEdit>(notebook_, selected_tab_index);
}

void MainWindow::OnNotebookPageClose(wxAuiNotebookEvent& evt) {
  FileEdit* edit = NotebookFromIndexOrNull<FileEdit>(notebook_, evt.GetSelection());
  if (edit) {
    if (edit->CanClose(true) == false) {
      evt.Veto();
    }
  }
}

FoundEdit MainWindow::GetEditFromFileName(const wxString& file) {
  for (unsigned int tab_index = 0; tab_index < notebook_->GetPageCount(); ++tab_index) {
    FileEdit* edit = NotebookFromIndexOrNull<FileEdit>(notebook_, tab_index);
    if (edit) {
      if (edit->filename() == file) {
        return FoundEdit(tab_index, edit);
      }
    }
  }

  return FoundEdit::NOT_FOUND;
}

void MainWindow::OnClose(wxCloseEvent& evt) {
  for (unsigned int tab_index = 0; tab_index < notebook_->GetPageCount(); ++tab_index) {
    FileEdit* edit = NotebookFromIndexOrNull<FileEdit>(notebook_, tab_index);
    if (edit) {
      const bool canAbort = evt.CanVeto();
      if (edit->CanClose(canAbort) == false) {
        evt.Veto();
        return;
      }
    }
  }

  // shutdown protobuf now, to avoid spewing out memory leaks...
  google::protobuf::ShutdownProtobufLibrary();
  evt.Skip();
}

void MainWindow::set_settings(const ride::Settings& settings) {
  assert(this);
  this->settings_ = settings;
  UpdateAllEdits();
}

void MainWindow::UpdateAllEdits() {
  for (unsigned int tab_index = 0; tab_index < notebook_->GetPageCount(); ++tab_index) {
    FileEdit* edit = NotebookFromIndexOrNull<FileEdit>(notebook_, tab_index);
    if (edit) {
      edit->UpdateTextControl();
    }
  }
  output_window_->UpdateStyle();
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
MEM_FUN(OpenInOnlineDocumentation)
MEM_FUN(ShowProperties)
#undef MEM_FUN
//////////////////////////////////////////////////////////////////////////

void MainWindow::UpdateTitle() {
  const wxString new_title = project_.root_folder().IsEmpty()
    ? app_name_
    // todo: only display project folder name instead of the whole path?
    : wxString::Format("%s - %s", project_.root_folder(), app_name_);
  this->SetTitle(new_title);
}

void MainWindow::OnProjectNew(wxCommandEvent& event) {
  // todo: implement creation of new project
  UpdateTitle();
}

void MainWindow::OnProjectOpen(wxCommandEvent& event) {
  wxFileDialog open_project_dialog(this, _("Open project"), "", "", "Cargo files|Cargo.toml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (open_project_dialog.ShowModal() == wxID_CANCEL)
    return;
  wxFileName cargo_file(open_project_dialog.GetPath());
  cargo_file.Normalize();

  const wxString full_path = cargo_file.GetFullPath();

  const bool is_really_a_cargo_file = full_path.Lower().EndsWith("cargo.toml");
  if (is_really_a_cargo_file == false) {
    wxMessageBox("You didn't select a proper cargo file", "No cargo file", wxICON_ERROR | wxOK);
    return;
  }

  // don't load the cargo file, load the whole folder instead as cargo files should be named in a specific way!
  const wxString project_folder = cargo_file.GetPathWithSep();
  project_ = Project(this, project_folder);
  project_explorer_->SetFolder(project_folder);
  UpdateTitle();
}

void MainWindow::SaveAllChangedProjectFiles() {
  for (unsigned int i = 0; i < notebook_->GetPageCount(); ++i) {
    FileEdit* edit = NotebookFromIndexOrNull<FileEdit>(notebook_, i);
    if (edit) {
      if (project_.IsPartOfProject(edit->filename())) {
        edit->Save();
      }
    }
  }
}

#define MEM_FUN(X) \
  void MainWindow::OnProject ## X(wxCommandEvent& event) {\
    project_. ## X ();\
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
