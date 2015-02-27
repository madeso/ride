#include "ride/projectexplorer.h"

#include <vector>

#include <wx/dir.h>
#include <wx/filename.h>

#include "ride/mainwindow.h"
#include "ride/resources/icons.h"

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
  ), images_(16, 16), main_(main) {
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

void ProjectExplorer::UpdateFolderStructure() {
  this->AppendItem(this->GetRootItem(), "Project", ICON_FOLDER_NORMAL);

  const int flags = wxDIR_FILES | wxDIR_DIRS; // walk files and folders
  const wxString filespec = "";

  SubUpdateFolderStructure(folder_, this->GetRootItem(), filespec, flags, 0);

  this->ExpandAll();
}


std::vector<wxString> TraverseFilesAndFolders(const wxFileName& root, const wxString filespec, const int flags) {
  std::vector<wxString> ret;
  const wxString root_full_path = root.GetFullPath();

  wxDir directory(root_full_path);

  directory.Open(root_full_path);

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

void ProjectExplorer::SubUpdateFolderStructure(const wxFileName& root, wxTreeItemId parent, const wxString filespec, const int flags, int index)
{
  const wxString root_full_path = root.GetFullPath();
  const std::vector<wxString> files_and_folders = TraverseFilesAndFolders(root, filespec, flags);
  for (const wxString file_or_directory_name : files_and_folders)
  {
    if( file_or_directory_name == "target" && index == 0 ) continue;

    const bool is_dir = IsDirectory(root, file_or_directory_name);
    wxTreeItemId child = this->AppendItem(parent, file_or_directory_name, is_dir ? ICON_FOLDER_NORMAL : ICON_FILE_NORMAL);
    if (is_dir) {
      const wxFileName child_name = SubFolder(root, file_or_directory_name);
      SubUpdateFolderStructure(child_name, child, filespec, flags, index+1);
    }
  }
}
