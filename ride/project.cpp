#include "ride/project.h"

Project::Project(const wxString& root_folder) : root_folder_(root_folder) {

}

const wxString& Project::root_folder() const {
  return root_folder_;
}

void Project::Settings() {
}

void Project::Build() {
}

void Project::Clean() {
}

void Project::Rebuild() {
}

void Project::Doc() {
}

void Project::Run() {
}

void Project::Test() {
}

void Project::Bench() {
}

void Project::Update() {
}
