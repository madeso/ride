#pragma once

#include <functional>
#include <memory>

#include "api/dependency_sdl.h"

#include "api/app.h"
#include "base/color.h"

struct StartupArguments
{
};

struct Renderer;
void draw_rect(Renderer* ren, const rect<dip>& rect, Color color);


using CreateAppFunction = std::function<std::unique_ptr<App> (const StartupArguments&)>;
int run_main(int argc, char** argv, CreateAppFunction create_app);
