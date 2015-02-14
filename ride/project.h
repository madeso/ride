#ifndef RIDE_PROJECT_H
#define RIDE_PROJECT_H

#include "ride/wx.h"

class Project {
public:
  Project(const wxString& root_folder);
  const wxString& root_folder() const;

private:
  wxString root_folder_;
};


#endif  // RIDE_PROJECT_H
