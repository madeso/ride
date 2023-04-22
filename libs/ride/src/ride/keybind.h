#pragma once

#include <map>

#include "api/key.h"


namespace ride::libride
{


struct CommandList;

struct KeyBind
{
    std::map<api::Stroke, std::vector<std::string>> commands;
    
    void add(const api::Stroke& stroke, std::vector<std::string> command_order);
    bool run(const api::Stroke& stroke, CommandList* command_list);
};


}

