#include "ride/projectexplorer.h"

#include <vector>

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/textdlg.h>
#include <wx/filefn.h> 

#include "ride/mainwindow.h"
#include "ride/resources/icons.h"
#include "ride/wxutils.h"
#include "ride/deletefolderdlg.h"

enum {
  ICON_FILE_NORMAL,
  ICON_FOLDER_NORMAL
};

ProjectExplorer::ProjectExplorer(MainWindow* main)
  : wxTreeCtrl(main, wxID_ANY, wxDefaultPosition, wxDefaultSize,
  wxTR_HAS_BUTTONS
  | wxTR_TWIST_BUTTONS
  | wxTR_MULTIPLE
  | wxTR_LINES_AT_ROOT
  | wxTR_EDIT_LABELS
  ), images_(16, 16), main_(main), last_highlighted_item_(NULL) {
  UpdateColors();

  this->SetImageList(&images_);

  images_.Add(wxIcon(file_normal_xpm));
  images_.Add(wxIcon(folder_normal_xpm));
}

void ProjectExplorer::UpdateColors() {
  const ride::Style& style = main_->settings().fonts_and_colors().default_style();
  this->SetBackgroundColour(C(style.background()));
  this->SetForegroundColour(C(style.foreground()));
}

void ProjectExplorer::SetFolder(const wxString& folder) {
  folder_ = folder;
  UpdateFolderStructure();
}

void ProjectExplorer::HighlightOpenFile(const wxString& file_path) {
  auto res = folder_to_item_.find(file_path);
  if (res != folder_to_item_.end()) {
    wxTreeItemId id = res->second;
    this->SetItemBold(id, true);
    if (last_highlighted_item_.IsOk()) {
      this->SetItemBold(last_highlighted_item_, false);
    }
    last_highlighted_item_ = id;
  }
}

wxFileName SubFolder(const wxFileName& root, const wxString& sub_folder) {
  wxFileName folder(root);
  folder.AppendDir(sub_folder);
  return folder;
}

bool IsDirectory(const wxFileName& root, const wxString directory) {
  const wxFileName temp = SubFolder(root, directory);
  const wxString full_path = temp.GetFullPath();
  const bool ret = wxDir::Exists(full_path);
  return ret;
}

class FileEntry : public wxTreeItemData {
public:
  FileEntry(bool is_directory, const wxString& path) : is_directory_(is_directory), path_(path) {
    int i = 0;
  }
  const wxString& path() const {
    return path_;
  }
  void set_path(const wxString& path) {
    path_ = path;
  }
  bool is_directory() const {
    return is_directory_;
  }
  const wxString GetRelativeFolderPath(const wxString root) const {
    wxFileName fn(path_);
    fn.SetFullName(wxEmptyString);
    const wxString native_path = fn.GetFullPath(wxPATH_NATIVE);
    assert(native_path.StartsWith(root));
    const wxString relative_native = native_path.Right(native_path.length() - root.length());
    const wxString native_path_sep = wxString(1, wxFileName::GetPathSeparator());
    wxString ret = relative_native;
    ret.Replace(native_path_sep, "/");
    return ret;
  }
private:
  bool is_directory_;
  wxString path_;
};

typedef std::pair<wxTreeItemId, FileEntry*> TreeItemFileEntry;

TreeItemFileEntry GetTreeItemData(const ProjectExplorer* pe, wxTreeItemId selected) {
  if (selected.IsOk() == false) return TreeItemFileEntry(NULL, NULL);
  wxTreeItemData* data = pe->GetItemData(selected);
  if (data) {
    FileEntry* entry = reinterpret_cast<FileEntry*>(data);
    return TreeItemFileEntry(selected, entry);
  }
  else {
    return TreeItemFileEntry(selected, NULL);
  }
}

TreeItemFileEntry GetFocused(const ProjectExplorer* pe) {
  wxTreeItemId selected = pe->GetFocusedItem();
  return GetTreeItemData(pe, selected);
}

void ProjectExplorer::UpdateFolderStructure() {
  const int flags = wxDIR_FILES | wxDIR_DIRS; // walk files and folders
  const wxString filespec = "";
  
  last_highlighted_item_.Unset();
  folder_to_item_.clear();
  this->Freeze();
  this->DeleteAllItems();
  this->AppendItem(this->GetRootItem(), "Project", ICON_FOLDER_NORMAL, ICON_FOLDER_NORMAL, new FileEntry(true, folder_));
  SubUpdateFolderStructure(folder_, this->GetRootItem(), filespec, flags, wxEmptyString, 0);
  this->Thaw();

  this->ExpandAll();
}

