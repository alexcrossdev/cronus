#ifndef TIMELINE_H
#define TIMELINE_H

#include "types.h"

typedef struct timeline timeline;

timeline *timeline_open(const char *timeline_path, const char *working_path);
b8 timeline_close(timeline *timeline);

#endif
