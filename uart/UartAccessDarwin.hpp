/*-
 * $Copyright$
-*/

#ifndef __UART_ACCESS_DARWIN_HPP_2F5B7286_CEA4_4860_9AA1_3401523F2223
#define __UART_ACCESS_DARWIN_HPP_2F5B7286_CEA4_4860_9AA1_3401523F2223

#include <stdio.h>

namespace uart {

class UartAccessDarwin {
private:
    FILE *m_stream;

public:
    UartAccessDarwin(FILE *p_stream);
    ~UartAccessDarwin();

    void putf(const char p_char) const;
    static void putf(void *p_this, const char p_char);
};

}; /* namespace uart */

#endif /* __UART_ACCESS_DARWIN_HPP_2F5B7286_CEA4_4860_9AA1_3401523F2223 */
