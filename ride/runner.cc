#include "ride/runner.h"
#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>

#include "ride/mainwindow.h"

Runner::Runner() {
}
//////////////////////////////////////////////////////////////////////////

class AsyncProcess : public wxProcess
{
public:
  AsyncProcess(Runner* project, const wxString& cmd)
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
  Runner *project_;
  wxString cmd_;
};

class PipedProcess : public AsyncProcess
{
public:
  PipedProcess(Runner *project, const wxString& cmd)
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

void Runner::RunCmd(const wxString& root, const wxString& cmd) {
  DoCmd(root, cmd, true);
}

int Runner::RunCmdWait(const wxString& root, const wxString& cmd) {
  return DoCmd(root, cmd, false);
}

int Runner::DoCmd(const wxString& root, const wxString& cmd, bool async) {
  PipedProcess* process = new PipedProcess(this, cmd);
  Append("> " + cmd);

  wxExecuteEnv env;
  env.cwd = root;

  const int flags = async
    ? wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE
    : wxEXEC_SYNC  | wxEXEC_HIDE_CONSOLE;

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

void Runner::AddPipedProcess(PipedProcess *process)
{
  piped_running_processes_.Add(process);
  async_running_processes_.Add(process);
}

void Runner::OnPipedProcessTerminated(PipedProcess *process)
{
  piped_running_processes_.Remove(process);
}

void Runner::OnAsyncProcessTerminated(AsyncProcess *process)
{
  async_running_processes_.Remove(process);
  delete process;
}

