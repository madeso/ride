// Copyright (2015) Gustav

#include "ride/settings.h"

#include <ride/wx.h>

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "ride/mainwindow.h"
#include "ride/proto.h"
#include "ride/wxutils.h"

wxString GetConfigFolder() { return wxStandardPaths::Get().GetUserDataDir(); }

wxFileName GetConfigFile() {
  return wxFileName(GetConfigFolder(), "settings", "data");
}

wxFileName GetSessionFile() {
  return wxFileName(GetConfigFolder(), "session", "data");
}

void LoadSettings(wxWindow* main, ::ride::Settings* settings) {
  if (false == LoadProtoBinary(settings, GetConfigFile())) {
    ShowWarning(main, "Unable to parse settings file!", "Error");
  }
}

bool SaveSettings(wxWindow*, const ::ride::Settings& settings) {
  return SaveProtoBinary(settings, GetConfigFile());
}

void LoadSession(wxWindow* main, ::ride::Session* settings) {
  if (false == LoadProtoBinary(settings, GetSessionFile())) {
    ShowWarning(main, "Unable to parse session file!", "Error");
  }
}

bool SaveSession(wxWindow*, const ::ride::Session& settings) {
  return SaveProtoBinary(settings, GetSessionFile());
}

wxColor C(const ride::Color& c) { return wxColor(c.r(), c.g(), c.b()); }

ride::Color C(const wxColor& c) {
  ride::Color r;
  r.set_r(c.Red());
  r.set_g(c.Green());
  r.set_b(c.Blue());
  return r;
}
