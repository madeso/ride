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

class AsyncProcess : public wxProcess
{
public:
  AsyncProcess(Project* project, const wxString& cmd)
    : wxProcess(), cmd_(cmd)
  {
    project_ = project;
  }

  virtual void OnTerminate(int pid, int status) {
    project_->Append(wxString::Format(wxT("Process %u ('%s') terminated with exit code %d."),
      pid, cmd_.c_str(), status));
    project_->Append("");
    project_->OnAsyncProcessTerminated(this);
  }

protected:
  Project *project_;
  wxString cmd_;
};

class PipedProcess : public AsyncProcess
{
public:
  PipedProcess(Project *project, const wxString& cmd)
    : AsyncProcess(project, cmd)
  {
    Redirect();
  }

  virtual void OnTerminate(int pid, int status) {
    // show the rest of the output
    while (HasInput()) { }
    project_->OnPipedProcessTerminated(this);
    AsyncProcess::OnTerminate(pid, status);
  }

  virtual bool HasInput() {
    bool hasInput = false;

    if (IsInputAvailable())
    {
      wxTextInputStream tis(*GetInputStream());
      const wxString msg = tis.ReadLine(); // this assumes that the output is always line buffered
      project_->Append(msg);
      hasInput = true;
    }

    if (IsErrorAvailable())
    {
      wxTextInputStream tis(*GetErrorStream());
      const wxString msg = tis.ReadLine(); // this assumes that the output is always line buffered
      project_->Append(msg);
      hasInput = true;
    }

    return hasInput;
  }
};

void Project::CleanOutput() {
  main_->Clear();
}

void Project::Append(const wxString str) {
  main_->Append(str);
}

void Project::RunCmd(const wxString& cmd) {
  if (root_folder_.IsEmpty()) {
    wxMessageBox("No project open, you need to open a cargo project first!", "No project open!", wxICON_INFORMATION);
    return;
  }

  PipedProcess* process = new PipedProcess(this, cmd);
  Append("> " + cmd);

  wxExecuteEnv env;
  env.cwd = root_folder_;
  
  if (!wxExecute(cmd, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE, process, &env)) {
    Append(wxString::Format(wxT("Execution of '%s' failed."), cmd.c_str()));
    delete process;
  }
  else {
    AddPipedProcess(process);
  }
}

void Project::AddPipedProcess(PipedProcess *process)
{
  piped_running_processes_.Add(process);
  async_running_processes_.Add(process);
}

void Project::OnPipedProcessTerminated(PipedProcess *process)
{
  piped_running_processes_.Remove(process);
}

void Project::OnAsyncProcessTerminated(AsyncProcess *process)
{
  async_running_processes_.Remove(process);
  delete process;
}

