#include "cronus/platform_io.h"

#include "cronus/types.h"

#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>

b8 is_valid_path(const char *path)
{

	struct stat stats;

	if (stat(path, &stats) != 0) {
		perror(path);
		return failure;
	} else if (!S_ISDIR(stats.st_mode)) {
		errno = ENOTDIR;
		perror(path);
		return failure;
	}

	return success;
}
