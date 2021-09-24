#pragma once

#include <functional>
#include <memory>

#include "api/sdl_dependency.h"

#include "api/app.h"

struct StartupArguments
{
};

using CreateAppFunction = std::function<std::unique_ptr<App> (const StartupArguments&)>;
int run_main(int argc, char** argv, CreateAppFunction create_app);
