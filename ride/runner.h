#ifndef RIDE_RUNNER_H
#define RIDE_RUNNER_H

#include "ride/wx.h"

class PipedProcess;
class AsyncProcess;

WX_DEFINE_ARRAY_PTR(PipedProcess*, PipedProcessesArray);
WX_DEFINE_ARRAY_PTR(AsyncProcess*, AsyncProcessesArray);

class Runner {
public:
  Runner();
  
protected:
  void RunCmd(const wxString& root, const wxString& cmd);
  int RunCmdWait(const wxString& root, const wxString& cmd);

public:
  // todo: move to private
  void OnPipedProcessTerminated(PipedProcess *process);
  void OnAsyncProcessTerminated(AsyncProcess *process);
  virtual void Append(const wxString& str) = 0;
private:
  int DoCmd(const wxString& root, const wxString& cmd, bool async);
  void AddPipedProcess(PipedProcess *process);

  PipedProcessesArray piped_running_processes_;
  AsyncProcessesArray async_running_processes_;
};


#endif  // RIDE_RUNNER_H
