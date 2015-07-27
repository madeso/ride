// Copyright (2015) Gustav

#ifndef RIDE_SETTINGS_H
#define RIDE_SETTINGS_H

#include "settings.pb.h"

#include <wx/colour.h>

class wxWindow;

void LoadSettings(wxWindow* main, ::ride::Settings& settings);
bool SaveSettings(wxWindow* main, ::ride::Settings& settings);

void LoadSession(wxWindow* main, ::ride::Session& session);
bool SaveSession(wxWindow* main, ::ride::Session& session);

wxColor C(const ride::Color& c);
ride::Color C(const wxColor& c);

#endif  // RIDE_SETTINGS_H