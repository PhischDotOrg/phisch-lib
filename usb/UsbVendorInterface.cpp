/*-
 * $Copyright$
-*/

#include <usb/UsbVendorInterface.hpp>
#include <usb/UsbInEndpoint.hpp>
#include <usb/UsbOutEndpoint.hpp>

#include <usb/UsbTypes.hpp>

#include <cassert>

#include <FreeRTOS.h>
#include <FreeRTOS/include/task.h>

/*****************************************************************************/
namespace usb {
/*****************************************************************************/

void
UsbVendorInterface::enable(void) const {
    USB_PRINTF("UsbVendorInterface::%s()\r\n", __func__);

    // TODO Implement UsbVendorInterface::enable()
}

void
UsbVendorInterface::disable(void) const {
    USB_PRINTF("UsbVendorInterface::%s()\r\n", __func__);

    // TODO Implement UsbVendorInterface::disable()

    m_inEndpoint.nack();
    m_outEndpoint.nack();
}

void
UsbVendorInterface::sendResponse(ReturnCode p_returnCode, uint8_t /* p_delayInMs */,
  const void * const p_data /* = nullptr */, const size_t p_length /* = 0 */) const {

    switch (p_returnCode) {
    case ReturnCode::e_Ack:
        m_defaultCtrlPipe->dataIn(static_cast<const uint8_t *>(p_data), p_length);
        break;
    case ReturnCode::e_Stall:
        m_defaultCtrlPipe->error();
        break;
    case ReturnCode::e_Timeout:
        /* FIXME Must be de-coupled from ISR routine */
        // vTaskDelay(p_delayInMs / portTICK_PERIOD_MS);
        m_defaultCtrlPipe->dataIn(static_cast<const uint8_t *>(p_data), p_length);
        break;
    default:
        assert(false);
    }
}

void
UsbVendorInterface::handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) {
    Request request         = static_cast<Request>(p_setupPacket.m_bRequest);
    ReturnCode returnCode   = static_cast<ReturnCode>((p_setupPacket.m_wValue >> 0) & 0xFF);
    uint8_t delay           = (p_setupPacket.m_wValue >> 8) & 0xFF;

    USB_PRINTF("UsbVendorInterface::%s(request=%d, length=%d, returnCode=%d, delay=%d)\r\n", __func__, request, p_setupPacket.m_wLength, returnCode, delay);

    switch (request) {
    case Request::e_NoData:
        sendResponse(returnCode, delay, nullptr);
        break;
    case Request::e_DataOut:
        m_defaultCtrlPipe->dataOut(m_dataOutBuffer.data(), std::min<size_t>(m_dataOutBuffer.size(), p_setupPacket.m_wLength));
        break;
    case Request::e_DataIn:
        m_defaultCtrlPipe->dataIn(m_dataInBuffer.data(), std::min<size_t>(m_dataInBuffer.size(), p_setupPacket.m_wLength));
        break;
    case Request::e_Invalid:
    default:
        assert(false);
    }
}

/*****************************************************************************/
} /* namespace usb */
/*****************************************************************************/
