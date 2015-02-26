#ifndef RIDE_PROJECTEXPLORER_H
#define RIDE_PROJECTEXPLORER_H

#include "ride/wx.h"
#include <wx/treectrl.h>

class wxFileName;

class ProjectExplorer : public wxTreeCtrl {
public:
  explicit ProjectExplorer(wxWindow* parent);

  void SetFolder(const wxString& folder);
private:
  void UpdateFolderStructure();

  void SubUpdateFolderStructure(const wxFileName& root, wxTreeItemId parent, const wxString filespec, const int flags);

private:
  wxString folder_;
};

#endif  // RIDE_PROJECTEXPLORER_H
