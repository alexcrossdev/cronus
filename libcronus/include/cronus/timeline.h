#ifndef TIMELINE_H
#define TIMELINE_H

#include "types.h"

typedef struct timeline timeline;

timeline *timeline_open(const char *timeline_path, const char *working_tree);
timeline *timeline_create(const char *timeline_path, const char *working_tree, b8 is_bare);
b8 timeline_close(timeline *timeline);

b8 is_valid_timeline(const char *path);

#endif
