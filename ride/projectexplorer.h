#ifndef RIDE_PROJECTEXPLORER_H
#define RIDE_PROJECTEXPLORER_H

#include "ride/wx.h"
#include <wx/treectrl.h>

class ProjectExplorer : public wxTreeCtrl {
public:
  explicit ProjectExplorer(wxWindow* parent);

private:
};

#endif  // RIDE_PROJECTEXPLORER_H
