// Copyright (2015) Gustav

#ifndef RIDE_CMDRUNNER_H_
#define RIDE_CMDRUNNER_H_

#include <ride/wx.h>

#include <map>

#include "ride/runner.h"

class CmdRunner : public SingleRunner {
 private:
  CmdRunner();

  wxString output;

  virtual void Append(const wxString& str);

 public:
  static const bool Run(const wxString& root, const wxString& cmd,
                        const std::map<wxString, wxString>& enviroment,
                        wxString* out);
};

#endif  // RIDE_CMDRUNNER_H_
