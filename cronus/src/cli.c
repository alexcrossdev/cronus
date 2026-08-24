#include "cli.h"

#include "args.h"
#include "commands.h"

#include <stdio.h>
#include <string.h>

typedef int (*command_function)(
	const cli_args *args
);

typedef struct cli_command {
	const char *name;
	command_function function;
} cli_command;

static const cli_command commands[] = {
	{
		.name = "init",
		.function = command_init,
	},
	{
		.name = "status",
		.function = command_status,
	},
	{
		.name = "cache",
		.function = command_cache,
	},
};

static const size_t command_count =
	sizeof(commands) / sizeof(commands[0]);

static command_function find_command(
	const char *name
)
{
	for (size_t i = 0; i < command_count; i++) {
		if (strcmp(commands[i].name, name) == 0)
			return commands[i].function;
	}

	return NULL;
}

static int print_version(void)
{
	printf("cronus 0.1.0\n");
	return 0;
}

int cli_run(int argc, char **argv)
{
	cli_args args;

	if (!cli_args_parse(argc, argv, &args)) {
		return 1;
	}

	if (args.help) {
		cli_args_print_help();
		return 0;
	}

	if (args.version) {
		return print_version();
	}

	if (args.command == NULL) {
		cli_args_print_help();
		return 1;
	}

	command_function function =
		find_command(args.command);

	if (function == NULL) {
		fprintf(
			stderr,
			"cronus: unknown command: %s\n",
			args.command
		);

		fprintf(
			stderr,
			"Run 'cronus --help' for usage.\n"
		);

		return 1;
	}

	return function(&args);
}
