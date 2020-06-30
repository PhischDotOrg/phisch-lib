/*-
 * $Copyright$
-*/

#ifndef _PHISCH_LOG_HPP_5BE05124_F09F_4C37_8F0F_98FB828FBEC0
#define _PHISCH_LOG_HPP_5BE05124_F09F_4C37_8F0F_98FB828FBEC0

#if !defined(NO_LOGGING)
    #include <uart/UartDevice.hpp>

    extern uart::UartDevice g_uart;

    #define PHISCH_LOG(...)  g_uart.printf(__VA_ARGS__)
#else /* defined(NO_LOGGING) */
    #define PHISCH_LOG(...)
#endif /* defined(NO_LOGGING) */

#endif /* _PHISCH_LOG_HPP_5BE05124_F09F_4C37_8F0F_98FB828FBEC0 */
