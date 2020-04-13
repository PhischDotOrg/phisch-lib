/*-
 * $Copyright$
-*/

#ifndef __UART_ACCESS_LINUX_HPP_8f0c0c66_5e56_431b_8e1a_47bc71d178c8
#define __UART_ACCESS_LINUX_HPP_8f0c0c66_5e56_431b_8e1a_47bc71d178c8

#include <stdio.h>

namespace uart {

class UartAccessLinux {
private:
    FILE *m_stream;

public:
    UartAccessLinux(FILE *p_stream);
    ~UartAccessLinux();

    void putf(const char p_char) const;
    static void putf(void *p_this, const char p_char);
};

}; /* namespace uart */

#endif /* __UART_ACCESS_LINUX_HPP_8f0c0c66_5e56_431b_8e1a_47bc71d178c8 */
