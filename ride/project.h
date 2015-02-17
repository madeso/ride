#ifndef RIDE_PROJECT_H
#define RIDE_PROJECT_H

#include "ride/wx.h"

class PipedProcess;
class AsyncProcess;

WX_DEFINE_ARRAY_PTR(PipedProcess*, PipedProcessesArray);
WX_DEFINE_ARRAY_PTR(AsyncProcess*, AsyncProcessesArray);

class MainWindow;

class Project {
public:
  Project(MainWindow* output, const wxString& root_folder);
  const wxString& root_folder() const;

  void Settings();
  void Build   (bool clean_output=true);
  void Clean   (bool clean_output=true);
  void Rebuild (bool clean_output=true);
  void Doc     (bool clean_output=true);
  void Run     (bool clean_output=true);
  void Test    (bool clean_output=true);
  void Bench   (bool clean_output=true);
  void Update  (bool clean_output=true);

public:
  void OnPipedProcessTerminated(PipedProcess *process);
  void OnAsyncProcessTerminated(AsyncProcess *process);
  void Append(const wxString str);
private:
  void CleanOutput();
  void RunCmd(const wxString& cmd);
  void AddPipedProcess(PipedProcess *process);

  PipedProcessesArray piped_running_processes_;
  AsyncProcessesArray async_running_processes_;

private:
  MainWindow* output_;
  wxString root_folder_;
};


#endif  // RIDE_PROJECT_H
