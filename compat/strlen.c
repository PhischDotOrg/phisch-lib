/*-
 * $Copyright$
-*/

#include <sys/types.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

size_t
strlen(const char * const s) {
    size_t len = 0;
    
    if (s != NULL) {
        while (s[len] != '\0') {
            len++;
        }
    }
    
    return len;
}
    
#if defined(__cplusplus)
}
#endif /* defined (__cplusplus) */
