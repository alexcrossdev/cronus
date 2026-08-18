#ifndef PLATFORM_IO_H
#define PLATFORM_IO_H

#include "types.h"
#include <dirent.h>

b8 is_valid_dir(const char *path, b8 output);
b8 dir_create(const char *path, u32 perms);
b8 get_cwd(char *out_cwd, s32 size);

typedef int (*file_filter_fn)(const struct dirent *entry);
typedef void (*file_apply_fn)(const char *full_path, const struct dirent *entry);
void process_file_tree(const char *path, file_filter_fn filter, file_apply_fn apply);

#endif
