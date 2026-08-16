#include "cronus/crstring.h"
#include "cronus/platform_io.h"
#include "cronus/types.h"
#include <cronus/timeline.h>

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static char doc[] = "Cronus CLI";

enum ARG_KEYS {
	KEY_TIMELINE_PATH = 1000,
	KEY_WORKING_TREE,
	KEY_IN_WORKING_TREE
};

struct arguments {
	b8 isVerbose;
	const char *timeline_path;
	const char *working_tree;
};

static struct argp_option global_options[] = {
	{"verbose", 'v', 0, 0, "Returns verbose output", 0},
	{"timeline-path", KEY_TIMELINE_PATH, "PATH", 0, "Specifies targeted timeline", 1},
	{"working-tree", KEY_WORKING_TREE, "PATH", 0, "Specifies working tree", 1},
	{0}
};

static error_t parse_global_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;
	switch (key) {
		case 'v': 
			arguments->isVerbose = true; 
			break;
		case KEY_TIMELINE_PATH:
			arguments->timeline_path = arg;
			break;
		case KEY_WORKING_TREE:
			arguments->working_tree = arg;
			break;
		case ARGP_KEY_ARG:
			return ARGP_ERR_UNKNOWN;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp global_argp = {global_options, parse_global_opt, "[command]", doc, 0, 0, 0};

static struct argp_option parse_sub_options[] = {
	{"is-in-working-tree", KEY_IN_WORKING_TREE, 0, 0, "Returns true if in working tree dir", 0},
	{0}
};

static error_t parse_sub_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;
	(void)arguments;
	(void)arg;

	switch (key) {
		case KEY_IN_WORKING_TREE: {
			char cwd[1024];
			if (!get_cwd(cwd, 1024)) return ARGP_ERR_UNKNOWN;

			if (is_valid_timeline(cwd)) {
				exit(0);
			} else {
				exit(1);
			}
			break;
		}
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp parse_sub_argp = {parse_sub_options, parse_sub_opt, 0, "Cronus internal parser utilities", 0, 0, 0};

int main(int argc, char **argv)
{
	struct arguments arguments = {
		.isVerbose = false,
		.timeline_path = ".",
		.working_tree = "."
	};

	int next_idx;
	argp_parse(&global_argp, argc, argv, ARGP_IN_ORDER, &next_idx, &arguments);

	if (next_idx < argc && strcmp(argv[next_idx], "parse") == 0) {
		argp_parse(&parse_sub_argp, argc - next_idx, argv + next_idx, 0, 0, &arguments);
	} else {
		fprintf(stderr, "cronus: missing or invalid command. See 'cronus --help'.\n");
		return 1;
	}

	return 0;
}
