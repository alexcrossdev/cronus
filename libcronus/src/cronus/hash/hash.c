#include "cronus/hash.h"
#include "cronus/types.h"
#include <stdio.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

b8 hash_to_hex(u8 *hash, u32 hash_len, char *output, u32 output_size)
{
	if (output_size < hash_len * 2 + 1) return failure;

	for (u32 i = 0; i < hash_len; i++) {
		snprintf(&output[i * 2], 3, "%02x", hash[i]);
	}

	return success;
}

b8 hex_to_hash(const char *hex, u8 *output, u32 output_size)
{
	u32 hex_length = 0;

	while(hex[hex_length] != '\0')
		hex_length++;

	if (hex_length % 2 != 0 || output_size < hex_length / 2)
		return failure;

	for (u32 i = 0; i < hex_length / 2; i++) {
		u32 value;

		if (sscanf(&hex[i * 2], "%2x", &value) != 1)
			return failure;

		output[i] = (u8)value;
	}

	return success;
}

void sha512_hash(const void *data, u32 length, u8 output[SHA512_DIGEST_LENGTH])
{
	SHA512(data, length, output);
}

u32 sha512_file(const char *filename, u8 hash[64])
{
	FILE *file = fopen(filename, "rb");

	if (file == NULL)
		return failure;

	EVP_MD_CTX *context = EVP_MD_CTX_new();

	if (context == NULL) {
		fclose(file);
		return failure;
	}

	if (EVP_DigestInit_ex(context, EVP_sha512(), NULL) != 1) {
		EVP_MD_CTX_free(context);
		return failure;
	}

	u8 buffer[8192];
	u32 bytes_read;

	while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
		if (EVP_DigestUpdate(context, buffer, bytes_read) != 1) {
			EVP_MD_CTX_free(context);
			fclose(file);
			return failure;
		}
	}

	if (ferror(file)) {
		EVP_MD_CTX_free(context);
		fclose(file);
		return failure;
	}

	u32 hash_length;

	if (EVP_DigestFinal_ex(context, hash, &hash_length) != 1) {
		EVP_MD_CTX_free(context);
		fclose(file);
		return failure;
	}

	EVP_MD_CTX_free(context);
	fclose(file);

	return hash_length == 64;
}
