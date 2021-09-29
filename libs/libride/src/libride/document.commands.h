#pragma once

#include <functional>

struct CommandList;
struct Document;

using active_document_or_null_getter = std::function<Document* ()>;

void add_edit_commands(CommandList* list, active_document_or_null_getter getter);