wxString ProjectExplorer::GetPathOfSelected() const {
  TreeItemFileEntry file = GetFocused(this);
  if (file.second == NULL) return wxEmptyString;
  return file.second->path();
}

wxString ProjectExplorer::GetRelativePathOfSelected() const {
  TreeItemFileEntry file = GetFocused(this);
  if (file.second == NULL) return wxEmptyString;
  return file.second->GetRelativeFolderPath(folder_);
}


std::vector<wxString> TraverseFilesAndFolders(const wxFileName& root, const wxString filespec, const int flags) {
  const wxString root_full_path = root.GetFullPath();

  if (root_full_path.IsEmpty()) return std::vector<wxString>();

  wxDir directory(root_full_path);

  directory.Open(root_full_path);

  std::vector<wxString> ret;
  wxString file_or_directory_name;
  bool cont = directory.GetFirst(&file_or_directory_name, filespec, flags);
  while (cont)
  {
    ret.push_back(file_or_directory_name);
    cont = directory.GetNext(&file_or_directory_name);
  }

  directory.Close();
  return ret;
}

wxString JoinPath(const wxFileName& root, const wxString& file_or_folder) {
  // todo: is this really the correct way to do things?
  return root.GetFullPath() + file_or_folder;
}

void ProjectExplorer::SubUpdateFolderStructure(const wxFileName& root, wxTreeItemId parent, const wxString filespec, const int flags, const wxString& relative_path, int index)
{
  const wxString root_full_path = root.GetFullPath();
  const std::vector<wxString> files_and_folders = TraverseFilesAndFolders(root, filespec, flags);
  for (const wxString file_or_directory_name : files_and_folders)
  {
    if( file_or_directory_name == "target" && index == 0 ) continue;

    const bool is_dir = IsDirectory(root, file_or_directory_name);
    const int image = is_dir ? ICON_FOLDER_NORMAL : ICON_FILE_NORMAL;

    const wxString path = JoinPath(root, file_or_directory_name);
    const wxString future_relative_path = relative_path + file_or_directory_name + "/";
    const wxString dir_path = wxDir(path).GetNameWithSep();

    wxTreeItemData* data = new FileEntry(is_dir
      , is_dir ? dir_path : path);
    wxTreeItemId child = this->AppendItem(parent, file_or_directory_name, image, image, data);
    folder_to_item_[path] = child;
    if (is_dir) {
      const wxFileName folder_name = SubFolder(root, file_or_directory_name);
      SubUpdateFolderStructure(folder_name, child, filespec, flags, future_relative_path, index+1);
    }
  }
}

void OpenFile(TreeItemFileEntry tife, MainWindow* main) {
  if (tife.second && false == tife.second->is_directory()) {
    main->OpenFile(tife.second->path());
  }
}

void ProjectExplorer::OnDoubleClick(wxMouseEvent& event) {
  const auto focused = GetFocused(this);
  if (focused.first.IsOk() == false) return;
  this->Toggle(focused.first);
  OpenFile(focused, main_);
}

enum {
  ID_FOLDER_COLLAPSE = wxID_HIGHEST
  , ID_FOLDER_EXPAND

  , ID_FOLDER_COLLAPSE_ALL_CHILDREN
  , ID_FOLDER_EXPAND_ALL_CHILDREN

  , ID_COLLAPSE_ALL
  , ID_EXPAND_ALL

  , ID_OPEN_FILE
  , ID_CREATE_NEW_FILE
  , ID_CREATE_NEW_FOLDER
  , ID_DELETE_FILE_OR_FOLDER
};

