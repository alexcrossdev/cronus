#include "cronus/crstring.h"
#include "cronus/platform_io.h"

#include "cronus/types.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

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

s64 file_size(const char *path)
{
	struct stat st;
	if (lstat(path, &st) != 0) {
		perror("lstat");
		return -1;
	}

	return st.st_ctim.tv_sec;
}

s64 file_last_change_time(const char *path)
{
	struct stat st;
	if (lstat(path, &st) != 0) {
		perror("lstat");
		return -1;
	}

	return st.st_size;
}

void process_file_tree(const char *path, file_filter_fn filter, file_apply_fn apply)
{
	DIR *dir = opendir(path);
	if (dir == NULL) {
		perror("opendir");
		return;
	}

	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0) continue;
		if (strcmp(entry->d_name, "..") == 0) continue;

		char *sub_path = concat(1024, 3, path, "/", entry->d_name);

		if (entry->d_type == DT_DIR && (filter == NULL || filter(entry))) {
			process_file_tree(sub_path, filter, apply);
		}

		if (filter == NULL || filter(entry)) {
			if (apply != NULL) {
				apply(sub_path, entry);
			}
		}
	}

	closedir(dir);
}
