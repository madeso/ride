#pragma once

#include <functional>

struct CommandList;
struct VirtualView;

using active_view_or_null_getter = std::function<VirtualView* ()>;

void add_edit_commands(CommandList* list, active_view_or_null_getter getter);
