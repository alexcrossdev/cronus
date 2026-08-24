#ifndef ARGS_H
#define ARGS_H

#include "cronus/types.h"

typedef struct cli_args {
	const char *command;
	const char *subcommand;

	const char *path;
	const char *message;

	b8 verbose;
	b8 help;
	b8 version;
} cli_args;

b8 cli_args_parse(int argc, char **argv, cli_args *args);
void cli_args_print_help(void);

#endif
