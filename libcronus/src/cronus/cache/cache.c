#include "cronus/cache.h"
#include "cronus/blob.h"
#include "cronus/crstring.h"
#include "cronus/platform_io.h"
#include "cronus/timeline.h"
#include "cronus/types.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct timeline {
	char *timeline_path;
	char *working_tree;
};

struct cache_entry {
	s64 size;
	s64 time;
	char path[1024];
};

static blob *cache;

/* TODO: Remove temporary filter for ignore file */
static int filter(const struct dirent *entry)
{
	(void)entry;

	if (strcmp(entry->d_name, ".cronus") == 0) return false;
	return true;
}

static char *cache_path(const char *timeline_path)
{
	return concat(1024, 2, timeline_path, "/.cronus/cache");
}

static void apply(const char *full_path, const struct dirent *entry)
{
	if (entry->d_type != DT_REG)
		return;

	cache_entry ent = {
		.size = file_size(full_path),
		.time = file_last_change_time(full_path),
	};

	snprintf(
		ent.path,
		sizeof(ent.path),
		"%s",
		full_path
	);

	blob_entry *be = blob_entry_create(
		(void *)&ent,
		sizeof(ent)
	);

	if (be == NULL)
		return;

	blob_write_entry(cache, be);
	blob_entry_destroy(be);
}

b8 cache_write(timeline *timeline)
{
	char *path = cache_path(timeline->timeline_path);

	cache = blob_create(path);

	if (cache == NULL) {
		free(path);
		return failure;
	}

	process_file_tree(
		timeline->working_tree,
		filter,
		apply
	);

	blob_close(cache);
	free(path);

	return success;
}

static cache_entry **current;
static u32 current_count;

static void apply_current(
	const char *full_path,
	const struct dirent *entry
)
{
	if (entry->d_type != DT_REG)
		return;

	cache_entry *entry_current = malloc(sizeof(*entry_current));

	if (entry_current == NULL)
		return;

	entry_current->size = file_size(full_path);
	entry_current->time = file_last_change_time(full_path);

	snprintf(
		entry_current->path,
		sizeof(entry_current->path),
		"%s",
		full_path
	);

	cache_entry **tmp = realloc(
		current,
		(current_count + 1) * sizeof(*current)
	);

	if (tmp == NULL) {
		free(entry_current);
		return;
	}

	current = tmp;
	current[current_count] = entry_current;
	current_count++;
}

static void collect_current_entries(const char *working_tree)
{
	current = NULL;
	current_count = 0;

	process_file_tree(
		working_tree,
		filter,
		apply_current
	);
}

static cache_entry *find_entry(
	cache_entry **entries,
	u32 count,
	const char *path
)
{
	for (u32 i = 0; i < count; i++) {
		if (entries[i] == NULL)
			continue;

		if (strcmp(entries[i]->path, path) == 0)
			return entries[i];
	}

	return NULL;
}

static b8 append_entry(
	cache_entry ***entries,
	u32 *count,
	cache_entry *entry
)
{
	cache_entry **tmp = realloc(
		*entries,
		(*count + 1) * sizeof(**entries)
	);

	if (tmp == NULL)
		return false;

	*entries = tmp;
	(*entries)[*count] = entry;
	(*count)++;

	return true;
}

static void free_entries(
	cache_entry **entries,
	u32 count
)
{
	for (u32 i = 0; i < count; i++)
		free(entries[i]);

	free(entries);
}

static cache_entry **read_cached_entries(
	const char *path,
	u32 *count
)
{
	*count = 0;

	blob *cache_file = blob_open(path);

	if (cache_file == NULL)
		return NULL;

	blob_entry *b_entry = blob_entry_create_empty();

	if (b_entry == NULL) {
		blob_close(cache_file);
		return NULL;
	}

	cache_entry **entries = NULL;

	while (blob_read_entry(cache_file, b_entry)) {
		const void *data = blob_entry_data(b_entry);

		cache_entry *entry = malloc(sizeof(*entry));

		if (entry == NULL)
			break;

		memcpy(
			entry,
			data,
			sizeof(*entry)
		);

		cache_entry **tmp = realloc(
			entries,
			(*count + 1) * sizeof(*entries)
		);

		if (tmp == NULL) {
			free(entry);
			break;
		}

		entries = tmp;
		entries[*count] = entry;
		(*count)++;
	}

	blob_entry_destroy(b_entry);
	blob_close(cache_file);

	return entries;
}

