/*-
 * $Copyright$
-*/

#ifndef _PHISCH_LOG_HPP_5BE05124_F09F_4C37_8F0F_98FB828FBEC0
#define _PHISCH_LOG_HPP_5BE05124_F09F_4C37_8F0F_98FB828FBEC0

#if !defined(NO_LOGGING)
    #if defined(__cplusplus)
        extern "C" {
    #endif /* defined(__cplusplus) */

        void debug_printf(const char * const p_fmt, ...);

    #if defined(__cplusplus)
        } /* extern "C" */
    #endif /* defined(__cplusplus) */

    #define PHISCH_LOG(...)  debug_printf(__VA_ARGS__)
#else /* defined(NO_LOGGING) */
    #define PHISCH_LOG(...)
#endif /* defined(NO_LOGGING) */

#endif /* _PHISCH_LOG_HPP_5BE05124_F09F_4C37_8F0F_98FB828FBEC0 */
