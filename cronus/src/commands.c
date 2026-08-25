#include "commands.h"
#include "cronus/types.h"

#include <cronus/cache.h>
#include <cronus/timeline.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *get_timeline_dir(const cli_args *args)
{
	if (args->timeline_dir != NULL)
		return args->timeline_dir;

	return ".";
}

static const char *get_working_dir(const cli_args *args)
{
	if (args->working_dir != NULL)
		return args->working_dir;

	return ".";
}

int command_init(const cli_args *args)
{
	const char *path = get_timeline_dir(args);
	char *abs_path;

	abs_path = realpath(path, NULL);
	if (abs_path == NULL)
		return 1;

	if (args->verbose)
		printf("Initializing timeline: %s\n", abs_path);

	printf(
		"Initialized timeline in %s\n",
		abs_path
	);
	free(abs_path);

	return 0;
}

int command_status(const cli_args *args)
{

	timeline *timeline = timeline_open(
		get_timeline_dir(args),
		get_working_dir(args)
	);

	if (timeline == NULL) {
		fprintf(
			stderr,
			"cronus: unable to open timeline: %s\n",
			args->timeline_dir
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
	const char *timeline_path = get_timeline_dir(args);
	const char *working_path = get_working_dir(args);

	timeline *timeline = timeline_open(
		timeline_path,
		working_path
	);

	if (timeline == NULL) {
		fprintf(
			stderr,
			"cronus: unable to open timeline: %s\n",
			timeline_path
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

/*int command_hash(const cli_args *args)
{
	
}*/
