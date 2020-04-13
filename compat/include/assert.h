/*-
 * $Copyright$
-*/

#ifndef _ASSERT_H_a6f90a4d_dec1_41c6_a328_56aef3d24572
#define _ASSERT_H_a6f90a4d_dec1_41c6_a328_56aef3d24572

#if defined(NDEBUG)
#define assert(_p)
#else /* defined(NDEBUG) */

#if defined(__cplusplus)
extern "C" {
#endif  /* defined(__cplusplus) */
void halt(const char * const p_file, unsigned p_line);
#if defined(__cplusplus)
}; /* extern "C" */
#endif /* defined(__cplusplus) */

#define assert(_p)  if (!(_p)) halt(__FILE__, __LINE__)

#endif /* defined(NDEBUG) */

#endif /* _ASSERT_H_a6f90a4d_dec1_41c6_a328_56aef3d24572 */
