#pragma once

#include <functional>

#include "apigl/dependency_sdl.h"
#include "apigl/app.h"



struct StartupArguments
{
};


using CreateAppFunction = std::function<std::unique_ptr<App> (const StartupArguments&)>;
int run_main(int argc, char** argv, CreateAppFunction create_app);

