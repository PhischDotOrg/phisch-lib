/*-
 * $Copyright$
-*/

#include <usb/UsbControlPipe.hpp>
#include <usb/UsbDevice.hpp>
#include <usb/UsbInterface.hpp>
#include <usb/UsbTypes.hpp>

#include <usb/UsbInEndpoint.hpp>
#include <usb/UsbOutEndpoint.hpp>

#include <usb/UsbConfiguration.hpp>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include <assert.h>
#include <stddef.h>

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */

#include <algorithm>

namespace usb {

/***************************************************************************//**
 * @brief Decodes a Standard USB Interface Request.
 * 
 * Handles the Standard USB Interface Requests and forwards them to the active
 * USB Configuration.
 * 
 * @param p_setupPacket Reference to the USB Setup Packet.
 ******************************************************************************/
void
UsbControlPipe::decodeInterfaceRequest(const UsbSetupPacket_t &p_setupPacket) const {
    assert(this->m_activeConfiguration != nullptr);
    this->m_activeConfiguration->handleCtrlRequest(p_setupPacket);
}

/***************************************************************************//**
 * @brief Decodes a Standard USB Device Request.
 * 
 * Handles the Standard USB Device Requests and forwards them to the UsbDevice
 * object managed by this USB Control Pipe.
 * 
 * @param p_setupPacket Reference to the USB Setup Packet.
 ******************************************************************************/
void
UsbControlPipe::decodeDeviceRequest(const UsbSetupPacket_t &p_setupPacket) {
    UsbRequest_t request = static_cast<UsbRequest_t>(p_setupPacket.m_bRequest);

    USB_PRINTF("UsbControlPipe::%s(): request=%d, m_wLength=%i\r\n", __func__, request, p_setupPacket.m_wLength);

    switch (request) {
    case e_SetAddress:
        this->m_usbDevice.setAddress(p_setupPacket.m_wValue & 0x7F);
        /* Acknowledge the setAddress() command on the Default Ctrl Endpoint */
        this->write(NULL, 0);
        break;
    case e_GetDescriptor:
        this->m_usbDevice.getDescriptor(p_setupPacket.m_wValue, p_setupPacket.m_wLength);
        break;
    case e_SetConfiguration:
        this->m_activeConfiguration = this->m_usbDevice.setConfiguration(p_setupPacket.m_wValue);
        this->write(NULL, 0);
        break;
    case e_GetStatus: {
        assert(p_setupPacket.m_wLength == 2);

        const ::usb::UsbDeviceStatus_t status = this->m_usbDevice.getStatus();
        this->write(reinterpret_cast<const uint8_t *>(&status), p_setupPacket.m_wLength);
        } break;
    case e_GetConfiguration: {
        assert(p_setupPacket.m_wValue == 0);
        assert(p_setupPacket.m_wIndex == 0);
        assert(p_setupPacket.m_wLength == 1);

        uint8_t cfg = this->m_usbDevice.getActiveConfiguration();
        this->write(&cfg, sizeof(cfg));        
        } break;
    default:
        assert(false);
        break;
    }
}

/***************************************************************************//**
 * @brief Handle "Transfer Complete" (="Data Stage Complete") Situation.
 *
 * Called by UsbControlOutEndpoint::transferComplete when the Data Stage is
 * complete.
 * 
 * @param p_numBytes Number of Bytes in the completed transfer.
 * 
 * @note
 * If \p p_numBytes == 0, then the method got called by the IRQ triggered by the
 * Host's ACK packet. This represents the End of the Status Stage and there's
 * nothing left to do.
 * 
 * @note
 * If \p p_numBytes != 0, then the method got called by the OUT Endpoint's Transfer
 * Complete IRQ. This represents the end of the Data (OUT) Stage.
 ******************************************************************************/
void
UsbControlPipe::transferComplete(const size_t p_numBytes) const {
    USB_PRINTF("UsbControlPipe::%s(p_numBytes=%d)\r\n", __func__, p_numBytes);

    if (p_numBytes) {
        /* FIXME We should probably evaluate some return code here... */
        this->m_inEndpoint.write(nullptr, 0);
    }
}

/***************************************************************************//**
 * @brief Handles a _Setup Stage Complete_ Event.
 * 
 * This method is called by the Control OUT Endpoint when the Setup Stage of a
 * Control Request is complete.
 * 
 * The USB Control Pipe can then decode the packet.
 * 
 * \see decodeSetupPacket
 * \see UsbCtrlOutEndpoint::setupComplete 
 ******************************************************************************/
void
UsbControlPipe::setupStageComplete(const UsbSetupPacket_t &p_setupPacket) {
    USB_PRINTF("UsbControlPipe::%s(): m_bmRequestType=0x%x m_bRequest=0x%x\r\n", __func__,
      p_setupPacket.m_bmRequestType, p_setupPacket.m_bRequest);

    this->decodeSetupPacket(p_setupPacket);
}

/***************************************************************************//**
 * @brief Set the Data Stage Buffer.
 * 
 * Used by Control Request handling classes, e.g. UsbInterface, to set up the
 * buffer that receives the Data OUT Stage of a Control Request.
 * 
 * @param p_buffer Address of Buffer that receives the Data OUT Stage.
 * @param p_length Size of Buffer that receives the Data OUT Stage.
 ******************************************************************************/
void
UsbControlPipe::setDataStageBuffer(uint32_t * const p_buffer, const size_t p_length) const {
    assert(this->m_outEndpoint != nullptr);
    this->m_outEndpoint->setDataStageBuffer(p_buffer, p_length);
}

/***************************************************************************//**
 * @brief Decode a USB _Setup_ Packet.
 * 
 * Decodes the USB Setup Packet and forwards the request to the right recipient.
 * 
 * \see UsbRecipient_t
 * 
 * @param p_setupPacket Reference to the USB Setup Packet.
 ******************************************************************************/
void
UsbControlPipe::decodeSetupPacket(const UsbSetupPacket_t &p_setupPacket) {
    const UsbRecipient_t usbRecipient = static_cast<UsbRecipient_t>(p_setupPacket.m_bmRequestType & 0x0F);

    USB_PRINTF("UsbControlPipe::%s(): m_bmRequestType=0x%x p_setupPacket=0x%x\r\n", __func__,
      p_setupPacket.m_bmRequestType, p_setupPacket.m_bRequest);

    switch (usbRecipient) {
    case e_Device:
        this->decodeDeviceRequest(p_setupPacket);
        break;
    case e_Interface:
        this->decodeInterfaceRequest(p_setupPacket);
        break;
    case e_Endpoint:
    case e_Other:
    default:
        /* Not yet implemented */
        assert(false);
        break;
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
