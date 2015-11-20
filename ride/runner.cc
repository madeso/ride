// Copyright (2015) Gustav

#include "ride/runner.h"
#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>

#include <sstream>  // NOLINT

#include "ride_compiler_settings.h"  // NOLINT

#include "ride/mainwindow.h"
#include "ride/wxid.h"

class PipedProcess;
class Process;

const int RUNNER_CONSOLE_OPTION = wxEXEC_HIDE_CONSOLE;
// const int RUNNER_CONSOLE_OPTION = wxEXEC_SHOW_CONSOLE;

//////////////////////////////////////////////////////////////////////////

Command::Command(const wxString& r, const wxString& c,
                 const std::map<wxString, wxString>& env)
    : root(r), cmd(c), enviroment(env) {}

//////////////////////////////////////////////////////////////////////////

/// The idle timer let the runner be "notified" that there might be unread
/// compiler messages instead of waiting to the end to read them all
class IdleTimer : public wxTimer {
 public:
  explicit IdleTimer(SingleRunner::Pimpl* p) : pimpl_(p) {}
  void Notify() override;

  SingleRunner::Pimpl* pimpl_;
};

//////////////////////////////////////////////////////////////////////////

struct SingleRunner::Pimpl {
  explicit Pimpl(SingleRunner* p)
      : parent_(p),
        processes_(NULL),
        pid_(0),
        has_exit_code_(false),
        exit_code_(-1) {
    assert(parent_);
    idle_timer_.reset(new IdleTimer(this));
  }
  ~Pimpl();

  void Append(const wxString& s) { parent_->Append(s); }

  bool RunCmd(const Command& cmd);

  void Notify();

  void OnCompleted() {
    assert(parent_);
    parent_->Completed();
    idle_timer_->Stop();
  }

  SingleRunner* parent_;
  Process* processes_;  // the current running process or NULL
  WXID pid_;            // the id of the current or previous running process
  std::unique_ptr<IdleTimer> idle_timer_;

  int exit_code() const {
    assert(has_exit_code_);
    return exit_code_;
  }
  bool has_exit_code() const { return has_exit_code_; }
  void set_exit_code(int x) {
    exit_code_ = x;
    has_exit_code_ = true;
  }

 private:
  bool has_exit_code_;
  int exit_code_;
};

void IdleTimer::Notify() { pimpl_->Notify(); }

class Process : public wxProcess {
 public:
  Process(SingleRunner::Pimpl* project, const wxString& cmd)
      : wxProcess(wxPROCESS_DEFAULT), cmd_(cmd) {
    runner_ = project;
    Redirect();
  }

  void OnTerminate(int pid, int status) override {
    assert(runner_);
    assert(runner_->pid_ == pid);

    // show the rest of the output
    while (HasInput()) {
    }

    runner_->Append(
        wxString::Format(wxT("Process %u ('%s') terminated with exit code %d."),
                         pid, cmd_.c_str(), status));
    runner_->Append("");
    runner_->set_exit_code(status);
    runner_->OnCompleted();
  }

  virtual bool HasInput() {
    // original source:
    // http://sourceforge.net/p/fourpane/code/HEAD/tree/trunk/ExecuteInDialog.cpp

    // The original used wxTextInputStream to read a line at a time.  Fine,
    // except when there was no \n, whereupon the thing would hang
    // Instead, read the stream (which may occasionally contain non-ascii bytes
    // e.g. from g++) into a memorybuffer, then to a wxString

    bool hasInput = false;

    while (IsInputAvailable()) {
      wxMemoryBuffer buf;
      do {
        const char c = GetInputStream()->GetC();

        if (GetInputStream()->Eof()) break;
        if (c == wxT('\n')) break;
        buf.AppendByte(c);
      } while (IsInputAvailable());

      wxString line((const char*)buf.GetData(), wxConvUTF8,
                    buf.GetDataLen());  // Convert the line to utf8
      runner_->Append(line);
      hasInput = true;
    }

    while (IsErrorAvailable()) {
      wxMemoryBuffer buf;
      do {
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
  SingleRunner::Pimpl* runner_;
  wxString cmd_;
};

SingleRunner::Pimpl::~Pimpl() { delete processes_; }

void SingleRunner::Pimpl::Notify() {
  if (processes_) {
    // sometimes the output hangs until we close the window
    // seems to be waiting for output or something
    // getting the input in a callback seems to remove the waiting...
    processes_->HasInput();
  }
}

wxString ListEnviroment(const wxEnvVariableHashMap& e) {
  std::ostringstream ss;
  for (auto x : e) {
    ss << x.first << " = " << x.second << "\n";
  }
  return ss.str().c_str();
}

bool SingleRunner::Pimpl::RunCmd(const Command& c) {
  Process* process = new Process(this, c.cmd);
  Append("> " + c.cmd);

  wxExecuteEnv env;
  env.cwd = c.root;

  wxString before = ListEnviroment(env.env);

  bool got_env = wxGetEnvMap(&env.env);
  assert(got_env);

  for (const auto& e : c.enviroment) {
    env.env[e.first] = e.second;
  }

#ifdef RIDE_OS_APPLE
  // for some reason, usr/local/bin isn't on the path on my machine
  // and rust is installed to usr/local/bin, so let's add it if
  // it doesn't exist
  wxString path = env.env["PATH"];
  const wxString opath = path;
  if (path.Contains("/usr/local/bin") == false) {
    path += ":/usr/local/bin";
    env.env["PATH"] = path;
  }
#endif
  wxString after = ListEnviroment(env.env);

  const int flags = wxEXEC_ASYNC | RUNNER_CONSOLE_OPTION;

  const WXID process_id = wxExecute(c.cmd, flags, process, &env);

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

//////////////////////////////////////////////////////////////////////////

SingleRunner::SingleRunner() : pimpl(new Pimpl(this)) {}

SingleRunner::~SingleRunner() {}

bool SingleRunner::RunCmd(const Command& cmd) {
  assert(pimpl);
  assert(this->IsRunning() == false);
  return pimpl->RunCmd(cmd);
}

bool SingleRunner::IsRunning() const {
  const bool has_process = pimpl->processes_ != NULL;
  if (has_process == false) return false;
  // for some reason, calling/assinging theese on osx, keeps osx from waiting
  // forever for a response... weird. there might be some horrible
  // threading issue at work here :(
  /*const bool exists = */ wxProcess::Exists(pimpl->processes_->GetPid());
  /*bool has_input = */ pimpl->processes_->HasInput();
  return !pimpl->has_exit_code();
}

void SingleRunner::Completed() {
  // empty
}

int SingleRunner::GetExitCode() {
  assert(this->IsRunning() == false);
  assert(pimpl->pid_ != 0);
  return pimpl->exit_code();
}

//////////////////////////////////////////////////////////////////////////

class MultiRunner::Runner : public SingleRunner {
 public:
  explicit Runner(MultiRunner* r) : runner_(r) { assert(runner_); }

  void Append(const wxString& str) override {
    assert(runner_);
    runner_->Append(str);
  }
  void Completed() override {
    assert(runner_);
    runner_->RunNext(GetExitCode());
  }
  bool Run(const Command& cmd) { return RunCmd(cmd); }

 private:
  MultiRunner* runner_;
};

MultiRunner::MultiRunner() {}

MultiRunner::~MultiRunner() {}

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
  } else {
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
