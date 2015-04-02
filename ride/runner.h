#ifndef RIDE_RUNNER_H
#define RIDE_RUNNER_H

#include <vector>
#include <memory>

#include "ride/wx.h"

class MultiRunner;

class Command {
public:
  Command(const wxString& root, const wxString& cmd);

  wxString root;
  wxString cmd;
};

// only run a single command
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

class BasicRunner;
// multiple runners, queue commands
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
  friend class BasicRunner;
  bool RunNext(int last_exit_code);
  std::unique_ptr<BasicRunner> runner_;
  std::vector<Command> commands_;
};


#endif  // RIDE_RUNNER_H
