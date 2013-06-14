/*
 * =====================================================================================
 *
 *       Filename:  la_common.h
 *
 *    Description:  common definitions
 *
 *        Version:  1.3.10
 *        Created:  03/09/2012 09:40:12 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_COMMON_H
#define LA_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* info */
#define LA_NAME "la-C"
#define LA_VERSION "1.4.1"
#define LA_AUTHOR "Stephan Laukien"

/* boolean type */
#ifndef BOOL
#define BOOL int
#define TRUE 1
#define FALSE 0 
#endif

#ifdef __cplusplus
}
#endif

#endif