#ifndef BLOB_H
#define BLOB_H

#include "cronus/types.h"

typedef struct blob blob;
typedef struct blob_entry blob_entry;

blob *blob_open(const char *blob_path);
blob *blob_create(const char *blob_path);
b8 blob_close(blob *blob);

blob_entry *blob_entry_create(const void *data, u32 size);
blob_entry *blob_entry_create_empty(void);
void blob_entry_destroy(blob_entry *entry);

const void *blob_entry_data(const blob_entry *entry);
u32 blob_entry_size(const blob_entry *entry);

b8 blob_write_entry(blob *blob, const blob_entry *entry);
b8 blob_read_entry(blob *blob, blob_entry *entry);

#endif
