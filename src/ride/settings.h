#ifndef RIDE_SETTINGS_H_
#define RIDE_SETTINGS_H_

#include <wx/colour.h>

#include "settings.proto.h"	 // NOLINT this is how we include it

class wxWindow;

bool LoadSettings(wxWindow* main, ::ride::MachineSettings* settings);
bool SaveSettings(wxWindow* main, const ::ride::MachineSettings& settings);

bool LoadSettings(wxWindow* main, ::ride::Settings* settings);
bool SaveSettings(wxWindow* main, const ::ride::Settings& settings);

bool LoadSession(wxWindow* main, ::ride::Session* session);
bool SaveSession(wxWindow* main, const ::ride::Session& session);

wxColor C(const ride::Color& c);
ride::Color C(const wxColor& c);

#endif	// RIDE_SETTINGS_H_
