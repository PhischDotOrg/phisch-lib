/*-
 * $Copyright$
-*/
#ifndef _STRINGS_H_4ca34ffe_81a8_4eaa_b827_63a4d71dd917
#define _STRINGS_H_4ca34ffe_81a8_4eaa_b827_63a4d71dd917

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

void bzero(void *s, size_t n);
void bcopy(const void *src, void *dest, size_t n);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */

#endif /* _STRINGS_H_4ca34ffe_81a8_4eaa_b827_63a4d71dd917 */
