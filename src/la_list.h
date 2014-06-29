/*
 * =====================================================================================
 *
 *       Filename:  la_list.h
 *    Description:  handle (string) objects in a linked list
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_LIST_H
#define LA_LIST_H

#define LIST_VALUE_SIZE 2048

#include "la_boolean.h"

#ifdef __cplusplus
#include "la_exception.h"
#include <string>

extern "C" {
#endif

typedef struct la_list LIST;

LIST *list_new();
void list_add(LIST *self, const char *value);
void list_addUnique(LIST *self, const char *value);
BOOL list_exists(LIST *self, const char *value);
void list_remove(LIST *self, unsigned int index);
void list_swap(LIST *self, unsigned int index1, unsigned int index2);
void list_sort(LIST *self);
void list_free(LIST *self);
void list_reset(LIST *self);
char *list_get(LIST *self, unsigned int index);
unsigned int list_size(LIST *self);
void list_show(LIST *self);
int list_loadFromFile(LIST *self, const char *filename);
int list_saveToFile(LIST *self, const char *filename);

#ifdef __cplusplus
}

namespace la {
	namespace exception {
		class ListException : public Exception {
			public:
				ListException() : Exception() {};
				ListException(EXCEPTION *e) : Exception(e) {};
				ListException(int id, const std::string &message) : Exception(id, message) {};
				ListException(int id, const std::string &message, const std::string &cause, const std::string &action) : Exception(id, message, cause, action) {};
		};
	}

	namespace list {
		class List {
			private:
				LIST *obj;
			public:
				List();
				List(LIST *obj);
				~List();
				void add(const std::string &value);
				void addUnique(const std::string &value);
				bool exists(const std::string &value);
				void remove(unsigned int index);
				void swap(unsigned int index1, unsigned int index2);
				void sort();
				void reset();
				std::string get(unsigned int index);
				unsigned int size();
				void show();
				void loadFromFile(const std::string &filename);
				void saveToFile(const std::string &filename);
		};
	}
}
#endif

#endif
