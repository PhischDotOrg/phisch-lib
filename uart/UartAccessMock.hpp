/*-
 * $Copyright$
 */

#ifndef _UART_ACCESS_MOCK_HPP_2a097d30_8f3a_458b_b66f_5dc0a717225f
#define _UART_ACCESS_MOCK_HPP_2a097d30_8f3a_458b_b66f_5dc0a717225f

/*******************************************************************************
 * 
 ******************************************************************************/
#if defined(UNITTEST)
#include <gmock/gmock.h>

namespace uart {

class UartAccessMock {
public:
    MOCK_CONST_METHOD1(putf, int(const char p_char));
    static void putf(void *p_this, char p_char) {
        (reinterpret_cast<UartAccessMock *>(p_this))->putf(p_char);
    }
};

} /* namespace uart */
#else /* defined(UNITTEST) */
namespace uart {
class UartAccessMock;
} /* namespace uart */
#endif /* defined(UNITTEST) */

#endif /* _UART_ACCESS_MOCK_HPP_2a097d30_8f3a_458b_b66f_5dc0a717225f */
