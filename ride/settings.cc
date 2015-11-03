// Copyright (2015) Gustav

#include "ride/settings.h"

#include <ride/wx.h>

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "ride/mainwindow.h"
#include "ride/proto.h"
#include "ride/wxutils.h"
#include "ride/builtinthemes.h"

wxString GetConfigFolder() { return wxStandardPaths::Get().GetUserDataDir(); }

wxFileName GetConfigFile() {
  return wxFileName(GetConfigFolder(), "settings", "data");
}

wxFileName GetSessionFile() {
  return wxFileName(GetConfigFolder(), "session", "data");
}

wxFileName GetMachineFile() {
  return wxFileName(GetConfigFolder(), "machine", "data");
}

void LoadProtoTextOrBinary(google::protobuf::Message* message,
                           const wxFileName& file, wxWindow* main,
                           const wxString& error) {
  if (false == LoadProto(message, file)) {
    if (false == LoadProtoBinary(message, file)) {
      ShowWarning(main, error, "Error");
    }
  }
}

void LoadSettings(wxWindow* main, ::ride::MachineSettings* settings) {
  LoadProtoTextOrBinary(settings, GetMachineFile(), main,
                        "Unable to parse machine settings file!");
}

bool SaveSettings(wxWindow*, const ::ride::MachineSettings& settings) {
  return SaveProto(settings, GetMachineFile());
}

void LoadSettings(wxWindow* main, ::ride::Settings* settings) {
  LoadProtoTextOrBinary(settings, GetConfigFile(), main,
                        "Unable to parse settings file!");
  AddBuiltInThemes(settings);
}

bool SaveSettings(wxWindow*, const ::ride::Settings& settings) {
  return SaveProto(settings, GetConfigFile());
}

void LoadSession(wxWindow* main, ::ride::Session* settings) {
  LoadProtoTextOrBinary(settings, GetSessionFile(), main,
                        "Unable to parse session file!");
}

bool SaveSession(wxWindow*, const ::ride::Session& settings) {
  return SaveProto(settings, GetSessionFile());
}

wxColor C(const ride::Color& c) { return wxColor(c.r(), c.g(), c.b()); }

ride::Color C(const wxColor& c) {
  ride::Color r;
  r.set_r(c.Red());
  r.set_g(c.Green());
  r.set_b(c.Blue());
  return r;
}
