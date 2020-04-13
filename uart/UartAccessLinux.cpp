/*-
 * $Copyright$
-*/

#include "uart/UartAccessLinux.hpp"

#include <assert.h>

namespace uart {

/*******************************************************************************
 *
 ******************************************************************************/
UartAccessLinux::UartAccessLinux(FILE *p_stream) : m_stream(p_stream) {
}

/*******************************************************************************
 *
 ******************************************************************************/
UartAccessLinux::~UartAccessLinux() {
    m_stream = NULL;
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UartAccessLinux::putf(void *p_this, const char p_char) {
    UartAccessLinux *obj = reinterpret_cast<uart::UartAccessLinux *>(p_this);
    assert(obj != NULL);
    obj->putf(p_char);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UartAccessLinux::putf(const char p_char) const {
    fprintf(this->m_stream, "%c", p_char);
}

} /* namespace uart */
