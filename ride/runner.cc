#include "ride/runner.h"
#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>

#include "ride/mainwindow.h"

class PipedProcess;
class AsyncProcess;

WX_DEFINE_ARRAY_PTR(PipedProcess*, PipedProcessesArray);
WX_DEFINE_ARRAY_PTR(AsyncProcess*, AsyncProcessesArray);

struct Runner::Pimpl {
  Pimpl();
  void Append(const wxString&) {}
  void AddPipedProcess(PipedProcess *process);
  void OnPipedProcessTerminated(PipedProcess *process);
  void OnAsyncProcessTerminated(AsyncProcess *process);

  int RunCmd(const wxString& root, const wxString& cmd);

  PipedProcessesArray piped_running_processes_;
  AsyncProcessesArray async_running_processes_;
};

Runner::Pimpl::Pimpl() {
}

class AsyncProcess : public wxProcess
{
public:
  AsyncProcess(Runner::Pimpl* project, const wxString& cmd)
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
  Runner::Pimpl *project_;
  wxString cmd_;
};

class PipedProcess : public AsyncProcess
{
public:
  PipedProcess(Runner::Pimpl *project, const wxString& cmd)
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

int Runner::Pimpl::RunCmd(const wxString& root, const wxString& cmd) {
  bool async = true;

  PipedProcess* process = new PipedProcess(this, cmd);
  Append("> " + cmd);

  wxExecuteEnv env;
  env.cwd = root;

  const int flags = async
    ? wxEXEC_ASYNC | wxEXEC_SHOW_CONSOLE
    : wxEXEC_SYNC | wxEXEC_SHOW_CONSOLE;

  const int execute_result = wxExecute(cmd, flags, process, &env);
  
  if (async) {
    // async call
    if (!execute_result) {
      Append(wxString::Format(wxT("Execution of '%s' failed."), cmd.c_str()));
      delete process;
    }
    else {
      AddPipedProcess(process);
    }
  }
  else {
    // sync call, remove process
    delete process;
  }

  return execute_result;
}

void Runner::Pimpl::AddPipedProcess(PipedProcess *process)
{
  piped_running_processes_.Add(process);
  async_running_processes_.Add(process);
}

void Runner::Pimpl::OnPipedProcessTerminated(PipedProcess *process)
{
  piped_running_processes_.Remove(process);
}

void Runner::Pimpl::OnAsyncProcessTerminated(AsyncProcess *process)
{
  async_running_processes_.Remove(process);
  delete process;
}

//////////////////////////////////////////////////////////////////////////

Runner::Runner() {
}
Runner::~Runner() {
}

bool Runner::RunCmd(const wxString& root, const wxString& cmd) {
  return false;
}
bool Runner::IsRunning() const {
  return true;
}
int Runner::GetExitCode() {
  return -1;
}

