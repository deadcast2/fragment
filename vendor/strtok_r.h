/*
 * public domain strtok_r() by Charlie Gordon
 *
 *   from comp.lang.c  9/14/2007
 *
 *      http://groups.google.com/group/comp.lang.c/msg/2ab1ecbb86646684
 *
 *     (Declaration that it's public domain):
 *      http://groups.google.com/group/comp.lang.c/msg/7c7b39328fefab9c
 */

#ifndef _STRTOK_R_H_
#define _STRTOK_R_H_

#include <string.h>

char *strtok_r(char *str, const char *delim, char **nextp);

#endif
