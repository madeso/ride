#include "ride/command.h"

#include <iostream>


Command::Command
(
    predicate_fun pe,
    perform_fun pm
)
: predicate(pe)
, perform(pm)
{
}

CommandAdder& CommandAdder::operator()(const std::string& name, perform_fun perform)
{
    root->add(name, predicate, perform);
    return *this;
}


CommandAdder CommandList::add()
{
    return add([]() -> bool { return true;});
}

CommandAdder CommandList::add(predicate_fun predicate)
{
    return {predicate, this};
}

void CommandList::add(const std::string& name, predicate_fun predicate, perform_fun command)
{
    auto found = map.find(name);
    if(found != map.end())
    {
        std::cerr << "command already exists: " << name << "\n";
        return;
    }
    map.emplace(name, std::make_shared<Command>(predicate, command));
}

std::vector<std::string> CommandList::get_all_valid()
{
    std::vector<std::string> r;

    for(auto& e: map)
    {
        if(e.second->predicate())
        {
            r.emplace_back(e.first);
        }
    }

    return r;
}

bool CommandList::perform(const std::string& name)
{
    const auto cmd = map.find(name);
    if( cmd != map.end() && cmd->second->predicate())
    {
        cmd->second->perform();
        return true;
    }
    std::cerr << "failed to evaluate " << name << "\n";
    return false;
}
