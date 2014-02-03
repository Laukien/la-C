/*
 * =====================================================================================
 *
 *       Filename:  la_stringbuffer.h
 *    Description:  variable-length sequence of characters (strings)
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_STRINGBUFFER_H
#define LA_STRINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct la_stringbuffer STRINGBUFFER;

STRINGBUFFER *stringbuffer_new();
void stringbuffer_free(STRINGBUFFER *ptr);
unsigned int stringbuffer_size(STRINGBUFFER *ptr); /* deprecated */
unsigned int stringbuffer_getLength(STRINGBUFFER *ptr);
void stringbuffer_append(STRINGBUFFER *ptr, const char *str);
void stringbuffer_insert(STRINGBUFFER *ptr, const char *str, unsigned int pos);
char *stringbuffer_text(STRINGBUFFER *ptr);     /* deprecated */
char *stringbuffer_getText(STRINGBUFFER *ptr);
char *stringbuffer_getTextPointer(STRINGBUFFER *ptr);
STRINGBUFFER *stringbuffer_clone(STRINGBUFFER *ptr);
void stringbuffer_reset(STRINGBUFFER *ptr);

#ifdef __cplusplus
}
#include <string>

namespace la {
	class stringbuffer {
		private:
			STRINGBUFFER *obj;
		public:
			stringbuffer();
			stringbuffer(STRINGBUFFER *obj);
			~stringbuffer();
			unsigned int getLength();
			void append(const std::string &string);
			void insert(const std::string &string, unsigned int pos);
			std::string getText();
			stringbuffer clone();
			void reset();
	};
}
#endif

#endif
