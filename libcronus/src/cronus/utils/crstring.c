#include "cronus/crstring.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

char *concat(s32 buffer_size, s32 arg_count, ...)
{
	if (buffer_size <= 0) return NULL;

	char *result = (char *)malloc(buffer_size);
	if (result == NULL) return NULL;

	result[0] = '\0';

	va_list args;
	va_start(args, arg_count);

	char *dest = result;
	s32 space_left = buffer_size - 1;

	for (s32 i = 0; i < arg_count && space_left > 0; i++) {
		const char *next_str = va_arg(args, const char *);
		if (!next_str) continue;

		s32 written = snprintf(dest, space_left + 1, "%s", next_str);

		if (written < 0) {
			break;
		}

		if (written >= space_left) {
			dest += space_left;
			space_left = 0;
		} else {
			dest += written;
			space_left -= written;
		}
	}

	va_end(args);
	return result;
}
