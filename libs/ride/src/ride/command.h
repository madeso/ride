#pragma once

#include <functional>
#include <map>
#include <memory>

using predicate_fun = std::function<bool ()>;
using perform_fun = std::function<void ()>;

struct CommandList;

struct Command
{
    predicate_fun predicate;
    perform_fun perform;

    Command
    (
        predicate_fun pe,
        perform_fun pm
    );
};

struct CommandAdder
{
    predicate_fun predicate;
    CommandList* root;

    CommandAdder& operator()(const std::string& name, perform_fun perform);
};

struct CommandList
{
    std::map<std::string, std::shared_ptr<Command> > map;


    CommandAdder add();
    CommandAdder add(predicate_fun predicate);
    void add(const std::string& name, predicate_fun predicate, perform_fun command);

    std::vector<std::string> get_all_valid();

    bool perform(const std::string& name);
};
