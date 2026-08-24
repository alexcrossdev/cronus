#ifndef CACHE_H
#define CACHE_H

#include "cronus/timeline.h"
#include "cronus/types.h"

typedef struct cache_entry cache_entry;
typedef struct cache_changes {
	cache_entry **modified;
	u32 modified_count;

	cache_entry **deleted;
	u32 deleted_count;

	cache_entry **new;
	u32 new_count;
} cache_changes;

b8 cache_write(timeline *timeline);
b8 cache_read(timeline *timeline);

cache_changes cache_get_changed_entries(timeline *timeline);
char *cache_entry_path(cache_entry *entry);
void cache_changes_destroy(cache_changes *changes);

#endif
