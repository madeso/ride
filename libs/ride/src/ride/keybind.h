#pragma once

#include <vector>
#include <map>
#include <string>

#include "api/key.h"

struct CommandList;

struct KeyBind
{
    std::map<Stroke, std::vector<std::string>> commands;
    
    void add(const Stroke& stroke, std::vector<std::string> command_order);
    bool run(const Stroke& stroke, CommandList* command_list);
};
