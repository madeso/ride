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

#define IMPLMENT_APP(APP_NAME, MAKE_APP) int main(int argc, char** argv)\
{\
  return ::ride::apigl::run_main(argc, argv, [](const ride::apigl::StartupArguments& APP_NAME){ return MAKE_APP; });\
}

