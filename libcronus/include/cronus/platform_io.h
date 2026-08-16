#ifndef PLATFORM_IO_H
#define PLATFORM_IO_H

#include "types.h"

b8 is_valid_dir(const char *path, b8 output);
b8 dir_create(const char *path, u32 perms);
b8 get_cwd(char *out_cwd, s32 size);

#endif
