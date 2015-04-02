#ifndef RIDE_RUNNER_H
#define RIDE_RUNNER_H

#include <memory>
class wxString;

class Runner {
public:
  Runner();
  ~Runner();

  Runner(const Runner&) = delete;
  bool operator=(const Runner&) = delete;
  
protected:
  bool RunCmd(const wxString& root, const wxString& cmd);
  bool IsRunning() const;
  virtual void Append(const wxString& str) = 0;
  int GetExitCode();

public:
  struct Pimpl;

private:
  std::unique_ptr<Pimpl> pimpl;
};


#endif  // RIDE_RUNNER_H
