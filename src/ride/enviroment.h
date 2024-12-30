#ifndef RIDE_ENVIROMENT_H_
#define RIDE_ENVIROMENT_H_

#include <ride/wx.h>

#include <map>

#include "ride/settings.h"

const std::map<wxString, wxString> CollectRideSpecificEnviroment(
	const ride::MachineSettings& settings
);

#endif	// RIDE_ENVIROMENT_H_
