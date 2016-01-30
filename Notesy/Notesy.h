#pragma once
#include <vector>
#include "command.h"

std::vector<NotesyCommand *> init_commands();
void init_config();
void list_collections();
void test(char *args[]);
void clean_up_commands(std::vector<NotesyCommand *>);

// for debugging
void setCrtFlags();



