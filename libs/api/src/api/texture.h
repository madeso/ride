#pragma once

struct Texture
{
    Texture() = default;
    virtual ~Texture() = default;

    virtual int get_width() const = 0;
    virtual int get_height() const = 0;
};

