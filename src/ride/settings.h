#ifndef RIDE_SETTINGS_H_
#define RIDE_SETTINGS_H_

#include <wx/colour.h>

#include "settings.proto.h"	 // NOLINT this is how we include it
#include "path.h"

class wxWindow;
struct SerLog;


Fil GetMachineFile();
bool LoadSettings(SerLog* log, wxWindow* main, ::ride::MachineSettings* settings);
bool SaveSettings(wxWindow* main, ::ride::MachineSettings* settings);


Fil GetSettingsFile();
bool LoadSettings(SerLog* log, wxWindow* main, ::ride::Settings* settings);
bool SaveSettings(wxWindow* main, ::ride::Settings* settings);


Fil GetSessionFile();
bool LoadSession(SerLog* log, wxWindow* main, ::ride::Session* session);
bool SaveSession(wxWindow* main, ::ride::Session* session);


Fil GetLanguageFile();
bool LoadLanguage(SerLog* log, wxWindow* main, ::ride::Languages* language);
bool SaveLanguage(wxWindow* main, ::ride::Languages* language);


wxColor C(const ride::Color& c);
ride::Color C(const wxColor& c);

#endif	// RIDE_SETTINGS_H_
