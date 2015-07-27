// Copyright (2015) Gustav

#ifndef RIDE_OUTPUTDIRECTOR_H
#define RIDE_OUTPUTDIRECTOR_H

#include "ride/wx.h"
#include "ride/compilermessage.h"

#include <vector>

class OutputControl;
class MainWindow;
class wxAuiManager;
class FileEdit;

class OutputDirector {
 public:
  OutputDirector();
  void Create(MainWindow* window, wxAuiManager& aui, const wxString& name,
              const wxString& caption);

  void Clear();
  void Append(const wxString& str);
  void UpdateStyles();
  void AddAllCompilerMessages(FileEdit* edit);

 protected:
  void AddCompilerMessage(const CompilerMessage& mess);

 private:
  MainWindow* main_;
  OutputControl* control_;
  std::vector<wxString> files_;
  std::vector<CompilerMessage> compiler_messages_;
};

#endif  // RIDE_OUTPUTDIRECTOR_H
