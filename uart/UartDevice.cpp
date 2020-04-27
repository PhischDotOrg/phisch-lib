/*-
 * $Copyright$
-*/

#include <uart/UartDevice.hpp>
#include <tinyprintf.h>

namespace uart {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
DeviceT<AccessT>::DeviceT(AccessT * const p_access) : m_access(p_access) {
    init_printf(this->m_access, AccessT::putf);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
DeviceT<AccessT>::~DeviceT(void) {

}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
void
DeviceT<AccessT>::printf(const char * const p_fmt, ...) const {
    va_list va;
    va_start(va, p_fmt);

    ::tfp_format(this->m_access, AccessT::putf, p_fmt, va);

    va_end(va);
}

/*******************************************************************************
 *
 ******************************************************************************/
// template class DeviceT<uart::UartAccess>;

} /* namespace uart */
