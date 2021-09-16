// Copyright (2015) Gustav

#ifndef RIDE_RUNNER_H_
#define RIDE_RUNNER_H_

#include <ride/wx.h>

#include <vector>
#include <memory>
#include <map>

class MultiRunner;

/// a command to run
class Command {
 public:
  Command(const wxString& root, const wxString& cmd,
          const std::map<wxString, wxString>& env);

  wxString root;
  wxString cmd;
  std::map<wxString, wxString> enviroment;
};

/// only runs a single command
class SingleRunner {
 public:
  SingleRunner();
  ~SingleRunner();

  SingleRunner(const SingleRunner&) = delete;
  bool operator=(const SingleRunner&) = delete;

  bool IsRunning() const;

 protected:
  bool RunCmd(const Command& cmd);
  virtual void Append(const wxString& str) = 0;
  virtual void Completed();
  int GetExitCode();

 public:
  struct Pimpl;

 private:
  friend struct Pimpl;
  std::unique_ptr<Pimpl> pimpl;
};

/// Run multiple commands by queuing them
class MultiRunner {
 public:
  MultiRunner();
  ~MultiRunner();

  MultiRunner(const MultiRunner&) = delete;
  bool operator=(const MultiRunner&) = delete;

 protected:
  bool RunCmd(const Command& cmd);
  bool IsRunning() const;
  virtual void Append(const wxString& str) = 0;

 private:
  class Runner;
  friend class Runner;
  bool RunNext(int last_exit_code);
  std::shared_ptr<Runner> runner_;
  std::shared_ptr<Runner> last_runner_;
  std::vector<Command> commands_;
};

#endif  // RIDE_RUNNER_H_
