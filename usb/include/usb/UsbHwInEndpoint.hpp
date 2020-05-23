/*-
 * $Copyright$
-*/

#ifndef _USB_HW_CTRL_IN_ENDPOINT_HPP_7E4C459F_E88C_4C2B_8197_16A54BCE8AF0
#define _USB_HW_CTRL_IN_ENDPOINT_HPP_7E4C459F_E88C_4C2B_8197_16A54BCE8AF0

#include <usb/UsbTypes.hpp>
#include <stdint.h>

#include <usb/InEndpointViaSTM32F4.hpp>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
class UsbHwBulkInEndpoint {
public:
    UsbHwBulkInEndpoint(void) {
    }

    virtual ~UsbHwBulkInEndpoint() {
    }

    virtual void enable(void) const = 0;
    virtual void disable(void) const = 0;

    virtual void write(const uint8_t * const p_data, const size_t p_length) const = 0;
};

#if 0
/*******************************************************************************
 *
 ******************************************************************************/
template<class UsbHwInEndpointT>
class UsbHwCtrlInEndpointT {
private:
    const UsbHwInEndpointT &m_hwEndpoint;

public:
    constexpr UsbHwCtrlInEndpointT(const UsbHwInEndpointT &p_hwEndpoint) : m_hwEndpoint(p_hwEndpoint) {
    }

    ~UsbHwCtrlInEndpointT() {
    }

    constexpr void write(const uint8_t * const p_data, const size_t p_length) const {
        this->m_hwEndpoint.write(p_data, p_length);
    }

    constexpr void writeString(const ::usb::UsbStringDescriptor &p_string, const size_t p_len) const {
        this->m_hwEndpoint.writeString(p_string, p_len);
    }
};

namespace stm32f4 {
    class CtrlInEndpointViaSTM32F4;
}

typedef UsbHwCtrlInEndpointT<stm32f4::CtrlInEndpointViaSTM32F4> UsbHwCtrlInEndpoint;
#endif

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_HW_CTRL_IN_ENDPOINT_HPP_7E4C459F_E88C_4C2B_8197_16A54BCE8AF0 */
