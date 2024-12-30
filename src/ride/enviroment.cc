// Copyright (2015) Gustav

#include "ride/enviroment.h"

#include <ride/wx.h>

#include <map>

const std::map<wxString, wxString> CollectRideSpecificEnviroment(
    const ride::MachineSettings& settings) {
  std::map<wxString, wxString> ret;
  if (settings.rust_src_path != "") {
    ret["RUST_SRC_PATH"] = settings.rust_src_path;
  }
  return ret;
}
