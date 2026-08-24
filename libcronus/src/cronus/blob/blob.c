#include "cronus/blob.h"
#include "cronus/types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct blob {
	FILE *file;
};

struct blob_entry {
	u8 *data;
	u32 size;
};

blob *blob_open(const char *blob_path)
{
	if (blob_path == NULL) {
		return NULL;
	}

	FILE *file = fopen(blob_path, "rb");
	if (file == NULL) {
		fprintf(stderr, "IO Error: could not open blob\n");
		return NULL;
	}

	blob *out_blob = malloc(sizeof *out_blob);
	if (out_blob == NULL) {
		fclose(file);
		return NULL;
	}

	out_blob->file = file;
	return out_blob;
}

blob *blob_create(const char *blob_path)
{
	if (blob_path == NULL) {
		return NULL;
	}

	FILE *file = fopen(blob_path, "wb");
	if (file == NULL) {
		fprintf(stderr, "IO Error: could not create blob\n");
		return NULL;
	}

	blob *out_blob = malloc(sizeof *out_blob);
	if (out_blob == NULL) {
		fclose(file);
		return NULL;
	}

	out_blob->file = file;
	return out_blob;
}

b8 blob_close(blob *blob)
{
	if (blob == NULL) {
		return failure;
	}

	b8 result = success;

	if (blob->file != NULL && fclose(blob->file) != 0) {
		result = failure;
	}

	free(blob);
	return result;
}

blob_entry *blob_entry_create(const void *data, u32 size)
{
	if (size > 0 && data == NULL) {
		return NULL;
	}

	blob_entry *entry = malloc(sizeof *entry);
	if (entry == NULL) {
		return NULL;
	}

	entry->data = NULL;
	entry->size = size;

	if (size > 0) {
		entry->data = malloc(size);

		if (entry->data == NULL) {
			free(entry);
			return NULL;
		}

		memcpy(entry->data, data, size);
	}

	return entry;
}

void blob_entry_destroy(blob_entry *entry)
{
	if (entry == NULL) {
		return;
	}

	free(entry->data);
	free(entry);
}

blob_entry *blob_entry_create_empty(void)
{
    blob_entry *entry = malloc(sizeof *entry);

    if (entry == NULL) {
        return NULL;
    }

    entry->data = NULL;
    entry->size = 0;

    return entry;
}

const void *blob_entry_data(const blob_entry *entry)
{
	if (entry == NULL) {
		return NULL;
	}

	return entry->data;
}

u32 blob_entry_size(const blob_entry *entry)
{
	if (entry == NULL) {
		return 0;
	}

	return entry->size;
}

b8 blob_write_entry(blob *blob, const blob_entry *entry)
{
	if (blob == NULL || blob->file == NULL || entry == NULL) {
		return failure;
	}

	if (entry->size > 0 && entry->data == NULL) {
		return failure;
	}

	if (fwrite(&entry->size, sizeof entry->size, 1, blob->file) != 1) {
		return failure;
	}

	if (entry->size > 0 &&
			fwrite(entry->data, 1, entry->size, blob->file) != entry->size) {
		return failure;
	}

	return success;
}

b8 blob_read_entry(blob *blob, blob_entry *entry)
{
	if (blob == NULL || blob->file == NULL || entry == NULL) {
		return failure;
	}

	free(entry->data);

	entry->data = NULL;
	entry->size = 0;

	u32 size;

	if (fread(&size, sizeof size, 1, blob->file) != 1) {
		return failure;
	}

	if (size > 0) {
		entry->data = malloc(size);

		if (entry->data == NULL) {
			return failure;
		}

		if (fread(entry->data, 1, size, blob->file) != size) {
			free(entry->data);
			entry->data = NULL;
			return failure;
		}
	}

	entry->size = size;
	return success;
}
