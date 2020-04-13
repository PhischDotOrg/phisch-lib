/*-
 * $Copyright$
-*/

#ifndef _COMPAT_STRING_H_e5bdb3ec_ea93_4a01_b226_16d6822afe94
#define _COMPAT_STRING_H_e5bdb3ec_ea93_4a01_b226_16d6822afe94

#include <sys/types.h>

void *memcpy(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
size_t strlen(const char * const s);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */

#endif /* _COMPAT_STRING_H_e5bdb3ec_ea93_4a01_b226_16d6822afe94 */
