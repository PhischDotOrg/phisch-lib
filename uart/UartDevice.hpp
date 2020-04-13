/*-
 * $Copyright$
-*/

#ifndef __UART_DEVICE_HPP_77b2f871_582a_4028_8515_7686d0161fbb
#define __UART_DEVICE_HPP_77b2f871_582a_4028_8515_7686d0161fbb

#include "uart/UartAccess.hpp"

namespace uart {

/*******************************************************************************
 * 
 ******************************************************************************/
template<typename AccessT>
class DeviceT {
private:
    AccessT * const m_access;

public:
    DeviceT(AccessT * const p_access);
    ~DeviceT(void);

    void printf(const char * const p_fmt, ...) const;
};

typedef DeviceT<uart::UartAccess> UartDevice;

} /* namespace uart */

#include <uart/UartDevice.cpp>

#endif /* __UART_DEVICE_HPP_77b2f871_582a_4028_8515_7686d0161fbb */
