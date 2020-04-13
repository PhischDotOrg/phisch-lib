/*-
 * $Copyright$
-*/

#ifndef _COMPAT_STDARG_H_2214274a_774b_4472_b32a_fd55ddc96d73
#define _COMPAT_STDARG_H_2214274a_774b_4472_b32a_fd55ddc96d73

typedef __builtin_va_list va_list;

#define va_start(v,l) __builtin_va_start(v,l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v,l) __builtin_va_arg(v,l)


#endif /* _COMPAT_STDARG_H_2214274a_774b_4472_b32a_fd55ddc96d73 */
