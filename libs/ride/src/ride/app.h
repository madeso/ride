#pragma once

#include "api/app.h"


namespace ride::libride
{

std::unique_ptr<api::App> create_ride_app(api::PlatformArg arg);

}

