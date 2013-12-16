/*
 * =====================================================================================
 *
 *       Filename:  la_list.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/08/2012 10:25:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_LIST_H
#define LA_LIST_H

#define LIST_VALUE_SIZE 2048

#include "la_boolean.h"

#ifdef __cplusplus
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
#include <string>

namespace la {
	class list {
		private:
			LIST *obj;
		public:
			list();
			list(LIST *obj);
			~list();
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
#endif

#endif
