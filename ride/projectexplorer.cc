#include "ride/projectexplorer.h"

#include <vector>

#include <wx/dir.h>
#include <wx/filename.h>

#include "ride/mainwindow.h"

ProjectExplorer::ProjectExplorer(MainWindow* main)
  : wxTreeCtrl(main, wxID_ANY, wxDefaultPosition, wxDefaultSize,
  wxTR_HAS_BUTTONS
  | wxTR_TWIST_BUTTONS
  | wxTR_MULTIPLE
  | wxTR_LINES_AT_ROOT
  ), main_(main) {
  UpdateColors();
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

bool IsDiretory(const wxFileName& root, const wxString directory) {
  wxFileName temp = root;
  temp.AppendDir(directory);
  const wxString full_path = temp.GetFullPath();
  const bool ret = wxDir::Exists(full_path);
  return ret;
}

void ProjectExplorer::UpdateFolderStructure() {
  this->AppendItem(this->GetRootItem(), "Project");

  const int flags = wxDIR_FILES | wxDIR_DIRS; // walk files and folders
  const wxString filespec = "";

  SubUpdateFolderStructure(folder_, this->GetRootItem(), filespec, flags);

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

void ProjectExplorer::SubUpdateFolderStructure(const wxFileName& root, wxTreeItemId parent, const wxString filespec, const int flags)
{
  const wxString root_full_path = root.GetFullPath();
  const std::vector<wxString> files_and_folders = TraverseFilesAndFolders(root, filespec, flags);
  for (const wxString file_or_directory_name : files_and_folders)
  {
    wxTreeItemId child = this->AppendItem(parent, file_or_directory_name);
    if (IsDiretory(root, file_or_directory_name)) {
      wxFileName child_name(root);
      child_name.AppendDir(file_or_directory_name);
      SubUpdateFolderStructure(child_name, child, filespec, flags);
    }
  }
}
