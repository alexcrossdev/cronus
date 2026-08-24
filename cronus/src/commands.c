#include "commands.h"

#include <cronus/cache.h>
#include <cronus/timeline.h>

#include <stdio.h>
#include <string.h>

static const char *get_path(const cli_args *args)
{
	if (args->path != NULL)
		return args->path;

	return ".";
}

int command_init(const cli_args *args)
{
	const char *path = get_path(args);

	if (args->verbose)
		printf("Initializing timeline: %s\n", path);

	/*
	 * Put your timeline initialization here.
	 *
	 * For example, once your library exposes an init function:
	 *
	 *     timeline_init(path);
	 */

	printf(
		"Initialized timeline in %s\n",
		path
	);

	return 0;
}

int command_status(const cli_args *args)
{
	const char *path = get_path(args);

	timeline *timeline = timeline_open(
		path,
		path
	);

	if (timeline == NULL) {
		fprintf(
			stderr,
			"cronus: unable to open timeline: %s\n",
			path
		);

		return 1;
	}

	cache_changes changes =
		cache_get_changed_entries(timeline);

	if (changes.modified_count != 0) {
		printf("Modified:\n");

		for (u32 i = 0; i < changes.modified_count; i++) {
			printf(
				"  %s\n",
				cache_entry_path(
					changes.modified[i]
				)
			);
		}

		printf("\n");
	}

	if (changes.deleted_count != 0) {
		printf("Deleted:\n");

		for (u32 i = 0; i < changes.deleted_count; i++) {
			printf(
				"  %s\n",
				cache_entry_path(
					changes.deleted[i]
				)
			);
		}

		printf("\n");
	}

	if (changes.new_count != 0) {
		printf("New:\n");

		for (u32 i = 0; i < changes.new_count; i++) {
			printf(
				"  %s\n",
				cache_entry_path(
					changes.new[i]
				)
			);
		}

		printf("\n");
	}

	if (changes.modified_count == 0 &&
	    changes.deleted_count == 0 &&
	    changes.new_count == 0) {
		printf("Working tree clean\n");
	}

	cache_changes_destroy(&changes);
	timeline_close(timeline);

	return 0;
}

int command_cache(const cli_args *args)
{
	const char *path = get_path(args);

	timeline *timeline = timeline_open(
		path,
		path
	);

	if (timeline == NULL) {
		fprintf(
			stderr,
			"cronus: unable to open timeline: %s\n",
			path
		);

		return 1;
	}

	if (args->subcommand != NULL) {
		if (strcmp(args->subcommand, "read") == 0) {
			cache_read(timeline);
		} else {
			fprintf(
				stderr,
				"cronus: unknown cache command: %s\n",
				args->subcommand
			);

			timeline_close(timeline);
			return 1;
		}
	} else {
		if (!cache_write(timeline)) {
			fprintf(
				stderr,
				"cronus: failed to update cache\n"
			);

			timeline_close(timeline);
			return 1;
		}

		if (args->verbose)
			printf("Cache updated\n");
	}

	timeline_close(timeline);

	return 0;
}
