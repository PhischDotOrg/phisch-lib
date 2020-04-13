/*-
 * $Copyright$
-*/

#include "uart/UartAccessDarwin.hpp"

#include <assert.h>

namespace uart {

/*******************************************************************************
 *
 ******************************************************************************/
UartAccessDarwin::UartAccessDarwin(FILE *p_stream) : m_stream(p_stream) {
}

/*******************************************************************************
 *
 ******************************************************************************/
UartAccessDarwin::~UartAccessDarwin() {
    m_stream = NULL;
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UartAccessDarwin::putf(void *p_this, const char p_char) {
    UartAccessDarwin *obj = reinterpret_cast<uart::UartAccessDarwin *>(p_this);
    assert(obj != NULL);
    obj->putf(p_char);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UartAccessDarwin::putf(const char p_char) const {
    fprintf(this->m_stream, "%c", p_char);
}

} /* namespace uart */
