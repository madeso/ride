#ifndef RIDE_PROJECT_H
#define RIDE_PROJECT_H

#include "ride/wx.h"

// Define an array of process pointers used by MyFrame
class MyPipedProcess;
WX_DEFINE_ARRAY_PTR(MyPipedProcess *, MyPipedProcessesArray);

class MyProcess;
WX_DEFINE_ARRAY_PTR(MyProcess *, MyProcessesArray);

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
  void OnProcessTerminated(MyPipedProcess *process);
  void OnAsyncTermination(MyProcess *process);
  void Append(const wxString str);
private:
  void CleanOutput();
  void RunCmd(const wxString& cmd);
  void AddPipedProcess(MyPipedProcess *process);

  // array of running processes with redirected IO
  MyPipedProcessesArray m_running;

  // array of all asynchrously running processes
  MyProcessesArray m_allAsync;

private:
  MainWindow* output_;
  wxString root_folder_;
};


#endif  // RIDE_PROJECT_H
