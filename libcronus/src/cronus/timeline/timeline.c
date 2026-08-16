#include "cronus/timeline.h"

#include "cronus/types.h"
#include "cronus/platform_io.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct timeline {
	char *timeline_path;
	char *working_tree;
};

static b8 is_valid_timeline(const char *timeline_path)
{

	char cronus_path[1024];
	int written = snprintf(cronus_path, sizeof(cronus_path), "%s/.cronus", timeline_path);
	if (written < 0 || (size_t)written >= sizeof(cronus_path)) {
		errno = ENAMETOOLONG;
		perror("timeline_path");
		return failure;
	}

	struct stat cronus_stats;
	if (stat(cronus_path, &cronus_stats) != 0) {
		if (strcmp(".", timeline_path) == 0) {
			fprintf(stderr, "Not a cronus timeline\n");
			return failure;
		}

		fprintf(stderr, "%s: Not a cronus timeline\n", timeline_path);
		return failure;
	}

	return success;
}

timeline *timeline_open(const char *timeline_path, const char *working_tree)
{
	if (!is_valid_path(timeline_path))
		return NULL;

	if (!is_valid_timeline(timeline_path))
		return NULL;

	if (!is_valid_path(working_tree))
		return NULL;

	timeline *out_timeline = (struct timeline *)malloc(sizeof(timeline));
	int len;

	if (out_timeline == NULL)
		return NULL;

	len = strlen(timeline_path);
	out_timeline->timeline_path = (char *)malloc(len + 1);
	if (out_timeline->timeline_path == NULL)
		return NULL;

	strncpy(out_timeline->timeline_path, timeline_path, len);
	out_timeline->timeline_path[len] = '\0';

	len = strlen(working_tree);
	out_timeline->working_tree = (char *)malloc(len + 1);
	if (out_timeline->working_tree == NULL)
		return NULL;

	strncpy(out_timeline->working_tree, working_tree, len);
	out_timeline->working_tree[len] = '\0';

	return out_timeline;
}


b8 timeline_close(timeline *timeline)
{

	free(timeline->timeline_path);
	free(timeline->working_tree);
	free(timeline);

	return 0;
}