static void find_new_entries(
	cache_entry **cached,
	u32 cached_count,
	cache_changes *changes
)
{
	for (u32 i = 0; i < current_count; i++) {
		cache_entry *entry = current[i];

		if (entry == NULL)
			continue;

		if (find_entry(
			cached,
			cached_count,
			entry->path
		) != NULL) {
			continue;
		}

		if (!append_entry(
			&changes->new,
			&changes->new_count,
			entry
		)) {
			continue;
		}

		current[i] = NULL;
	}
}

static void find_modified_entries(
	cache_entry **cached,
	u32 cached_count,
	cache_changes *changes
)
{
	for (u32 i = 0; i < current_count; i++) {
		cache_entry *current_entry = current[i];

		if (current_entry == NULL)
			continue;

		cache_entry *cached_entry = find_entry(
			cached,
			cached_count,
			current_entry->path
		);

		if (cached_entry == NULL)
			continue;

		if (current_entry->size == cached_entry->size &&
		    current_entry->time == cached_entry->time) {
			continue;
		}

		if (!append_entry(
			&changes->modified,
			&changes->modified_count,
			current_entry
		)) {
			continue;
		}

		current[i] = NULL;
	}
}

static void find_deleted_entries(
	cache_entry **cached,
	u32 cached_count,
	cache_changes *changes
)
{
	for (u32 i = 0; i < cached_count; i++) {
		cache_entry *cached_entry = cached[i];

		if (cached_entry == NULL)
			continue;

		b8 found = false;

		if (find_entry(
			current,
			current_count,
			cached_entry->path
		) != NULL) {
			found = true;
		}

		if (!found &&
		    find_entry(
			changes->modified,
			changes->modified_count,
			cached_entry->path
		    ) != NULL) {
			found = true;
		}

		if (!found &&
		    find_entry(
			changes->new,
			changes->new_count,
			cached_entry->path
		    ) != NULL) {
			found = true;
		}

		if (found)
			continue;

		if (!append_entry(
			&changes->deleted,
			&changes->deleted_count,
			cached_entry
		)) {
			continue;
		}

		cached[i] = NULL;
	}
}

cache_changes cache_get_changed_entries(timeline *timeline)
{
	cache_changes changes = {0};

	char *path = cache_path(timeline->timeline_path);

	if (path == NULL)
		return changes;

	u32 cached_count = 0;

	cache_entry **cached = read_cached_entries(
		path,
		&cached_count
	);

	free(path);

	collect_current_entries(
		timeline->working_tree
	);

	find_new_entries(
		cached,
		cached_count,
		&changes
	);

	find_modified_entries(
		cached,
		cached_count,
		&changes
	);

	find_deleted_entries(
		cached,
		cached_count,
		&changes
	);

	for (u32 i = 0; i < current_count; i++)
		free(current[i]);

	free(current);

	current = NULL;
	current_count = 0;

	for (u32 i = 0; i < cached_count; i++)
		free(cached[i]);

	free(cached);

	return changes;
}

b8 cache_read(timeline *timeline)
{
	char *path = cache_path(timeline->timeline_path);

	if (path == NULL)
		return failure;

	cache = blob_open(path);

	if (cache == NULL) {
		free(path);
		return failure;
	}

	blob_entry *b_entry = blob_entry_create_empty();

	if (b_entry == NULL) {
		blob_close(cache);
		free(path);
		return failure;
	}

	cache_entry c_entry;

	while (blob_read_entry(cache, b_entry)) {
		const void *data = blob_entry_data(b_entry);

		memcpy(
			&c_entry,
			data,
			sizeof(c_entry)
		);

		printf(
			"Size: %li, Last: %li, Path: %s\n",
			c_entry.size,
			c_entry.time,
			c_entry.path
		);
	}

	blob_entry_destroy(b_entry);
	blob_close(cache);
	free(path);

	return success;
}

void cache_changes_destroy(cache_changes *changes)
{
	if (changes == NULL)
		return;

	free_entries(
		changes->modified,
		changes->modified_count
	);

	free_entries(
		changes->deleted,
		changes->deleted_count
	);

	free_entries(
		changes->new,
		changes->new_count
	);

	*changes = (cache_changes){0};
}

char *cache_entry_path(cache_entry *entry)
{
	if (entry == NULL)
		return NULL;

	return entry->path;
}
