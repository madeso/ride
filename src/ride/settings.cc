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

template<typename T>
bool LoadProto(T* message, const wxFileName& file, wxWindow* main, const wxString& name) {
  wxFileName json = file;
  json.SetExt("json");
  if (json.FileExists()) {
    const wxString jsonerr = LoadProtoJson(message, json);
    if (jsonerr != "") {
      ShowError(main, "Unable to load " + name + " as json: " + jsonerr,
                "Error while loading");
      return false;
    }
  } else {
    const wxString jsonerr = LoadProtoJson(message, file);
    if (jsonerr != "") {
      ShowError(main, "Unable to load " + name + ": " + jsonerr,
                "Error while loading");
      return false;
    }
  }
  return true;
}

template<typename T>
bool SaveProto(const T& message, const wxFileName file,
               wxWindow* main, const wxString& name) {
  wxFileName json = file;
  json.SetExt("json");
  const wxString jsonerr = SaveProtoJson(message, json);
  if (jsonerr != "") {
    ShowError(main, "Error while saving " + name + " as json: " + jsonerr,
              "Unable to save");
    return false;
  }

  return true;
}

bool LoadSettings(wxWindow* main, ::ride::MachineSettings* settings) {
  return LoadProto(settings, GetMachineFile(), main, "machine settings");
}

bool SaveSettings(wxWindow* main, const ::ride::MachineSettings& settings) {
  return SaveProto(settings, GetMachineFile(), main, "machine settings");
}

bool LoadSettings(wxWindow* main, ::ride::Settings* settings) {
  const auto r = LoadProto(settings, GetSettingsFile(), main, "settings");
  AddBuiltInThemes(settings);
  return r;
}

bool SaveSettings(wxWindow* main, const ::ride::Settings& settings) {
  return SaveProto(settings, GetSettingsFile(), main, "settings");
}

bool LoadSession(wxWindow* main, ::ride::Session* settings) {
  return LoadProto(settings, GetSessionFile(), main, "last session");
}

bool SaveSession(wxWindow* main, const ::ride::Session& settings) {
  return SaveProto(settings, GetSessionFile(), main, "current session");
}

wxColor C(const ride::Color& c) { return wxColor(c.r, c.g, c.b); }

ride::Color C(const wxColor& c) {
  ride::Color r;
  r.r = c.Red();
  r.g = c.Green();
  r.b = c.Blue();
  return r;
}
