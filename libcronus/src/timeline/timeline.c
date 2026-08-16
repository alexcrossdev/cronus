
#include "timeline.h"
#include <stdlib.h>
#include <string.h>

struct timeline
{
	char *timeline_path;
	char *working_path;
};

timeline *timeline_open(const char *timeline_path, const char *working_path)
{
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

	len = strlen(working_path);
	out_timeline->working_path = (char *)malloc(len + 1);
	if (out_timeline->working_path == NULL)
		return NULL;

	strncpy(out_timeline->working_path, working_path, len);
	out_timeline->working_path[len] = '\0';

	return out_timeline;
}


b8 timeline_close(timeline *timeline)
{

	free(timeline->timeline_path);
	free(timeline->working_path);
	free(timeline);

	return 0;
}
