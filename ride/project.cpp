#include "ride/project.h"

Project::Project(const wxString& root_folder) : root_folder_(root_folder) {

}

const wxString& Project::root_folder() const {
  return root_folder_;
}
