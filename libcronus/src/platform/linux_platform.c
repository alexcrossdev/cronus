#include "cronus/platform_io.h"

#include "cronus/types.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

b8 is_valid_dir(const char *path, b8 output)
{

	struct stat stats;

	if (stat(path, &stats) != 0) {
		if (output) perror(path);
		return failure;
	} else if (!S_ISDIR(stats.st_mode)) {
		errno = ENOTDIR;
		if (output) perror(path);
		return failure;
	}

	return success;
}

b8 dir_create(const char *path, u32 perms)
{
	s32 check = mkdir(path, perms);

	return !check;
}

b8 get_cwd(char *out_cwd, s32 size)
{
	if (getcwd(out_cwd, size) == NULL) {
		perror("getcwd: ");
		return failure;
	}

	return success;
}
