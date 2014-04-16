/*
 * =====================================================================================
 *
 *       Filename:  la_array.c
 *    Description:  additional memory management
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "la_memory.h"

void *memory_set(void *buffer, int character, size_t size) {
	if (!size) return NULL;

	char *p;
	p = (char *)buffer;

	while (size) {
		*p++ = character;
		--size;
	}

	return buffer;
}

void *memory_zero(void *buffer, size_t size) { 
	if (!size) return NULL;

	/* write buffer */
	memset(buffer, '\0', size);

	/* suppress GCC- & LLVM-optimize */
	asm volatile("" : : "r"(&buffer) : "memory");

	/* convert type */
	char *p;
	p = (char *)buffer;

	/* make read-operation */
	if (size > 1) {
		if (p[0] > p[size - 1]) p[0] = p[size - 1];
		else p[size - 1] = p[0];
	} else {
		if (p[0] != '\0') p[0] = '\0';
	}

	return buffer;
}

void memory_print(void *buffer, size_t size) {
	if (!size) return;

	char *p;
	p = (char *)buffer;

	while (size) {
		putc(*p++, stdout);
		--size;
	}
}

#ifdef __cplusplus
namespace la {
	namespace memory {
		void *set(void *buffer, int character, size_t size) {
			return memory_set(buffer, character, size);
		}

		void *zero(void *buffer, size_t size) {
			return memory_zero(buffer, size);
		}

		void print(void *buffer, size_t size) {
			memory_print(buffer, size);
		}
	}
}
#endif
