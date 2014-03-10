/*
 * =====================================================================================
 *
 *       Filename:  la_array.h
 *    Description:  simple string-vector
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_ARRAY_H
#define LA_ARRAY_H

#include <stddef.h>
typedef struct la_array ARRAY;

#ifdef __cplusplus
extern "C" {
#endif

	ARRAY *array_new();
	void array_free(ARRAY *self);
	void array_reset(ARRAY *self);
	void array_setSize(ARRAY *self, size_t size);
	size_t array_getSize(ARRAY *self);
	void array_show(ARRAY *self);
	void array_setString(ARRAY *self, unsigned int idx, const char *str);
	char *array_getString(ARRAY *self, unsigned int idx);
	char **array_getArray(ARRAY *self);
	void array_showArray(char **array);
	void array_freeArray(char **array);
	void array_loadFromString(ARRAY *self, const char *str, const char sep);
	
#ifdef __cplusplus
}
#include <string>

namespace la {
	namespace array {
		class Array {
			private:
				ARRAY *obj;
			public:
			Array();
			~Array();
		};
	}
}
#endif

#endif

