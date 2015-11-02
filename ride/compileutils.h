// Copyright (2015) Gustav

#ifndef RIDE_COMPILEUTILS_H_
#define RIDE_COMPILEUTILS_H_

#include <ride/wx.h>

#include "ride/settings.h"
class MainWindow;

void CompileProtoFile(const ride::MachineSettings& machine,
                      const wxString& filename, MainWindow* main_window);

#endif  // RIDE_COMPILEUTILS_H_
