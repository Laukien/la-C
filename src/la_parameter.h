/*
 * =====================================================================================
 *
 *       Filename:  la_parameter.h
 *    Description:  handle (string) objects in a linked map
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_PARAMETER_H
#define LA_PARAMETER_H

#define PARAMETER_KEY_SIZE 128
#define PARAMETER_VALUE_SIZE 2048

#include "la_boolean.h"
#include "la_exception.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct la_parameter PARAMETER;

PARAMETER *parameter_new();
void parameter_add(PARAMETER *self, const char *key, const char *value);
void parameter_addUnique(PARAMETER *self, const char *key, const char *value);
void parameter_addReplace(PARAMETER *self, const char *key, const char *value);
BOOL parameter_exists(PARAMETER *self, const char *key);
void parameter_remove(PARAMETER *self, const char *key);
void parameter_free(PARAMETER *self);
void parameter_reset(PARAMETER *self);
PARAMETER *parameter_getByIndex(PARAMETER *self, unsigned int index);
char *parameter_getKeyByIndex(PARAMETER *self, unsigned int index);
char *parameter_getValueByIndex(PARAMETER *self, unsigned int index);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  parameter_getIndexByKey
 *  Description:  return index of the element; otherwise '0' if the exlement doesn't exists
 * =====================================================================================
 */
unsigned int parameter_getIndexByKey (PARAMETER *self, const char *key);
char *parameter_getValueByKey(PARAMETER *param, const char *key);
char *parameter_get(PARAMETER *self, const char *key);
unsigned int parameter_size(PARAMETER *self);
void parameter_show(PARAMETER *self);
int parameter_loadFromFile(PARAMETER *param, const char *filename);
int parameter_saveToFile(PARAMETER *param, const char *filename);

#ifdef __cplusplus
}
#include <string>

namespace la {
	namespace exception {
		class ParameterException : public Exception {
			public:
				ParameterException() : Exception() {};
				ParameterException(EXCEPTION *e) : Exception(e) {};
				ParameterException(int id, const std::string &message) : Exception(id, message) {};
				ParameterException(int id, const std::string &message, const std::string &cause, const std::string &action) : Exception(id, message, cause, action) {};
		};
	}
	
	namespace parameter {
		class Parameter {
			private:
				PARAMETER *obj;
			public:
				Parameter();
				Parameter(PARAMETER *obj);
				~Parameter();
				void add(const std::string &key, const std::string &value);
				void addUnique(const std::string &key, const std::string &value);
				void addReplace(const std::string &key, const std::string &value);
				void exists(const std::string &key);
				void remove(const std::string &key);
				void reset();
				Parameter getByIndex(unsigned int index);
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
}
#endif

#endif

