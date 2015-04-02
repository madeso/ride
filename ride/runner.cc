#include "ride/runner.h"
#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>

#include "ride/mainwindow.h"

class PipedProcess;
class Process;

//////////////////////////////////////////////////////////////////////////

Command::Command(const wxString& r, const wxString& c) : root(r), cmd(c) {
}

//////////////////////////////////////////////////////////////////////////

struct SingleRunner::Pimpl {
  explicit Pimpl(SingleRunner* p) : parent(p), processes_(NULL), delete_processes_(NULL), pid_(0), exit_code_(-1) {
    assert(parent);
  }
  ~Pimpl();

  void Append(const wxString& s) {
    parent->Append(s);
  }

  void MarkForDeletion(Process *process);

  bool RunCmd(const Command& cmd);

  void OnCompleted() {
    assert(parent);
    parent->Completed();
  }

  SingleRunner* parent;
  Process* processes_; // the current running process or NULL
  Process* delete_processes_; // process to be deleted at the end
  long pid_; // the id of the current or previous running process
  int exit_code_;
};

class Process : public wxProcess
{
public:
  Process(SingleRunner::Pimpl* project, const wxString& cmd)
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
    runner_->OnCompleted();
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
  SingleRunner::Pimpl *runner_;
  wxString cmd_;
};

bool SingleRunner::Pimpl::RunCmd(const Command& c) {
  Process* process = new Process(this, c.cmd);
  Append("> " + c.cmd);

  wxExecuteEnv env;
  env.cwd = c.root;

  const int flags = wxEXEC_ASYNC | wxEXEC_SHOW_CONSOLE;

  const long process_id = wxExecute(c.cmd, flags, process, &env);
  
  // async call
  if (!process_id) {
    Append(wxT("Execution failed."));
    delete process;
    return false;
  }

  assert(processes_ == NULL);
  assert(pid_ == 0);
  processes_ = process;
  pid_ = process_id;
  return true;
}

void SingleRunner::Pimpl::MarkForDeletion(Process *process)
{
  assert(processes_ == process);
  processes_ = NULL;
  delete_processes_ = process;
}

SingleRunner::Pimpl:: ~Pimpl() {
  delete delete_processes_;
}

//////////////////////////////////////////////////////////////////////////

SingleRunner::SingleRunner() : pimpl(new Pimpl(this)) {
}

SingleRunner::~SingleRunner() {
}

bool SingleRunner::RunCmd(const Command& cmd) {
  assert(pimpl);
  assert(this->IsRunning() == false);
  return pimpl->RunCmd(cmd);
}

bool SingleRunner::IsRunning() const {
  return pimpl->processes_ != NULL;
}

void SingleRunner::Completed() {
  // empty
}

int SingleRunner::GetExitCode() {
  assert(this->IsRunning() == false);
  assert(pimpl->pid_ != 0);
  return pimpl->exit_code_;
}

//////////////////////////////////////////////////////////////////////////

class BasicRunner : public SingleRunner {
public:
  BasicRunner(Runner* r) : runner_(r) {
    assert(runner_);
  }

  virtual void Append(const wxString& str) {
    assert(runner_);
    runner_->Append(str);
  }
  virtual void Completed() {
    assert(runner_);
    runner_->RunNext(GetExitCode());
  }
  bool Run(const Command& cmd) {
    return RunCmd(cmd);
  }
private:
  Runner* runner_;
};

Runner::Runner(){
}

Runner::~Runner(){
}

bool Runner::RunCmd(const Command& cmd) {
  commands_.push_back(cmd);
  if (IsRunning() == true) return true;
  return RunNext(0);
}

bool Runner::RunNext(int last_exit_code) {
  assert(IsRunning() == false);
  if (commands_.empty()) return false;

  if (last_exit_code == 0) {
    // run
    runner_.reset(new BasicRunner(this));
    const bool run_result = runner_->Run(*commands_.begin());
    if (run_result) {
      commands_.erase(commands_.begin());
    }
    return run_result;
  }
  else {
    commands_.clear();
    return false;
  }
}

bool Runner::IsRunning() const {
  if (runner_) {
    return runner_->IsRunning();
  }
  return false;
}
