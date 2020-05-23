/*-
 * $Copyright$
-*/

#ifndef _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510
#define _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510

#include <usb/InEndpointViaSTM32F4.hpp>
#include <usb/UsbTypes.hpp>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
    namespace stm32f4 {
        class BulkInEndpointViaSTM32F4;
        class CtrlInEndpointViaSTM32F4;
    }

/*******************************************************************************
 *
 ******************************************************************************/
template<class UsbHwBulkInEndpointT = ::usb::stm32f4::BulkInEndpointViaSTM32F4>
class UsbBulkInEndpointT {
private:
    UsbHwBulkInEndpointT &  m_hwEndpoint;

public:
    UsbBulkInEndpointT(UsbHwBulkInEndpointT &p_hwEndpoint) : m_hwEndpoint(p_hwEndpoint) {

    }

    ~UsbBulkInEndpointT() {

    }

    constexpr void enable(void) const {
        this->m_hwEndpoint.enable();
    }

    constexpr void disable(void) const {
        this->m_hwEndpoint.disable();
    }

    constexpr void write(const uint8_t * const p_data, const size_t p_length) const {
        this->m_hwEndpoint.write(p_data, p_length);
    }
};

typedef UsbBulkInEndpointT<> UsbBulkInEndpoint;

/*******************************************************************************
 *
 ******************************************************************************/
template<class UsbHwCtrlInEndpointT = ::usb::stm32f4::CtrlInEndpointViaSTM32F4>
class UsbCtrlInEndpointT {
private:
    UsbHwCtrlInEndpointT &   m_hwEndpoint;

public:
    constexpr UsbCtrlInEndpointT(UsbHwCtrlInEndpointT &p_hwEndpoint) : m_hwEndpoint(p_hwEndpoint) {

    }

    ~UsbCtrlInEndpointT() {

    }
    
    constexpr void write(const uint8_t * const p_data, const size_t p_length) const {
        this->m_hwEndpoint.write(p_data, p_length);
    }

    constexpr void writeString(const ::usb::UsbStringDescriptor &p_string, const size_t p_len) const {
        this->m_hwEndpoint.writeString(p_string, p_len);
    }
};
typedef UsbCtrlInEndpointT<> UsbCtrlInEndpoint;

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510 */