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

#define PARAMETER_KEY_SIZE 128
#define PARAMETER_VALUE_SIZE 2048

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
PARAMETER *parameter_getByIndex (PARAMETER *param, unsigned int index);
char *parameter_getKeyByIndex (PARAMETER *param, unsigned int index);
char *parameter_getValueByIndex (PARAMETER *param, unsigned int index);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  parameter_getIndexByKey
 *  Description:  return index of the element; otherwise '0' if the exlement doesn't exists
 * =====================================================================================
 */
unsigned int parameter_getIndexByKey (PARAMETER *self, const char *key);
char *parameter_getValueByKey(PARAMETER *param, const char *key);
char *parameter_get (PARAMETER *param, const char *key);
unsigned int parameter_size (PARAMETER *param);
void parameter_show(PARAMETER *self);
int parameter_loadFromFile(PARAMETER *param, const char *filename);
int parameter_saveToFile(PARAMETER *param, const char *filename);

#ifdef __cplusplus
}
#include <string>

namespace la {
	class parameter {
		private:
			PARAMETER *obj;
		public:
			parameter();
			parameter(PARAMETER *obj);
			~parameter();
			void add(const std::string &key, const std::string &value);
			void remove(const std::string &key);
			void clear();
			void reset();
			parameter getByIndex(unsigned int index);
			std::string getKeyByIndex(unsigned int index);
			std::string getValueByIndex(unsigned int index);
			unsigned int getIndexByKey(const std::string &key);
			std::string getValueByKey(const std::string &key);
			std::string get(const std::string &key);
			unsigned int size();
			void show();
			void loadFromFile(const std::string &filename);
			void saveToFile(const std::string &filename);
	};
}
#endif

#endif

