/*-
 * $Copyright$
-*/

#ifndef _USB_INTERFACE_HPP_03CCC19E_822C_44A4_9213_B8215F901BE7
#define _USB_INTERFACE_HPP_03CCC19E_822C_44A4_9213_B8215F901BE7

#include <usb/UsbInEndpoint.hpp>
#include <usb/UsbTypes.hpp>

namespace usb {

class UsbBulkOutEndpoint;
class UsbControlPipe;

/*******************************************************************************
 *
 ******************************************************************************/
class UsbInterface {
protected:
    const UsbControlPipe *  m_defaultCtrlPipe;

public:
    virtual         ~UsbInterface() {};

    void enable(const UsbControlPipe &p_defaultCtrlPipe) {
        this->m_defaultCtrlPipe = &p_defaultCtrlPipe;
        this->enable();
    };

    virtual void    enable(void) const = 0;
    virtual void    disable(void) const = 0;
    /*
     * Can't be a const-method b/c the concrete implementation will likely have to
     * call UsbControlPipe::setDataStageBuffer to set up the receive buffer for the
     * data stage. This is a non-const operation.
     */
    virtual void    handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) = 0;
};

/*******************************************************************************
 *
 ******************************************************************************/
class UsbVcpInterface : public UsbInterface {
private:
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
    typedef struct UsbCdcLineCoding_s UsbCdcLineCoding_t;
    
    UsbCdcLineCoding_t      m_usbCdcLineCoding;

    UsbBulkOutEndpoint &    m_outEndpoint;
    UsbBulkInEndpoint &     m_inEndpoint;

public:
    UsbVcpInterface(UsbBulkOutEndpoint &p_outEndpoint, UsbBulkInEndpoint &p_inEndpoint)
      : m_outEndpoint(p_outEndpoint), m_inEndpoint(p_inEndpoint) {
    }

    virtual ~UsbVcpInterface() override {

    }

    void    enable(void) const override;
    void    disable(void) const override;

    void    handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) override;
};

/*******************************************************************************
 *
 ******************************************************************************/
class UsbVendorInterface : public UsbInterface {
private:
    UsbBulkOutEndpoint &    m_outEndpoint;
    UsbBulkInEndpoint &     m_inEndpoint;

public:
    UsbVendorInterface(UsbBulkOutEndpoint &p_outEndpoint, UsbBulkInEndpoint &p_inEndpoint)
      : m_outEndpoint(p_outEndpoint), m_inEndpoint(p_inEndpoint) {

    }

    virtual ~UsbVendorInterface() override {

    }

    void enable(void) const override;
    void disable(void) const override;

    void handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) override;
};

/*******************************************************************************
 *
 ******************************************************************************/
class UsbHidInterface : public UsbInterface {
private:
    UsbIrqInEndpoint &  m_inEndpoint;

public:
    UsbHidInterface(UsbIrqInEndpoint &p_inEndpoint)
      : m_inEndpoint(p_inEndpoint) {

    }

    virtual ~UsbHidInterface() override {

    }

    void enable(void) const override;
    void disable(void) const override;

    void handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) override;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_INTERFACE_HPP_03CCC19E_822C_44A4_9213_B8215F901BE7 */