void ProjectExplorer::OnContextMenu(wxContextMenuEvent& event) {
  const wxPoint mouse_point = GetContextEventPosition(event);
  const wxPoint client_point = ScreenToClient(mouse_point);

  const auto selected = GetFocused(this);

  const bool is_folder = selected.second ? selected.second->is_directory() == true  : false;
  const bool is_file   = selected.second ? selected.second->is_directory() == false : false;
  
  wxMenu menu;
  AppendEnabled(menu, ID_OPEN_FILE, "Open file", is_file);
  menu.AppendSeparator();
  AppendEnabled(menu, ID_CREATE_NEW_FILE, "Create new file...", is_folder);
  AppendEnabled(menu, ID_CREATE_NEW_FOLDER, "Create new folder...", is_folder);
  menu.AppendSeparator();
  AppendEnabled(menu, ID_FOLDER_COLLAPSE, "Collapse", is_folder);
  AppendEnabled(menu, ID_FOLDER_COLLAPSE_ALL_CHILDREN, "Collapse children", is_folder);
  AppendEnabled(menu, ID_COLLAPSE_ALL, "Collapse all", true); 
  menu.AppendSeparator();
  AppendEnabled(menu, ID_FOLDER_EXPAND, "Expand", is_folder);
  AppendEnabled(menu, ID_FOLDER_EXPAND_ALL_CHILDREN, "Expand children", is_folder);
  AppendEnabled(menu, ID_EXPAND_ALL, "Expand all", true);
  menu.AppendSeparator();
  AppendEnabled(menu, ID_DELETE_FILE_OR_FOLDER, "Delete", true);

  PopupMenu(&menu);
}

void ProjectExplorer::OnCreateNewFolder(wxCommandEvent& event) {
  const wxString path_of_selected = GetPathOfSelected();
  wxTextEntryDialog dlg(this, "Please enter folder name:", "Folder name");
  if (dlg.ShowModal() != wxID_OK) return;
  const wxString folder_name = dlg.GetValue();
  wxFileName fn(path_of_selected); 
  fn.AppendDir(folder_name);
  const wxString dir = fn.GetPathWithSep();
  const bool folder_exists = wxDir::Exists(dir);
  if (folder_exists) {
    wxMessageBox("Entered folder exist", "Unable to create", wxICON_WARNING, this);
    return;
  }
  const bool created_folder = wxDir::Make(dir);
  if (false == created_folder) {
    wxMessageBox("Unable to create folder", "Unable to create", wxICON_ERROR, this);
    return;
  }
  UpdateFolderStructure();
}

void ProjectExplorer::OnCreateNewFile(wxCommandEvent& event) {
  CreateNewFile(folder_, main_, this);
}

void ProjectExplorer::OnFolderCollapse(wxCommandEvent& event){
  const auto selected = GetFocused(this);
  if (selected.first.IsOk() == false) return;
  this->Collapse(selected.first);
}

void ProjectExplorer::OnFolderExpand(wxCommandEvent& event){
  const auto selected = GetFocused(this);
  if (selected.first.IsOk() == false) return;
  this->Expand(selected.first);
}

void ProjectExplorer::OnFolderCollapseAllChildren(wxCommandEvent& event){
  const auto selected = GetFocused(this);
  if (selected.first.IsOk() == false) return;
  this->CollapseAllChildren(selected.first);
}

void ProjectExplorer::OnFolderExpandAllChildren(wxCommandEvent& event){
  const auto selected = GetFocused(this);
  if (selected.first.IsOk() == false) return;
  this->ExpandAllChildren(selected.first);
}

void ProjectExplorer::OnCollapseAll(wxCommandEvent& event){
  this->CollapseAll();
}

void ProjectExplorer::OnExpandAll(wxCommandEvent& event){
  this->ExpandAll();
}

void ProjectExplorer::OnOpenFile(wxCommandEvent& event){
  const auto selected = GetFocused(this);
  OpenFile(selected, main_);
}

void ProjectExplorer::OnDeleteFileOrFolder(wxCommandEvent& event) {
  const auto selected = GetFocused(this);
  FileEntry* file = selected.second;
  if (file == NULL)  return;
  
  // if a dialog is shown, the file pointer is invalidated, since we rebuild the whole tree structure and thus delete the entries
  // lets "fix" this for now by storing copies and setting our reference to null
  const wxString path = file->path();
  const bool is_directory = file->is_directory();
  file = NULL;

  if (is_directory) {
    // we don't care about the value of these really, since the dialog should set them
    // however, let's set them to a safe value to be sure
    bool full = false;
    bool recursive = false;
    if (false == ShowDeleteFolderDlg(this, path, &full, &recursive)) {
      return;
    }
    int flags = 0;
    if (full) {
      flags |= wxPATH_RMDIR_FULL;
    }
    if (recursive) {
      flags |= wxPATH_RMDIR_RECURSIVE;
    }
    const bool removed = wxFileName::Rmdir(path, flags);
    if (false == removed) {
      // wxMessageBox("Unable to remove folder!", "Unable to remove", wxICON_ERROR, this);
      // wxWidgets seems to display a dialog here for us...
    }
  }
  else {
    wxString mess = "Do you want to delete " + path;
    // wxMessageBox(mess, "Delete?", wxICON_QUESTION | wxYES_NO, NULL);
    DialogResult ret = ShowYesNo(this, "Remove?", "Remove file", "Keep file", path, "Do you want to delete " + path);
    if (DialogResult::YES != ret) {
      return;
    }
    const bool file_removed = wxRemoveFile(path);
    if (false == file_removed) {
      wxMessageBox("Unable to remove file!", "Unable to remove", wxICON_ERROR, this);
    }
  }
  UpdateFolderStructure();
}

