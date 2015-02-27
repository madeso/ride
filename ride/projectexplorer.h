#ifndef RIDE_PROJECTEXPLORER_H
#define RIDE_PROJECTEXPLORER_H

#include "ride/wx.h"
#include <wx/treectrl.h>
#include <map>

class wxFileName;
class MainWindow;

class ProjectExplorer : public wxTreeCtrl {
public:
  explicit ProjectExplorer(MainWindow* main);

  void UpdateColors();

  void SetFolder(const wxString& folder);
  void UpdateFolderStructure();
  void HighlightOpenFile(const wxString& file_path);

private:
  void SubUpdateFolderStructure(const wxFileName& root, wxTreeItemId parent, const wxString filespec, const int flags, int index);
void OnDoubleClick(wxMouseEvent& event);

  wxDECLARE_EVENT_TABLE();

private:
  wxImageList images_;
  MainWindow* main_;
  wxString folder_;
  std::map<wxString, wxTreeItemId> folder_to_item_;
  wxTreeItemId last_highlighted_item_;
};

#endif  // RIDE_PROJECTEXPLORER_H
