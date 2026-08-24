#include "args.h"

#include <stdio.h>
#include <string.h>

static void args_init(cli_args *args)
{
	*args = (cli_args){0};
}

b8 cli_args_parse(
	int argc,
	char **argv,
	cli_args *args
)
{
	args_init(args);

	if (argc < 2) {
		args->help = true;
		return true;
	}

	for (int i = 1; i < argc; i++) {
		const char *arg = argv[i];

		if (strcmp(arg, "-h") == 0 ||
		    strcmp(arg, "--help") == 0) {
			args->help = true;
			continue;
		}

		if (strcmp(arg, "-v") == 0 ||
		    strcmp(arg, "--verbose") == 0) {
			args->verbose = true;
			continue;
		}

		if (strcmp(arg, "--version") == 0) {
			args->version = true;
			continue;
		}

		/*
		 * First non-option argument is the command.
		 */
		if (args->command == NULL) {
			args->command = arg;
			continue;
		}

		/*
		 * Second non-option argument is a subcommand.
		 */
		if (args->subcommand == NULL) {
			args->subcommand = arg;
			continue;
		}

		/*
		 * Remaining positional argument is treated as a path
		 * unless the command has another meaning for it.
		 */
		if (args->path == NULL) {
			args->path = arg;
			continue;
		}

		fprintf(
			stderr,
			"cronus: unexpected argument: %s\n",
			arg
		);

		return false;
	}

	return true;
}

void cli_args_print_help(void)
{
	printf(
		"Usage: cronus <command> [options] [arguments]\n"
		"\n"
		"Commands:\n"
		"  init                 Initialize a timeline\n"
		"  status               Show working tree changes\n"
		"  cache                Update the cache\n"
		"  cache read           Read the current cache\n"
		"\n"
		"Options:\n"
		"  -h, --help           Show this help\n"
		"  -v, --verbose        Enable verbose output\n"
		"      --version        Show version\n"
		"\n"
	);
}
