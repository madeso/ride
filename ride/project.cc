#include "ride/project.h"
#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/filename.h>
#include "ride/wxutils.h"

#include "ride/mainwindow.h"
#include "settings.pb.h"
#include "ride/proto.h"

Project::Project(MainWindow* output, const wxString& root_folder) : main_(output), root_folder_(root_folder) {
  if (root_folder_.IsEmpty() == false) {
    if (LoadProto(&project_, GetProjectFile()) == false) {
    }
  }

}

Project::~Project() {
  Save();
}

const wxString& Project::root_folder() const {
  return root_folder_;
}

bool Project::Save() {
  if (root_folder_.IsEmpty() ) return false;
  ride::Project project;
  project.set_tabwidth(tabwidth());
  project.set_usetabs(usetabs());
  return SaveProto(&project, GetProjectFile());
}

int Project::tabwidth() const {
  return project_.tabwidth();
}
bool Project::usetabs() const {
  return project_.usetabs();
}

void Project::set_tabwidth(int tabwidth) {
  project_.set_tabwidth(tabwidth);
}
void Project::set_usetabs(bool usetabs) {
  project_.set_usetabs(usetabs);
}

const wxString Project::GetCargoFile() const {
  if (root_folder_.IsEmpty()) return "";
  wxFileName cargo(root_folder_, "cargo.toml");
  return cargo.GetFullPath();
}

const wxString Project::GetProjectFile() const {
  if (root_folder_.IsEmpty()) return "";
  wxFileName cargo(root_folder_, "project.ride");
  return cargo.GetFullPath();
}

bool Project::IsPartOfProject(const wxString& filename) {
  // todo: implement a better logic for checking if the file is part of the project
  return true;
}

void Project::Settings() {
  // todo: implement me
}

void Project::SaveAllFiles() {
  main_->SaveAllChangedProjectFiles();
  Save();
}

void Project::Build(bool origin_main) {
  if (origin_main) {
    CleanOutput();
    SaveAllFiles();
  }

  // todo: expand commandline with arguments
  RunCmd("cargo build");
}

void Project::Clean(bool origin_main) {
  if (origin_main) {
    CleanOutput();
    SaveAllFiles();
  }

  // todo: expand commandline with arguments
  RunCmd("cargo clean");
}

void Project::Rebuild(bool origin_main) {
  if (origin_main) {
    CleanOutput();
    SaveAllFiles();
  }
  Clean(false);
  Build(false);
}

void Project::Doc(bool origin_main) {
  if (origin_main) {
    CleanOutput();
    SaveAllFiles();
  }

  // todo: expand commandline with arguments
  RunCmd("cargo doc");
}

void Project::Run(bool origin_main) {
  if (origin_main) {
    CleanOutput();
    SaveAllFiles();
  }

  Build(false);
  //todo: run the application
}

void Project::Test(bool origin_main) {
  if (origin_main) {
    CleanOutput();
    SaveAllFiles();
  }

  // todo: expand commandline with arguments
  RunCmd("cargo test");
}

void Project::Bench(bool origin_main) {
  if (origin_main) {
    CleanOutput();
    SaveAllFiles();
  }

  // todo: expand commandline with arguments
  RunCmd("cargo bench");
}

void Project::Update(bool origin_main) {
  if (origin_main) {
    CleanOutput();
    SaveAllFiles();
  }

  // todo: expand commandline with arguments
  RunCmd("cargo update");
}

//////////////////////////////////////////////////////////////////////////

void Project::CleanOutput() {
  main_->build_output().Clear();
}

void Project::Append(const wxString& str) {
  main_->build_output().Append(str);
}

void Project::RunCmd(const wxString& cmd) {
  if (root_folder_.IsEmpty()) {
    ShowInfo(main_, "No project open, you need to open a cargo project first!", "No project open!");
    return;
  }

  MultiRunner::RunCmd(Command(root_folder_, cmd));
}
