// Copyright (2015) Gustav

#include "ride/pathtester.h"

#include <string>

#include "ride/cmdrunner.h"

wxString RunTest(const std::string& app, const wxString& cmd) {
  wxString output;
  if (CmdRunner::Run("", app.c_str() + cmd, &output) == false) {
    return "";
  }

  // TODO(Gustav): Only return the first row
  return output.Trim(false);
}

PathTester::PathTester(const ride::MachineSettings& machine)
    : cargo_(RunTest(machine.cargo(), " --version")),
      rustc_(RunTest(machine.rustc(), " --version")),
      racer_(RunTest(machine.racer(), " complete std::io::B")),
      protoc_(RunTest(machine.protoc(), " --version")) {}

const wxString PathTester::cargo() const { return cargo_; }
const wxString PathTester::rustc() const { return rustc_; }
const wxString PathTester::racer() const { return racer_; }
const wxString PathTester::protoc() const { return protoc_; }
