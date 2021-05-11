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

#include <stm32f4xx.h>

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

    switch (request) {
    case UsbRequest_e::e_SetAddress:
        this->statusIn<true>(Status_e::e_Ok);
        this->m_usbDevice.setAddress(p_setupPacket.m_wValue & 0x7F);
        break;
    case UsbRequest_e::e_GetDescriptor:
        this->m_usbDevice.getDescriptor(p_setupPacket.m_wValue, p_setupPacket.m_wLength);
        break;
    case UsbRequest_e::e_SetConfiguration:
        this->m_activeConfiguration = this->m_usbDevice.setConfiguration(p_setupPacket.m_wValue);
        statusIn(Status_e::e_Ok); /* TODO Check for errors here */
        break;
    case UsbRequest_e::e_GetStatus: {
        assert(p_setupPacket.m_wLength == 2);

        const ::usb::UsbDeviceStatus_t status = this->m_usbDevice.getStatus();
        this->dataIn(reinterpret_cast<const uint8_t *>(&status), p_setupPacket.m_wLength);
        } break;
    case UsbRequest_e::e_GetConfiguration: {
        assert(p_setupPacket.m_wValue == 0);
        assert(p_setupPacket.m_wIndex == 0);
        assert(p_setupPacket.m_wLength == 1);

        uint8_t cfg = this->m_usbDevice.getActiveConfiguration();
        this->dataIn(&cfg, sizeof(cfg));
        } break;
    case UsbRequest_e::e_SetFeature:
        /* TODO Actually implement something here */
        this->statusIn(Status_e::e_Ok);
        break;
    default:
        this->error();
        break;
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
UsbControlPipe::notifySetupPacketReceived(const UsbSetupPacket_t &p_setupPacket) {
    assert((static_cast<State_e>(m_state) == State_e::e_Idle)
      || (static_cast<State_e>(m_state) == State_e::e_StatusOut)
      || (static_cast<State_e>(m_state) == State_e::e_Error)
    );

    m_state = State_e::e_SetupReceived;
    this->decodeSetupPacket(p_setupPacket);
}

void
UsbControlPipe::notifyReadComplete(size_t p_numBytes) {
    (void) p_numBytes;

    switch (static_cast<State_e>(m_state)) {
    case State_e::e_DataOut:
        /* FIXME Check if expected amount of data was received? */
        statusIn(Status_e::e_Ok);
        break;
    case State_e::e_StatusOut:
        assert(p_numBytes == 0);
        idle();
        break;
    case State_e::e_Idle:
    default:
        assert(false);
    }
}

void
UsbControlPipe::notifyWriteComplete(size_t p_numBytes) {
    (void) p_numBytes;

    switch (static_cast<State_e>(m_state)) {
    case State_e::e_DataIn:
        /* FIXME Check if expected amount of data was received? */
        statusOut(Status_e::e_Ok);
        break;
    case State_e::e_StatusIn:
        assert(p_numBytes == 0);
        idle();
        break;
    default:
        assert(false);
    }
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

    USB_PRINTF("UsbControlPipe::%s(): m_bmRequestType=0x%x m_bRequest=0x%x m_wValue=0x%x m_wIndex=0x%x m_wLength=0x%x\r\n", __func__,
      p_setupPacket.m_bmRequestType, p_setupPacket.m_bRequest, p_setupPacket.m_wValue, p_setupPacket.m_wIndex, p_setupPacket.m_wLength);

    switch (usbRecipient) {
    case UsbRecipient_e::e_Device:
        this->decodeDeviceRequest(p_setupPacket);
        break;
    case UsbRecipient_e::e_Interface:
        this->decodeInterfaceRequest(p_setupPacket);
        break;
    case UsbRecipient_e::e_Endpoint:
        /* FIXME Actually implement this request */ 
        assert(p_setupPacket.m_bRequest == 0x1);
        this->statusIn(Status_e::e_Ok);
        break;
    case UsbRecipient_e::e_Other:
    default:
        this->error();
        break;
    }
}

void
UsbControlPipe::start(void) {
    m_usbDevice.start();

    idle();
}

void
UsbControlPipe::stop(void) {
    m_state = State_e::e_Undefined;
    // m_outEndpoint->disable();
    m_usbDevice.stop();
}

void
UsbControlPipe::idle(void) {
    m_state = State_e::e_Idle;

    m_inEndpoint.setPid(UsbInEndpoint::Pid_e::e_Data0);
    m_inEndpoint.nack();

    m_outEndpoint->setPid(UsbOutEndpoint::Pid_e::e_Data0);
    m_outEndpoint->enableSetupPacket();
}

void
UsbControlPipe::dataOut(uint8_t * p_data, size_t p_length) {
    assert(static_cast<State_e>(m_state) == State_e::e_SetupReceived);

    m_state = State_e::e_DataOut;

    assert(m_outEndpoint != nullptr);
    m_outEndpoint->setupRead(p_data, p_length);
}

void
UsbControlPipe::dataIn(const uint8_t * p_data, size_t p_length) {
    assert(static_cast<State_e>(m_state) == State_e::e_SetupReceived);

    m_state = State_e::e_DataIn;
    m_inEndpoint.setupWrite(p_data, p_length);
}

void
UsbControlPipe::statusOut(Status_e p_status) {
    m_state = State_e::e_StatusOut;

    switch (p_status) {
    case Status_e::e_Ok:
        /* FIXME Why is the setPid() on OUT EP0 required here? */

        // m_inEndpoint.setPid(UsbInEndpoint::Pid_e::e_Data0);
        // m_inEndpoint.stall();

        // m_outEndpoint->setPid(UsbOutEndpoint::Pid_e::e_Data0);

        assert(m_outEndpoint != nullptr);
        m_outEndpoint->setupRead(nullptr, 0);
        break;
    case Status_e::e_Error:
        error();
        break;
    default:
        assert(false);
    }
}

template<
    bool isSync /* = false */
>
void
UsbControlPipe::statusIn(Status_e p_status) {
    m_state = State_e::e_StatusIn;

    switch (p_status) {
    case Status_e::e_Ok:
        if constexpr(isSync == true) {
            m_inEndpoint.write(nullptr, 0);
        } else {
            m_inEndpoint.setupWrite(nullptr, 0);
        }
        break;
    case Status_e::e_Error:
        error();
        break;
    default:
        assert(false);
    }
}

void
UsbControlPipe::error(void) {
    m_state = State_e::e_Error;

    assert(m_outEndpoint != nullptr);
    m_outEndpoint->stall();
    
    m_inEndpoint.stall();
}
/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
