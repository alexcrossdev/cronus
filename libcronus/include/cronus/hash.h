#ifndef HASH_H
#define HASH_H

#include "cronus/types.h"

b8 hash_to_hex(u8 *hash, u32 hash_len, char *output, u32 output_size);
b8 string_to_hash(const char *string);

u32 sha512_file(const char *filename, u8 hash[64]);
void sha512_hash(const void *data, u32 length, u8 output[64]);

#endif
