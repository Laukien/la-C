/*
 * =====================================================================================
 *
 *       Filename:  la_parameter.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/2011 10:25:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_PARAMETER_H
#define LA_PARAMETER_H

#define LA_PARAMETER_KEY_SIZE 32
#define LA_PARAMETER_VALUE_SIZE 64

#ifdef __cplusplus
extern "C" {
#endif

struct la_parameter {
	char *key;
	char *value;
	struct la_parameter *next;
};
typedef struct la_parameter LA_PARAMETER;

LA_PARAMETER *parameter_new();
void parameter_add (LA_PARAMETER *param, const char *key, const char *value);
void parameter_remove (LA_PARAMETER *param, const char *key);
void parameter_clear (LA_PARAMETER *param);
void parameter_free (LA_PARAMETER *param);
void parameter_reset (LA_PARAMETER *param);
char *parameter_get (LA_PARAMETER *param, const char *key);
unsigned int parameter_size (LA_PARAMETER *param);
int parameter_loadFromFile(LA_PARAMETER *param, const char *filename);
int parameter_saveToFile(LA_PARAMETER *param, const char *filename);

#ifdef __cplusplus
}
#endif

#endif

