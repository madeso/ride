// Copyright (2015) Gustav

#ifndef RIDE_CMDRUNNER_H
#define RIDE_CMDRUNNER_H

#include "ride/wx.h"
#include "ride/runner.h"

class CmdRunner : public SingleRunner {
 private:
  CmdRunner();

  wxString output;

  virtual void Append(const wxString& str);

 public:
  static const bool Run(const wxString& root, const wxString& cmd,
                        wxString* out);
};

#endif  // RIDE_CMDRUNNER_H
