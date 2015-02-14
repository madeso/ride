#include "ride/project.h"
#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>

Project::Project(const wxString& root_folder) : root_folder_(root_folder) {
}

const wxString& Project::root_folder() const {
  return root_folder_;
}

void Project::Settings() {
}

void Project::Build() {
  RunCmd("cargo build");
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
      wxString msg;
      msg << m_cmd << wxT(" (stdout): ") << tis.ReadLine();

      m_parent->Append(msg);

      hasInput = true;
    }

    if (IsErrorAvailable())
    {
      wxTextInputStream tis(*GetErrorStream());

      // this assumes that the output is always line buffered
      wxString msg;
      msg << m_cmd << wxT(" (stderr): ") << tis.ReadLine();

      m_parent->Append(msg);

      hasInput = true;
    }

    return hasInput;
  }
};

void Project::Append(const wxString str) {
#ifdef WIN32
  // todo: move to gui console window
  OutputDebugStringA(str.c_str());
#endif
}

void Project::RunCmd(const wxString& cmd) {
  MyPipedProcess* process = new MyPipedProcess(this, cmd);
  
  if (!wxExecute(cmd, wxEXEC_ASYNC, process)) {
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

