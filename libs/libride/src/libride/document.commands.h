#pragma once

#include <functional>

struct position;
struct CommandList;
struct VirtualView;

position end_of_line(const position& p);

using active_view_or_null_getter = std::function<VirtualView* ()>;

void add_edit_commands(CommandList* list, active_view_or_null_getter getter);
