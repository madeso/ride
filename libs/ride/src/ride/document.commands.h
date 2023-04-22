#pragma once

#include <functional>


namespace ride::libride
{


struct Position;
struct CommandList;
struct VirtualView;

Position end_of_line(const Position& p);

using active_view_or_null_getter = std::function<VirtualView* ()>;

void add_edit_commands(CommandList* list, active_view_or_null_getter getter);


}
