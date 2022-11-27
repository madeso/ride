#pragma once

#include <functional>

#include "apiact/dependency_sdl.h"
#include "apiact/app.h"


namespace ride::apigl
{


struct StartupArguments
{
};


using CreateAppFunction = std::function<std::unique_ptr<App> (const StartupArguments&)>;
int run_main(int argc, char** argv, CreateAppFunction create_app);


}


