// Copyright (2015) Gustav

#ifndef RIDE_PATHTESTER_H_
#define RIDE_PATHTESTER_H_

#include <ride/wx.h>
#include "ride/settings.h"

class PathTester {
 public:
  explicit PathTester(const ride::MachineSettings& machine);

  const wxString cargo() const;
  const wxString rustc() const;
  const wxString racer() const;
  const wxString protoc() const;

  bool CargoIsValid() const;
  bool RustcIsValid() const;
  bool RacerIsValid() const;
  bool ProtocIsValid() const;

 private:
  wxString cargo_;
  wxString rustc_;
  wxString racer_;
  wxString protoc_;
};

#endif  // RIDE_PATHTESTER_H_
