#ifndef COMMANDS_H
#define COMMANDS_H

#include "args.h"

int command_init(const cli_args *args);
int command_status(const cli_args *args);
int command_cache(const cli_args *args);
int command_hash(const cli_args *args);

#endif
