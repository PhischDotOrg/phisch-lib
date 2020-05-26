/*-
 * $Copyright$
-*/

#include <usb/UsbBulkOutEndpoint.hpp>
#include <usb/UsbTypes.hpp>
#include <uart/UartDevice.hpp>
#include <assert.h>
#include <stddef.h>

#include <algorithm>

#include <usb/OutEndpointViaSTM32F4.hpp>

#include <usb/UsbInEndpoint.hpp>
extern usb::UsbBulkInEndpoint bulkInEndpoint;

extern uart::UartDevice g_uart;

/* FIXME Template Code, should be in .hpp */

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbHwBulkOutEndpointT>
UsbBulkOutEndpointT<UsbHwBulkOutEndpointT>::UsbBulkOutEndpointT(UsbHwBulkOutEndpointT &p_hwEndpoint)
  : m_hwEndpoint(p_hwEndpoint)
{
    this->m_hwEndpoint.registerEndpointCallback(*this);
    this->m_hwEndpoint.setDataBuffer(&this->m_inData, sizeof(this->m_inData));
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbHwBulkOutEndpointT>
UsbBulkOutEndpointT<UsbHwBulkOutEndpointT>::~UsbBulkOutEndpointT() {
    this->m_hwEndpoint.unregisterEndpointCallback();
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbHwBulkOutEndpointT>
void
UsbBulkOutEndpointT<UsbHwBulkOutEndpointT>::transferComplete(const size_t p_numBytes) const {
    static uint8_t data[sizeof(this->m_inData) + 1];

    USB_PRINTF("UsbBulkOutEndpoint::%s(p_numBytes=%d): ", __func__, p_numBytes);

    for (unsigned i = 0; i < std::min(p_numBytes, sizeof(data) - 1); i++) {
        data[i] = (this->m_inData >> (8 * i)) & 0xFF;
    }

    data[std::min(p_numBytes, sizeof(data) - 1)] = '\0';
    g_uart.printf("%s", data);

#if defined(USB_DEBUG)
    g_uart.printf("\r\n");
#endif

    bulkInEndpoint.write(data, std::min(p_numBytes, sizeof(data) - 1));
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbHwBulkOutEndpointT>
void
UsbBulkOutEndpointT<UsbHwBulkOutEndpointT>::disable(void) const {
    this->m_hwEndpoint.disable();
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbHwBulkOutEndpointT>
void
UsbBulkOutEndpointT<UsbHwBulkOutEndpointT>::enable(void) const {
    this->m_hwEndpoint.setup();
    this->m_hwEndpoint.enable();
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

/*******************************************************************************
 *
 ******************************************************************************/
template class ::usb::UsbBulkOutEndpointT<::usb::stm32f4::BulkOutEndpointViaSTM32F4>;
