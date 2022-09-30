#pragma once

#include <functional>
#include <memory>
#include <optional>

#include "base/rect.h"
#include "base/color.h"

#include "api/dependency_sdl.h"
#include "api/app.h"


struct Renderer;
struct Texture;

struct StartupArguments
{
};

enum class Submit
{
    no, yes
};

void draw_rect(Renderer* ren, const rect<dip>& rect, Color color);
void draw_image(Renderer* ren, std::shared_ptr<Texture> texture, const rect<dip>& rect, Color tint, std::optional<Rectf> sub = {}, Submit submit=Submit::yes);
void draw_image(Renderer* ren, std::shared_ptr<Texture> texture, dip x, dip y, Color tint, std::optional<Rectf> sub = {}, Submit submit=Submit::yes);
void submit_renderer(Renderer* ren);

struct ClipScope
{
    Renderer* cache;

    ClipScope(Renderer* c, const rect<dip>& r);
    ~ClipScope();
};

using CreateAppFunction = std::function<std::unique_ptr<App> (const StartupArguments&)>;
int run_main(int argc, char** argv, CreateAppFunction create_app);
