#include "ride/keybind.h"

#include "ride/command.h"



namespace ride::libride
{


void KeyBind::add(const api::Stroke& stroke, std::vector<std::string> command_order)
    {
    commands.insert({stroke, command_order});
}

bool KeyBind::run(const api::Stroke& stroke, CommandList* command_list)
{
    const auto found = commands.find(stroke);
    if(found != commands.end())
    {
        for(const auto& cmd: found->second)
        {
            if(command_list->perform(cmd))
            {
                return true;
            }
        }
    }

    return false;
}


}

