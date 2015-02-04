#ifndef RIDE_SETTINGS_H
#define RIDE_SETTINGS_H

#include "settings.pb.h"

void LoadSettings(::ride::Settings& settings);
bool SaveSettings(::ride::Settings& settings);

#endif  // RIDE_SETTINGS_H