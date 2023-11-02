/*-
 * $Copyright$
-*/

#include <usb/UsbInterface.hpp>
#include <usb/UsbControlPipe.hpp>
#include <usb/UsbInEndpoint.hpp>
#include <usb/UsbOutEndpoint.hpp>

#include <assert.h>
#include <string.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbVcpInterface::enable(void) const {
    /* TODO Implement Function */
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbVcpInterface::disable(void) const {
    /* TODO Implement Function */
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbVcpInterface::handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) {
    UsbCdcRequest_e request = static_cast<UsbCdcRequest_e>(p_setupPacket.m_bRequest);

    switch (request) {
    case e_GetLineCoding:
        assert(p_setupPacket.m_wValue == 0);
        assert(p_setupPacket.m_wIndex == 0); /* FIXME Should be same as UsbInterface::m_interfaceNo */
        assert(p_setupPacket.m_wLength == 7);
        assert(p_setupPacket.m_bmRequestType == 0xA1);

        this->m_defaultCtrlPipe->dataIn(reinterpret_cast<const uint8_t *>(&m_usbCdcLineCoding), sizeof(struct UsbCdcLineCoding_s));
        break;
    case e_SetControlLineState:
        assert(p_setupPacket.m_bmRequestType == 0x21);
        assert(p_setupPacket.m_wIndex == 0); /* FIXME Should be same as UsbInterface::m_interfaceNo */
        assert(p_setupPacket.m_wLength == 0);

        this->m_defaultCtrlPipe->statusIn(UsbControlPipe::Status_e::e_Ok);
        break;
    case e_SetLineCoding:
        assert(p_setupPacket.m_wValue == 0);
        assert(p_setupPacket.m_wIndex == 0); /* FIXME Should be same as UsbInterface::m_interfaceNo */
        assert(p_setupPacket.m_wLength == 7);
        assert(p_setupPacket.m_bmRequestType == 0x21);

        this->m_defaultCtrlPipe->dataOut(reinterpret_cast<uint8_t *>(&m_usbCdcLineCoding), sizeof(m_usbCdcLineCoding));
        break;
    default:
        assert(false);
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbHidInterface::enable(void) const {

}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbHidInterface::disable(void) const {

}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbHidInterface::handleCtrlRequest(const UsbSetupPacket_t & p_setupPacket) {
    UsbHidRequest_e request = static_cast<UsbHidRequest_e>(p_setupPacket.m_bRequest);

    switch (request) {
    case e_SetIdle:
        /* FIXME Actually do something useful here */
        this->m_defaultCtrlPipe->statusIn(UsbControlPipe::Status_e::e_Ok);
        break;
    case e_GetDescriptor:
        this->getDescriptor(p_setupPacket.m_wValue, p_setupPacket.m_wLength);
        break;
    case e_SetProtocol:
        /* FIXME Actually do something useful here */
        this->m_defaultCtrlPipe->statusIn(UsbControlPipe::Status_e::e_Ok);
        break;
    default:
        assert(false);
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbHidInterface::getDescriptor(const uint16_t p_descriptor, const size_t p_len) const {
    const UsbHidDescriptorTypeId_e  descriptorType = static_cast<UsbHidDescriptorTypeId_e>((p_descriptor >> 8) & 0xFF);

    switch (descriptorType) {
    case e_HidReport:
        assert((p_descriptor & 0xFF) == 0);
        this->m_defaultCtrlPipe->dataIn(this->m_reportDescriptor, std::min(this->m_reportDescriptorLength, p_len));
        break;
    default:
        assert(false);
    }
}

} /* namespace usb */
