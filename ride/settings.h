// Copyright (2015) Gustav

#ifndef RIDE_SETTINGS_H
#define RIDE_SETTINGS_H

#include <wx/colour.h>

#include "settings.pb.h"

class wxWindow;

void LoadSettings(wxWindow* main, ::ride::Settings* settings);
bool SaveSettings(wxWindow* main, const ::ride::Settings& settings);

void LoadSession(wxWindow* main, ::ride::Session* session);
bool SaveSession(wxWindow* main, const ::ride::Session& session);

wxColor C(const ride::Color& c);
ride::Color C(const wxColor& c);

#endif  // RIDE_SETTINGS_H