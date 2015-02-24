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

  bool IsPartOfProject(const wxString& filename);

  void Settings();
  void Build   (bool origin_main=true);
  void Clean   (bool origin_main=true);
  void Rebuild (bool origin_main=true);
  void Doc     (bool origin_main=true);
  void Run     (bool origin_main=true);
  void Test    (bool origin_main=true);
  void Bench   (bool origin_main=true);
  void Update  (bool origin_main=true);

public:
  void OnPipedProcessTerminated(PipedProcess *process);
  void OnAsyncProcessTerminated(AsyncProcess *process);
  void Append(const wxString str);
private:
  void SaveAllFiles();
  void CleanOutput();
  void RunCmd(const wxString& cmd);
  void AddPipedProcess(PipedProcess *process);

  PipedProcessesArray piped_running_processes_;
  AsyncProcessesArray async_running_processes_;

private:
  MainWindow* main_;
  wxString root_folder_;
};


#endif  // RIDE_PROJECT_H
