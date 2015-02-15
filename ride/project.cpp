#include "ride/project.h"
#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>

Project::Project(wxTextCtrl* output, const wxString& root_folder) : output_(output), root_folder_(root_folder) {
}

const wxString& Project::root_folder() const {
  return root_folder_;
}

void Project::Settings() {
  // todo: implement me
}

void Project::Build(bool clean_output) {
  if (clean_output) {
    CleanOutput();
  }
  RunCmd("cargo build");
}

void Project::Clean(bool clean_output) {
  if (clean_output) {
    CleanOutput();
  }
  RunCmd("cargo clean");
}

void Project::Rebuild(bool clean_output) {
  if (clean_output) {
    CleanOutput();
  }
  Clean(false);
  Build(false);
}

void Project::Doc(bool clean_output) {
  if (clean_output) {
    CleanOutput();
  }
  RunCmd("cargo doc");
}

void Project::Run(bool clean_output) {
  if (clean_output) {
    CleanOutput();
  }
  Build(false);
}

void Project::Test(bool clean_output) {
  if (clean_output) {
    CleanOutput();
  }
  RunCmd("cargo test");
}

void Project::Bench(bool clean_output) {
  if (clean_output) {
    CleanOutput();
  }
  RunCmd("cargo bench");
}

void Project::Update(bool clean_output) {
  if (clean_output) {
    CleanOutput();
  }
  RunCmd("cargo update");
}

//////////////////////////////////////////////////////////////////////////

class MyProcess : public wxProcess
{
public:
  MyProcess(Project* parent, const wxString& cmd)
    : wxProcess(), m_cmd(cmd)
  {
    m_parent = parent;
  }

  // instead of overriding this virtual function we might as well process the
  // event from it in the frame class - this might be more convenient in some
  // cases
  virtual void OnTerminate(int pid, int status) {
    m_parent->Append(wxString::Format(wxT("Process %u ('%s') terminated with exit code %d."),
      pid, m_cmd.c_str(), status));
    m_parent->Append("");
    m_parent->OnAsyncTermination(this);
  }

protected:
  Project *m_parent;
  wxString m_cmd;
};

// A specialization of MyProcess for redirecting the output
class MyPipedProcess : public MyProcess
{
public:
  MyPipedProcess(Project *parent, const wxString& cmd)
    : MyProcess(parent, cmd)
  {
    Redirect();
  }

  virtual void OnTerminate(int pid, int status) {
    // show the rest of the output
    while (HasInput())
      ;

    m_parent->OnProcessTerminated(this);

    MyProcess::OnTerminate(pid, status);
  }

  virtual bool HasInput() {
    bool hasInput = false;

    if (IsInputAvailable())
    {
      wxTextInputStream tis(*GetInputStream());

      // this assumes that the output is always line buffered
      wxString msg = tis.ReadLine();

      m_parent->Append(msg);

      hasInput = true;
    }

    if (IsErrorAvailable())
    {
      wxTextInputStream tis(*GetErrorStream());

      // this assumes that the output is always line buffered
      wxString msg = tis.ReadLine();

      m_parent->Append(msg);

      hasInput = true;
    }

    return hasInput;
  }
};

void Project::CleanOutput() {
  output_->Clear();
}

void Project::Append(const wxString str) {
  output_->AppendText(str);
  output_->AppendText("\n");
}

void Project::RunCmd(const wxString& cmd) {
  if (root_folder_.IsEmpty()) {
    wxMessageBox("No project open, you need to open a cargo project first!", "No project open!", wxICON_INFORMATION);
    return;
  }

  MyPipedProcess* process = new MyPipedProcess(this, cmd);
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

void Project::AddPipedProcess(MyPipedProcess *process)
{
  m_running.Add(process);
  m_allAsync.Add(process);
}

void Project::OnProcessTerminated(MyPipedProcess *process)
{
  m_running.Remove(process);
}

void Project::OnAsyncTermination(MyProcess *process)
{
  m_allAsync.Remove(process);
  delete process;
}

