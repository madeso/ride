#ifndef RIDE_SETTINGS_H_
#define RIDE_SETTINGS_H_

#include <wx/colour.h>

#include "settings.proto.h"	 // NOLINT this is how we include it
#include "path.h"

class wxWindow;
struct SerLog;

Fil GetSettingsFile();
Fil GetSessionFile();
Fil GetMachineFile();


bool LoadSettings(SerLog* log, wxWindow* main, ::ride::MachineSettings* settings);
bool SaveSettings(wxWindow* main, ::ride::MachineSettings* settings);

bool LoadSettings(SerLog* log, wxWindow* main, ::ride::Settings* settings);
bool SaveSettings(wxWindow* main, ::ride::Settings* settings);

bool LoadSession(SerLog* log, wxWindow* main, ::ride::Session* session);
bool SaveSession(wxWindow* main, ::ride::Session* session);

wxColor C(const ride::Color& c);
ride::Color C(const wxColor& c);

#endif	// RIDE_SETTINGS_H_
