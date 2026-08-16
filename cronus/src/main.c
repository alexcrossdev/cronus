#include <cronus/timeline.h>

#include <argp.h>
#include <stdio.h>

static char doc[] = "Cronus CLI";
static char args_doc[] = "[test]";

enum ARG_KEYS {
	KEY_TIMELINE_PATH = 1000,
	KEY_WORKING_TREE
};

static struct argp_option options[] = {
	{"verbose", 'v', 0, 0, "Returns verbose output", 0},
	{"timeline-path", KEY_TIMELINE_PATH, "PATH", 1, "Specifies targeted timeline", 1},
	{"working-tree", KEY_WORKING_TREE, "PATH", 1, "Specifies working tree", 1},
	{0}
};

struct arguments {
	b8 isCaseInsensitive;
	b8 isVerbose;

	const char *timeline_path;
	const char *working_tree;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
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

		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char **argv)
{
	struct arguments arguments = {
		.isVerbose = false,
		.isCaseInsensitive = false,
		.timeline_path = ".",
		.working_tree = "."
	};

	argp_parse(&argp, argc, argv, 0, 0, &arguments);


	timeline *tl = timeline_open(arguments.timeline_path, arguments.working_tree);

	if (tl == NULL)
		return 1;

	timeline_close(tl);
}
