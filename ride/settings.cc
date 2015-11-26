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
      ShowError(main, error, "Error while loading");
    }
  }
}

void SaveProto(const google::protobuf::Message& message, const wxFileName file,
               wxWindow* main, const wxString& error) {
  wxFileName json = file;
  json.SetExt("json");
  const wxString jsonerr = SaveProtoJson(message, json);
  if (jsonerr != "") {
    ShowError(main, "Error while json: " + jsonerr, error);
  }

  if (false == SaveProto(message, file)) {
    ShowError(main, error, "Error while saving");
  }
}

void LoadSettings(wxWindow* main, ::ride::MachineSettings* settings) {
  LoadProtoTextOrBinary(settings, GetMachineFile(), main,
                        "Unable to load machine settings file!");
}

void SaveSettings(wxWindow* main, const ::ride::MachineSettings& settings) {
  return SaveProto(settings, GetMachineFile(), main,
                   "Unable to save machine settings!");
}

void LoadSettings(wxWindow* main, ::ride::Settings* settings) {
  LoadProtoTextOrBinary(settings, GetConfigFile(), main,
                        "Unable to load settings file!");
  AddBuiltInThemes(settings);
}

void SaveSettings(wxWindow* main, const ::ride::Settings& settings) {
  SaveProto(settings, GetConfigFile(), main, "Unable to save settings!");
}

void LoadSession(wxWindow* main, ::ride::Session* settings) {
  LoadProtoTextOrBinary(settings, GetSessionFile(), main,
                        "Unable to load last session!");
}

void SaveSession(wxWindow* main, const ::ride::Session& settings) {
  SaveProto(settings, GetSessionFile(), main,
            "Unable to save current session!");
}

wxColor C(const ride::Color& c) { return wxColor(c.r(), c.g(), c.b()); }

ride::Color C(const wxColor& c) {
  ride::Color r;
  r.set_r(c.Red());
  r.set_g(c.Green());
  r.set_b(c.Blue());
  return r;
}
