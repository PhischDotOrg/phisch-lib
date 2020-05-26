/*-
 * $Copyright$
-*/

#ifndef _USB_INTERFACE_HPP_03CCC19E_822C_44A4_9213_B8215F901BE7
#define _USB_INTERFACE_HPP_03CCC19E_822C_44A4_9213_B8215F901BE7

#include <usb/UsbInEndpoint.hpp>
#include <usb/UsbBulkOutEndpoint.hpp>
#include <usb/UsbTypes.hpp>

namespace usb {

class UsbControlPipe;

/*******************************************************************************
 *
 ******************************************************************************/
class UsbInterface {
public:
    virtual         ~UsbInterface() {};

    virtual void    enable(const UsbControlPipe &p_defaultCtrlPipe) = 0;
    virtual void    disable(void) const = 0;
    virtual void    handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) const = 0;
};

/*******************************************************************************
 *
 ******************************************************************************/
class UsbVcpInterface : public UsbInterface {
private:
    const UsbControlPipe *  m_defaultCtrlPipe;
    UsbBulkOutEndpoint &    m_outEndpoint;
    UsbBulkInEndpoint &     m_inEndpoint;

public:
    UsbVcpInterface(UsbBulkOutEndpoint &p_outEndpoint, UsbBulkInEndpoint &p_inEndpoint)
      : m_outEndpoint(p_outEndpoint), m_inEndpoint(p_inEndpoint) {
    }

    virtual ~UsbVcpInterface() override {

    }

    void    enable(const UsbControlPipe &p_defaultCtrlPipe) override;
    void    disable(void) const override;

    void    handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) const override;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_INTERFACE_HPP_03CCC19E_822C_44A4_9213_B8215F901BE7 */
