#pragma once

#include <map>

#include "core/string.h"

template<typename Enum>
struct EnumStringMap
{
    std::map<std::string, Enum> string_to_enum;
    std::map<Enum, std::string> enum_to_string;

    void bind(Enum enum_value, const std::string& name)
    {
        const auto id = to_lower(name);
        assert(enum_to_string.find(enum_value) == enum_to_string.end());
        assert(string_to_enum.find(id) == string_to_enum.end());

        string_to_enum[id] = enum_value;
        enum_to_string[enum_value] = name;
    }

    std::optional<Enum> from_string(const std::string& name) const
    {
        const auto id = to_lower(name);
        const auto found_enum = string_to_enum.find(id);
        if(found_enum == string_to_enum.end())
        {
            return {};
        }
        return found_enum->second;
    }

    std::string to_string(Enum enum_value) const
    {
        const auto found_name = enum_to_string.find(enum_value);
        if(found_name == enum_to_string.end())
        {
            return "<?>";
        }
        return found_name->second;
    }
};
