#pragma once

#include <functional>

#include "backend/dependency_sdl.h"
#include "backend/app.h"


namespace ride::backends::gl
{


struct StartupArguments
{
};


using CreateAppFunction = std::function<std::unique_ptr<App> (const StartupArguments&)>;
int run_main(int argc, char** argv, CreateAppFunction create_app);

}

#define IMPLMENT_APP(APP_NAME, MAKE_APP) int main(int argc, char** argv)\
{\
  return ::ride::backends::gl::run_main(argc, argv, [](const ride::backends::gl::StartupArguments& APP_NAME){ return MAKE_APP; });\
}

