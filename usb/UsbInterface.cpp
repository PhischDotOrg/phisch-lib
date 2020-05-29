/*-
 * $Copyright$
-*/

#include <usb/UsbInterface.hpp>
#include <usb/UsbControlPipe.hpp>
#include <usb/UsbInEndpoint.hpp>
#include <usb/UsbBulkOutEndpoint.hpp>

#include <assert.h>
#include <string.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbVcpInterface::enable(const UsbControlPipe &p_defaultCtrlPipe) {
    this->m_defaultCtrlPipe = &p_defaultCtrlPipe;

    this->m_outEndpoint.enable();
    this->m_inEndpoint.enable();
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbVcpInterface::disable(void) const {
    this->m_outEndpoint.disable();
    this->m_inEndpoint.disable();
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

        this->m_defaultCtrlPipe->write(reinterpret_cast<const uint8_t *>(&m_usbCdcLineCoding), sizeof(struct UsbCdcLineCoding_s));
        break;
    case e_SetControlLineState:
        assert(p_setupPacket.m_bmRequestType == 0x21);
        assert(p_setupPacket.m_wIndex == 0); /* FIXME Should be same as UsbInterface::m_interfaceNo */
        assert(p_setupPacket.m_wLength == 0);

        this->m_defaultCtrlPipe->write(nullptr, 0);
        break;
    case e_SetLineCoding:
        assert(p_setupPacket.m_wValue == 0);
        assert(p_setupPacket.m_wIndex == 0); /* FIXME Should be same as UsbInterface::m_interfaceNo */
        assert(p_setupPacket.m_wLength == 7);
        assert(p_setupPacket.m_bmRequestType == 0x21);

        this->m_defaultCtrlPipe->setDataStageBuffer(reinterpret_cast<uint32_t *>(&m_usbCdcLineCoding), sizeof(m_usbCdcLineCoding));
        break;
    default:
        assert(false);
    }
}

} /* namespace usb */
