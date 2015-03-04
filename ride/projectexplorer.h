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

  wxString GetPathOfSelected() const;
  wxString GetRelativePathOfSelected() const;

private:
  void SubUpdateFolderStructure(const wxFileName& root, wxTreeItemId parent, const wxString filespec, const int flags, const wxString& relative_path, int index);
  void OnDoubleClick(wxMouseEvent& event);
  void OnContextMenu(wxContextMenuEvent& event);

  void OnCreateNewFolder          (wxCommandEvent& event);
  void OnCreateNewFile            (wxCommandEvent& event);
  void OnFolderCollapse           (wxCommandEvent& event);
  void OnFolderExpand             (wxCommandEvent& event);
  void OnFolderCollapseAllChildren(wxCommandEvent& event);
  void OnFolderExpandAllChildren  (wxCommandEvent& event);
  void OnCollapseAll              (wxCommandEvent& event);
  void OnExpandAll                (wxCommandEvent& event);
  void OnOpenFile                 (wxCommandEvent& event);

  wxDECLARE_EVENT_TABLE();

private:
  wxImageList images_;
  MainWindow* main_;
  wxString folder_;
  std::map<wxString, wxTreeItemId> folder_to_item_;
  wxTreeItemId last_highlighted_item_;
};

#endif  // RIDE_PROJECTEXPLORER_H
