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

// idle timer to keep the process updated since wx apperently doesn't do that
class IdleTimer : public wxTimer {
public:
  IdleTimer(SingleRunner::Pimpl* p) : pimpl_(p) {
  }
  void Notify();

  SingleRunner::Pimpl* pimpl_;
};

//////////////////////////////////////////////////////////////////////////

struct SingleRunner::Pimpl {
  explicit Pimpl(SingleRunner* p) : parent_(p), processes_(NULL), delete_processes_(NULL), pid_(0), exit_code_(-1) {
    assert(parent_);
    idle_timer_.reset(new IdleTimer(this));
  }
  ~Pimpl();

  void Append(const wxString& s) {
    parent_->Append(s);
  }

  void MarkForDeletion(Process *process);

  bool RunCmd(const Command& cmd);

  void Notify();

  void OnCompleted() {
    assert(parent_);
    parent_->Completed();
    idle_timer_->Stop();
  }

  SingleRunner* parent_;
  Process* processes_; // the current running process or NULL
  Process* delete_processes_; // process to be deleted at the end
  long pid_; // the id of the current or previous running process
  int exit_code_;
  std::unique_ptr<IdleTimer> idle_timer_;
};

void IdleTimer::Notify() {
  pimpl_->Notify();
}

class Process : public wxProcess
{
public:
  Process(SingleRunner::Pimpl* project, const wxString& cmd)
    : wxProcess(wxPROCESS_DEFAULT), cmd_(cmd)
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
    // original source: http://sourceforge.net/p/fourpane/code/HEAD/tree/trunk/ExecuteInDialog.cpp

    // The original used wxTextInputStream to read a line at a time.  Fine, except when there was no \n, whereupon the thing would hang
    // Instead, read the stream (which may occasionally contain non-ascii bytes e.g. from g++) into a memorybuffer, then to a wxString

    bool hasInput = false;

    while (IsInputAvailable()) {
      wxMemoryBuffer buf;
      do {
        const char c = GetInputStream()->GetC();

        if (GetInputStream()->Eof()) break;
        if (c == wxT('\n')) break;
        buf.AppendByte(c);
      } while (IsInputAvailable());

      wxString line((const char*)buf.GetData(), wxConvUTF8, buf.GetDataLen()); // Convert the line to utf8
      runner_->Append(line);
      hasInput = true;
    }

    while (IsErrorAvailable())
    {
      wxMemoryBuffer buf;
      do
      {
        const char c = GetErrorStream()->GetC();
        
        if (GetErrorStream()->Eof()) break;
        if (c == wxT('\n')) break;
        buf.AppendByte(c);
      } while (IsErrorAvailable());

      wxString line((const char*)buf.GetData(), wxConvUTF8, buf.GetDataLen());
      runner_->Append(line);
      hasInput = true;
    }

    return hasInput;
  }

protected:
  SingleRunner::Pimpl *runner_;
  wxString cmd_;
};

void SingleRunner::Pimpl::Notify(){
  if (processes_) {
    // sometimes the output hangs until we close the window
    // seems to be waiting for output or something
    // getting the input in a callback seems to remove the waiting...
    processes_->HasInput();
  }
}

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

  idle_timer_->Start(100);
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

class MultiRunner::Runner : public SingleRunner {
public:
  Runner(MultiRunner* r) : runner_(r) {
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
  MultiRunner* runner_;
};

MultiRunner::MultiRunner(){
}

MultiRunner::~MultiRunner(){
}

bool MultiRunner::RunCmd(const Command& cmd) {
  commands_.push_back(cmd);
  if (IsRunning() == true) return true;
  return RunNext(0);
}

bool MultiRunner::RunNext(int last_exit_code) {
  assert(IsRunning() == false);
  if (commands_.empty()) return false;

  if (last_exit_code == 0) {
    last_runner_ = runner_;
    runner_.reset(new Runner(this));
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

bool MultiRunner::IsRunning() const {
  if (runner_) {
    return runner_->IsRunning();
  }
  return false;
}
