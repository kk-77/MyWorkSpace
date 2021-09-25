/*例程使用到的一些函数，以及解析命令行的函数*/
#ifndef MYLIB_H
#define MYLIB_H /* Prevent accidental double inclusion */

#include <sys/types.h> /* Type definitions used by many programs */
#include <stdio.h>     /* Standard I/O functions */
#include <stdlib.h>    /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>    /* Prototypes for many system calls */
#include <errno.h>     /* Declares errno and defines error constants */
#include <string.h>    /* Commonly used string-handling functions */
#include <limits.h>
#include <stdint.h>

#define GN_NONNEG 01 /* Value must be >= 0 */
#define GN_GT_0 02   /* Value must be > 0 */

/* By default, integers are decimal */
#define GN_ANY_BASE 0100 /* Can use any base - like strtol(3) */
#define GN_BASE_8 0200   /* Value is expressed in octal */
#define GN_BASE_16 0400  /* Value is expressed in hexadecimal */

long getLong(const char *arg, int flags, const char *name);

int getInt(const char *arg, int flags, const char *name);

#endif
