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

#define LIST_VALUE_SIZE 128

#ifdef __cplusplus
extern "C" {
#endif

struct la_list {
	char *value;
	struct la_list *next;
};
typedef struct la_list LIST;

LIST *list_new();
void list_add(LIST *self, const char *value);
void list_remove(LIST *self, unsigned int index);
void list_clear(LIST *self);
void list_swap(LIST *self, unsigned int index1, unsigned int index2);
void list_free(LIST *self);
void list_reset(LIST *self);
char *list_get(LIST *self, unsigned int index);
unsigned int list_size(LIST *self);
void list_show(LIST *self);
int list_loadFromFile(LIST *self, const char *filename);
int list_saveToFile(LIST *self, const char *filename);

#ifdef __cplusplus
}
#endif

#endif

