#pragma once

#include <cstddef> // std::size_t

#include "base/ints.h"

/** 32bit fnv-1a hash.
*/
struct Hash
{
    static constexpr u32 INITIAL = 2166136261;
    u32 value = INITIAL;

    static void update(u32* value, const void* data, std::size_t size);

    void add(const void* data, std::size_t size);

    template <typename T>
    Hash& operator<<(const T& t)
    {
        add(&t, sizeof(T));
        return *this;
    }
};

