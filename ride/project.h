// Copyright (2015) Gustav

#ifndef RIDE_PROJECT_H
#define RIDE_PROJECT_H

#include <ride/wx.h>
#include "settings.pb.h"  // NOLINT this is how we include it
#include "ride/runner.h"

class MainWindow;

class Project : public MultiRunner {
 public:
  Project(MainWindow* output, const wxString& root_folder);
  ~Project();
  const wxString& root_folder() const;

  bool Save();

  int tabwidth() const;
  bool usetabs() const;

  void set_tabwidth(int tabwidth);
  void set_usetabs(bool usetabs);

  const wxString GetCargoFile() const;
  const wxString GetProjectFile() const;
  const wxString GetUserFile() const;

  bool IsPartOfProject(const wxString& filename);

  void Settings();
  void SelectActiveBuild();
  void SetMainStatusbarText();

  void Build(bool origin_main = true);
  void Clean(bool origin_main = true);
  void Rebuild(bool origin_main = true);
  void Doc(bool origin_main = true);
  void Run(bool origin_main = true);
  void Test(bool origin_main = true);
  void Bench(bool origin_main = true);
  void Update(bool origin_main = true);

 public:
  void Append(const wxString& str);

 private:
  void SaveAllFiles();
  void CleanOutput();
  void RunCmd(const wxString& cmd);

  bool SaveUser();
  int GetSelectedBuildIndex();
  const ride::BuildSetting& GetCurrentBuildSetting();

 private:
  MainWindow* main_;
  wxString root_folder_;

  ride::Project project_;
  ride::UserProject user_;
};

#endif  // RIDE_PROJECT_H
