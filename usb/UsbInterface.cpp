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
UsbVcpInterface::UsbVcpInterface(UsbControlPipe &p_defaultCtrlPipe,
  UsbBulkOutEndpoint &p_outEndpoint, UsbBulkInEndpoint &p_inEndpoint)
  : m_defaultCtrlPipe(p_defaultCtrlPipe), m_outEndpoint(p_outEndpoint), m_inEndpoint(p_inEndpoint) {
    m_defaultCtrlPipe.registerUsbInterface(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
UsbVcpInterface::~UsbVcpInterface() {
    m_defaultCtrlPipe.unregisterUsbInterface(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbVcpInterface::enable(void) const {
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
UsbVcpInterface::handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket, const void * const /* p_data */, const size_t /* p_length */) const {
    /*
     * Table 19: "Class-Specific Request Codes" from "Universal Serial Bus Class Definitions for Communications Devices", Rev. 1.2 (Errata 1) from Nov 3rd, 2010.
     */
    enum UsbCdcRequest_e {
        e_SetLineCoding         = 0x20,
        e_GetLineCoding         = 0x21,
        e_SetControlLineState   = 0x22,
    };

    /**
     * @brief USB Line Coding Structure
     * 
     * Line Coding Structure according to:
     * 
     * Table 17 "Line Coding Structure"
     * 
     * in
     * 
     * "Universal Serial Bus Communications Class Subclass Specification for PSTN Devices", Rev. 1.2 from Feb 9th, 2007.
     * 
     * As per the USB Standard, this struct is 7 Bytes long. However, because we're transmitting
     * OUT Packets in words (i.e. chunks of four bytes) into application memory, this struct should
     * be aligned on a 4-Byte boundary. This will effectively increase the RAM footprint of the
     * structure to 8 Bytes.
     * 
     * \c see ::usb::stm32f4::OutEndpointViaSTM32F4::dataReceivedDeviceCallback
     */
    struct UsbCdcLineCoding_s {
        union {
            uint32_t    m_u32;
            uint8_t     m_u8[4];
        } m_dwDTERate;
        uint8_t         m_bCharFormat;
        uint8_t         m_bParityType;
        uint8_t         m_bDataBits;
    } __attribute__((aligned(4), packed));
    static_assert(sizeof(struct UsbCdcLineCoding_s) >= 7);

    static struct UsbCdcLineCoding_s usbCdcLineCoding = { };

    UsbCdcRequest_e request = static_cast<UsbCdcRequest_e>(p_setupPacket.m_bRequest);

    switch (request) {
    case e_GetLineCoding:
        assert(p_setupPacket.m_wValue == 0);
        assert(p_setupPacket.m_wIndex == 0); /* FIXME Should be same as UsbInterface::m_interfaceNo */
        assert(p_setupPacket.m_wLength == 7);
        assert(p_setupPacket.m_bmRequestType == 0xA1);

        this->m_defaultCtrlPipe.write(reinterpret_cast<const uint8_t *>(&usbCdcLineCoding), sizeof(struct UsbCdcLineCoding_s));
        break;
    case e_SetControlLineState:
        assert(p_setupPacket.m_bmRequestType == 0x21);
        assert(p_setupPacket.m_wIndex == 0); /* FIXME Should be same as UsbInterface::m_interfaceNo */
        assert(p_setupPacket.m_wLength == 0);

        this->m_defaultCtrlPipe.write(nullptr, 0);
        break;
    case e_SetLineCoding:
        assert(p_setupPacket.m_wValue == 0);
        assert(p_setupPacket.m_wIndex == 0); /* FIXME Should be same as UsbInterface::m_interfaceNo */
        assert(p_setupPacket.m_wLength == 7);
        assert(p_setupPacket.m_bmRequestType == 0x21);

        this->m_defaultCtrlPipe.setDataStageBuffer(reinterpret_cast<uint32_t *>(&usbCdcLineCoding), sizeof(usbCdcLineCoding));
        break;
    default:
        assert(false);
    }
}

} /* namespace usb */
