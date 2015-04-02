#include "ride/runner.h"
#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>

#include "ride/mainwindow.h"

class PipedProcess;
class Process;

struct Runner::Pimpl {
  explicit Pimpl(Runner* p) : parent(p), processes_(NULL), delete_processes_(NULL), pid_(0), exit_code_(-1) {
    assert(parent);
  }
  ~Pimpl();

  void Append(const wxString& s) {
    parent->Append(s);
  }

  void MarkForDeletion(Process *process);

  bool RunCmd(const wxString& root, const wxString& cmd);

  Runner* parent;
  Process* processes_; // the current running process or NULL
  Process* delete_processes_; // process to be deleted at the end
  long pid_; // the id of the current or previous running process
  int exit_code_;
};

class Process : public wxProcess
{
public:
  Process(Runner::Pimpl* project, const wxString& cmd)
    : wxProcess(), cmd_(cmd)
  {
    runner_ = project;
    Redirect();
  }

  virtual void OnTerminate(int pid, int status) {
    assert(runner_->pid_ == pid);

    // show the rest of the output
    while (HasInput()) {}
    runner_->MarkForDeletion(this);

    runner_->Append(wxString::Format(wxT("Process %u ('%s') terminated with exit code %d."),
      pid, cmd_.c_str(), status));
    runner_->Append("");
    runner_->exit_code_ = status;
  }

  virtual bool HasInput() {
    bool hasInput = false;

    if (IsInputAvailable())
    {
      wxTextInputStream tis(*GetInputStream());
      const wxString msg = tis.ReadLine(); // this assumes that the output is always line buffered
      runner_->Append(msg);
      hasInput = true;
    }

    if (IsErrorAvailable())
    {
      wxTextInputStream tis(*GetErrorStream());
      const wxString msg = tis.ReadLine(); // this assumes that the output is always line buffered
      runner_->Append(msg);
      hasInput = true;
    }

    return hasInput;
  }

protected:
  Runner::Pimpl *runner_;
  wxString cmd_;
};

bool Runner::Pimpl::RunCmd(const wxString& root, const wxString& cmd) {
  Process* process = new Process(this, cmd);
  Append("> " + cmd);

  wxExecuteEnv env;
  env.cwd = root;

  const int flags = wxEXEC_ASYNC | wxEXEC_SHOW_CONSOLE;

  const long process_id = wxExecute(cmd, flags, process, &env);
  
  // async call
  if (!process_id) {
    Append(wxString::Format(wxT("Execution of '%s' failed."), cmd.c_str()));
    delete process;
    return false;
  }

  assert(processes_ == NULL);
  assert(pid_ == 0);
  processes_ = process;
  pid_ = process_id;
  return true;
}

void Runner::Pimpl::MarkForDeletion(Process *process)
{
  assert(processes_ == process);
  processes_ = NULL;
  delete_processes_ = process;
}

Runner::Pimpl:: ~Pimpl() {
  delete delete_processes_;
}

//////////////////////////////////////////////////////////////////////////

Runner::Runner() : pimpl(new Pimpl(this)) {
}

Runner::~Runner() {
}

bool Runner::RunCmd(const wxString& root, const wxString& cmd) {
  assert(pimpl);
  assert(this->IsRunning() == false);
  return pimpl->RunCmd(root, cmd);
}

bool Runner::IsRunning() const {
  return pimpl->processes_ != NULL;
}

int Runner::GetExitCode() {
  assert(this->IsRunning() == false);
  assert(pimpl->pid_ != 0);
  return pimpl->exit_code_;
}

