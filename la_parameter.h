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

#define PARAMETER_KEY_SIZE 32
#define PARAMETER_VALUE_SIZE 64

#ifdef __cplusplus
extern "C" {
#endif

struct la_parameter {
	char *key;
	char *value;
	struct la_parameter *next;
};
typedef struct la_parameter PARAMETER;

PARAMETER *parameter_new();
void parameter_add (PARAMETER *param, const char *key, const char *value);
void parameter_remove (PARAMETER *param, const char *key);
void parameter_clear (PARAMETER *param);
void parameter_free (PARAMETER *param);
void parameter_reset (PARAMETER *param);
char *parameter_get (PARAMETER *param, const char *key);
unsigned int parameter_size (PARAMETER *param);
int parameter_loadFromFile(PARAMETER *param, const char *filename);
int parameter_saveToFile(PARAMETER *param, const char *filename);

#ifdef __cplusplus
}
#endif

#endif

