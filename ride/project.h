#ifndef RIDE_PROJECT_H
#define RIDE_PROJECT_H

#include "ride/wx.h"

// Define an array of process pointers used by MyFrame
class MyPipedProcess;
WX_DEFINE_ARRAY_PTR(MyPipedProcess *, MyPipedProcessesArray);

class MyProcess;
WX_DEFINE_ARRAY_PTR(MyProcess *, MyProcessesArray);

class Project {
public:
  Project(const wxString& root_folder);
  const wxString& root_folder() const;

  void Settings();
  void Build   ();
  void Clean   ();
  void Rebuild ();
  void Doc     ();
  void Run     ();
  void Test    ();
  void Bench   ();
  void Update  ();

public:
  void OnProcessTerminated(MyPipedProcess *process);
  void OnAsyncTermination(MyProcess *process);
  void Append(const wxString str);
private:
  void RunCmd(const wxString& cmd);
  void AddPipedProcess(MyPipedProcess *process);

  // array of running processes with redirected IO
  MyPipedProcessesArray m_running;

  // array of all asynchrously running processes
  MyProcessesArray m_allAsync;

private:
  wxString root_folder_;
};


#endif  // RIDE_PROJECT_H