void ProjectExplorer::OnEditLabelStart(wxTreeEvent& event) {
  if (event.IsEditCancelled()) return;
  auto data = GetTreeItemData(this, event.GetItem());
  FileEntry* file = data.second;
  if (file == NULL)  {
    event.Veto();
    return;
  }
  if (file->is_directory()) {
    // directory edit isn't allowed
    event.Veto();
    return;
  }
}

void ProjectExplorer::OnEditLabelEnd(wxTreeEvent& event) {
  if (event.IsEditCancelled()) return;
  auto data = GetTreeItemData(this, event.GetItem());
  FileEntry* file = data.second;
  if (file == NULL)  {
    event.Veto();
    return;
  }
  const wxString text = event.GetLabel();// this->GetItemText(data.first);
  
  const wxString old_path = file->path();
  if (file->is_directory()) {
    wxFileName new_name(old_path);
    new_name.RemoveLastDir();
    const bool appended = new_name.AppendDir(text);
    if (appended == false) {
      wxMessageBox("Unable to change directory path", "Unable to rename", wxICON_ERROR, this);
      event.Veto();
      return;
    }
    const wxString new_path = new_name.GetFullPath();

    // todo: rename directory!
    event.Veto();
    return;
  }
  else {
    wxFileName new_name(old_path);
    new_name.SetFullName(text);
    const wxString new_path = new_name.GetFullPath();
    const bool renamed_file = wxRenameFile(old_path, new_path, false);
    if (renamed_file == false) {
      event.Veto();
      wxMessageBox("Unable to change file name", "Unable to rename", wxICON_ERROR, this);
      return;
    }
    main_->FileHasBeenRenamed(old_path, new_path);
    file->set_path(new_path);
    return;
  }
  event.Veto();
}

wxBEGIN_EVENT_TABLE(ProjectExplorer, wxTreeCtrl)
EVT_LEFT_DCLICK(ProjectExplorer::OnDoubleClick)
EVT_CONTEXT_MENU(ProjectExplorer::OnContextMenu)

EVT_MENU(ID_CREATE_NEW_FILE             , ProjectExplorer::OnCreateNewFile            )
EVT_MENU(ID_CREATE_NEW_FOLDER           , ProjectExplorer::OnCreateNewFolder          )
EVT_MENU(ID_FOLDER_COLLAPSE             , ProjectExplorer::OnFolderCollapse           )
EVT_MENU(ID_FOLDER_EXPAND               , ProjectExplorer::OnFolderExpand             )
EVT_MENU(ID_FOLDER_COLLAPSE_ALL_CHILDREN, ProjectExplorer::OnFolderCollapseAllChildren)
EVT_MENU(ID_FOLDER_EXPAND_ALL_CHILDREN  , ProjectExplorer::OnFolderExpandAllChildren  )
EVT_MENU(ID_COLLAPSE_ALL                , ProjectExplorer::OnCollapseAll              )
EVT_MENU(ID_EXPAND_ALL                  , ProjectExplorer::OnExpandAll                )
EVT_MENU(ID_OPEN_FILE                   , ProjectExplorer::OnOpenFile                 )
EVT_MENU(ID_DELETE_FILE_OR_FOLDER       , ProjectExplorer::OnDeleteFileOrFolder       )

EVT_TREE_BEGIN_LABEL_EDIT(wxID_ANY, ProjectExplorer::OnEditLabelStart)
EVT_TREE_END_LABEL_EDIT(wxID_ANY, ProjectExplorer::OnEditLabelEnd)

wxEND_EVENT_TABLE()
