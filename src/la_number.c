/*
 * =====================================================================================
 *
 *       Filename:  la_number.h
 *
 *    Description:  convert numbers
 *
 *        Version:  1.0
 *        Created:  03/09/2012 07:29:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "la_number.h"
#include <stdlib.h>
#include <stdio.h>

size_t number_intToSize(int num) {
	/* get size (log10) */
	size_t size = 0;
	int i;
	for (i = num; i != 0; i /= 10) {
		++size;
	}

	if (num < 0) ++size;                        /* negative value */

	return size;
}

size_t number_uintToSize(unsigned int num) {
	/* get size (log10) */
	size_t size = 0;
	int i;
	for (i = num; i != 0; i /= 10) {
		++size;
	}

	return size;
}

char *number_intToString(int num) {
	size_t size = number_intToSize(num);

	char *str;
	str = (char *) malloc(size + 1);

	sprintf(str, "%d", num);

	return str;
}

char *number_uintToString(unsigned int num) {
	/* get size (log10) */
	int size = 0;
	int i;
	for (i = num; i > 0; i /= 10) {
		++size;
	}

	char *str;
	str = (char *) malloc(size + 1);

	sprintf(str, "%d", num);

	return str;
}

#ifdef __cplusplus
namespace la {
	namespace number {
		size_t intToSize(int num) {
			return number_intToSize(num);
		}

		size_t uintToSize(unsigned int num) {
			return number_uintToSize(num);
		}

		std::string intToString(int num) {
			char *tmp = number_intToString(num);
			std::string res = std::string(tmp);
			free(tmp);

			return res;
		}

		std::string uintToString(unsigned int num) {
			char *tmp = number_uintToString(num);
			std::string res = std::string(tmp);
			free(tmp);

			return res;
		}
	}
}
#endif
