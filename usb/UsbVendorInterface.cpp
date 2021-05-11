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
    /* TODO Implement Function */
    // this->m_outEndpoint.enable();
    // this->m_inEndpoint.enable();
}

void
UsbVendorInterface::disable(void) const {
    /* TODO Implement Function */
    // this->m_inEndpoint.disable();
    // this->m_outEndpoint.disable();
}

void
UsbVendorInterface::sendResponse(ReturnCode p_returnCode, uint8_t /* p_delayInMs */,
  const void * const p_data /* = nullptr */, const size_t p_length /* = 0 */) const {
    /* FIXME Must be de-coupled from ISR routine */
    // vTaskDelay(p_delayInMs / portTICK_PERIOD_MS);

    switch (p_returnCode) {
    case ReturnCode::e_Ack:
        m_defaultCtrlPipe->dataIn(static_cast<const uint8_t *>(p_data), p_length);
        break;
    case ReturnCode::e_Stall:
        m_defaultCtrlPipe->error();
        break;
    default:
        assert(false);
    }
}

void
UsbVendorInterface::handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) {
    Request request         = static_cast<Request>(p_setupPacket.m_bmRequestType & 0b1'1111);
    ReturnCode returnCode   = static_cast<ReturnCode>((p_setupPacket.m_wValue >> 0) & 0xFF);
    uint8_t delay           = (p_setupPacket.m_wValue >> 8) & 0xFF;

    USB_PRINTF("UsbVendorInterface::%s(request=%d, returnCode=%d, delay=%d)\r\n", __func__, request, returnCode, delay);

    switch (request) {
    case Request::e_NoData:
        sendResponse(returnCode, delay, nullptr);
        break;
    case Request::e_DataOut:
    case Request::e_DataIn:
    case Request::e_Invalid:
    default:
        assert(false);
    }
}

/*****************************************************************************/
} /* namespace usb */
/*****************************************************************************/
