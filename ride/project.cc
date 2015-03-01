#include "ride/project.h"
#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>

#include "ride/mainwindow.h"

Project::Project(MainWindow* output, const wxString& root_folder) : main_(output), root_folder_(root_folder) {
}

const wxString& Project::root_folder() const {
  return root_folder_;
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
  main_->Clear();
}

void Project::Append(const wxString& str) {
  main_->Append(str);
}

void Project::RunCmd(const wxString& cmd) {
  if (root_folder_.IsEmpty()) {
    wxMessageBox("No project open, you need to open a cargo project first!", "No project open!", wxICON_INFORMATION);
    return;
  }

  Runner::RunCmd(root_folder_, cmd);
}
