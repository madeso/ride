#ifndef RIDE_PROJECT_H
#define RIDE_PROJECT_H

#include "ride/wx.h"
#include "ride/runner.h"

class MainWindow;

class Project : public MultiRunner {
public:
  Project(MainWindow* output, const wxString& root_folder);
  const wxString& root_folder() const;

  const wxString GetCargoFile() const;

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
  void Append(const wxString& str);
private:
  void SaveAllFiles();
  void CleanOutput();
  void RunCmd(const wxString& cmd);

private:
  MainWindow* main_;
  wxString root_folder_;
};


#endif  // RIDE_PROJECT_H
