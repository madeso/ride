#pragma once

#include <vector>
#include <cstdint>
#include <memory>

#include "embed/types.h"

#include "api/color.h"

struct Image
{
    std::vector<std::uint8_t> pixels;
    int width;
    int height;

    void setup(int width, int height);
    bool load(const embedded_binary& data);

    void set_color(int x, int y, const Color& c);
    Color get_color(int x, int y) const;
};

std::shared_ptr<Image> load_shared(const embedded_binary& data);