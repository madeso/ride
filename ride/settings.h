// Copyright (2015) Gustav

#ifndef RIDE_SETTINGS_H_
#define RIDE_SETTINGS_H_

#include <wx/colour.h>

#include "settings.pb.h"  // NOLINT this is how we include it

class wxWindow;

void LoadSettings(wxWindow* main, ::ride::MachineSettings* settings);
void SaveSettings(wxWindow* main, const ::ride::MachineSettings& settings);

void LoadSettings(wxWindow* main, ::ride::Settings* settings);
void SaveSettings(wxWindow* main, const ::ride::Settings& settings);

void LoadSession(wxWindow* main, ::ride::Session* session);
void SaveSession(wxWindow* main, const ::ride::Session& session);

wxColor C(const ride::Color& c);
ride::Color C(const wxColor& c);

#endif  // RIDE_SETTINGS_H_
