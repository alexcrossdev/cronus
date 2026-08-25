#ifndef HASH_H
#define HASH_H


#include "cronus/types.h"
b8 hash_to_string(u8 *hash, u32 hash_len, char *output, u32 output_size);
b8 string_to_hash(const char *string);

#endif
