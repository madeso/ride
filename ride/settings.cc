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

wxFileName GetSettingsFile() {
  return wxFileName(GetConfigFolder(), "settings", "data");
}

wxFileName GetSessionFile() {
  return wxFileName(GetConfigFolder(), "session", "data");
}

wxFileName GetMachineFile() {
  return wxFileName(GetConfigFolder(), "machine", "data");
}

void LoadProto(google::protobuf::Message* message, const wxFileName& file,
               wxWindow* main, const wxString& name) {
  wxFileName json = file;
  json.SetExt("json");
  if (json.FileExists()) {
    const wxString jsonerr = LoadProtoJson(message, json);
    if (jsonerr != "") {
      ShowError(main, "Unable to load " + name + " as json: " + jsonerr,
                "Error while loading");
    }
  } else {
    if (false == LoadProtoBinary(message, file)) {
      if (false == LoadProtoText(message, file)) {
        ShowError(main, "Unable to load " + name + "!", "Error while loading");
      }
    }
  }
}

void SaveProto(const google::protobuf::Message& message, const wxFileName file,
               wxWindow* main, const wxString& name) {
  wxFileName json = file;
  json.SetExt("json");
  const wxString jsonerr = SaveProtoJson(message, json);
  if (jsonerr != "") {
    ShowError(main, "Error while saving " + name + " as json: " + jsonerr,
              "Unable to save");
  }

  if (false == SaveProtoBinary(message, file)) {
    ShowError(main, "Error while saving " + name + " as binary!",
              "Error saving binary");
  }
}

void LoadSettings(wxWindow* main, ::ride::MachineSettings* settings) {
  LoadProto(settings, GetMachineFile(), main, "machine settings");
}

void SaveSettings(wxWindow* main, const ::ride::MachineSettings& settings) {
  return SaveProto(settings, GetMachineFile(), main, "machine settings");
}

void LoadSettings(wxWindow* main, ::ride::Settings* settings) {
  LoadProto(settings, GetSettingsFile(), main, "settings");
  AddBuiltInThemes(settings);
}

void SaveSettings(wxWindow* main, const ::ride::Settings& settings) {
  SaveProto(settings, GetSettingsFile(), main, "settings");
}

void LoadSession(wxWindow* main, ::ride::Session* settings) {
  LoadProto(settings, GetSessionFile(), main, "last session");
}

void SaveSession(wxWindow* main, const ::ride::Session& settings) {
  SaveProto(settings, GetSessionFile(), main, "current session");
}

wxColor C(const ride::Color& c) { return wxColor(c.r(), c.g(), c.b()); }

ride::Color C(const wxColor& c) {
  ride::Color r;
  r.set_r(c.Red());
  r.set_g(c.Green());
  r.set_b(c.Blue());
  return r;
}
