#ifndef RIDE_SETTINGS_H
#define RIDE_SETTINGS_H

#include "settings.pb.h"

#include <wx/colour.h>

void LoadSettings(::ride::Settings& settings);
bool SaveSettings(::ride::Settings& settings);

wxColor C(const ride::Color& c);
ride::Color C(const wxColor& c);

#endif  // RIDE_SETTINGS_H