// Copyright (2015) Gustav

#include "ride/pathtester.h"

#include <string>

#include "ride/cmdrunner.h"
#include "ride/stringutils.h"

wxString RunTest(const std::string& app, const wxString& cmd) {
  wxString output;
  if (CmdRunner::Run("", app.c_str() + cmd, &output) == false) {
    return "";
  }

  auto tok = Tokenize(output.Trim(false).c_str().AsChar(), "\n", true);
  if (tok.size() > 1) {
    // only return the first line
    return tok[1].c_str();
  } else {
    return "";
  }
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

bool PathTester::CargoIsValid() const {
  return cargo_.IsEmpty() == false && cargo_.Contains("cargo");
}

bool PathTester::RustcIsValid() const {
  return rustc_.IsEmpty() == false && rustc_.Contains("rustc");
}

bool PathTester::RacerIsValid() const {
  return racer_.IsEmpty() == false && racer_.Contains("MATCH");
}

bool PathTester::ProtocIsValid() const {
  return protoc_.IsEmpty() == false && protoc_.Contains("libprotoc");
}
