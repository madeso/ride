#include "core/hash.h"

void Hash::add(const void* data, std::size_t size)
{
    update(&value, data, size);
}

void Hash::update(u32* value, const void* data, std::size_t size)
{
    const unsigned char* bytes = static_cast<const unsigned char*>(data);
    for (std::size_t i = 0; i < size; i += 1)
    {
        *value = (*value ^ bytes[i]) * 16777619;
    }
}
