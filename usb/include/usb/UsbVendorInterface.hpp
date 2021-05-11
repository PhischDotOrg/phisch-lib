/*-
 * $Copyright$
-*/

#ifndef _USB_VENDOR_INTERFACE_HPP_414154C1_B04A_457C_99F7_9F0E14D6C8F1
#define _USB_VENDOR_INTERFACE_HPP_414154C1_B04A_457C_99F7_9F0E14D6C8F1

#include <usb/UsbInterface.hpp>

/*****************************************************************************/
namespace usb {
/*****************************************************************************/

class UsbBulkOutEndpoint;

class UsbVendorInterface : public UsbInterface {
private:
    // UsbBulkOutEndpoint &    m_outEndpoint;
    // UsbBulkInEndpoint &     m_inEndpoint;

    enum class Request : uint8_t {
        e_NoData    = 0x01,
        e_DataOut   = 0x02,
        e_DataIn    = 0x03,
        e_Invalid   = 0xFF
    };
    static_assert(sizeof(Request) == 1);

    enum class ReturnCode : uint8_t {
        e_Ack       = 0,
        e_Timeout   = 1,
        e_Stall     = 2,
    };
    static_assert(sizeof(ReturnCode) == 1);

    void
    sendResponse(ReturnCode p_returnCode, uint8_t p_delayInMs, const void * const p_data = nullptr, const size_t p_length = 0) const;

public:
    UsbVendorInterface(UsbBulkOutEndpoint & /* p_outEndpoint */, const UsbBulkInEndpoint & /* p_inEndpoint */)
    //   : m_outEndpoint(p_outEndpoint), m_inEndpoint(p_inEndpoint) {
    {

    }

    virtual ~UsbVendorInterface() override {

    }

    void enable(void) const override;
    void disable(void) const override;

    void handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) override;
};

/*****************************************************************************/
} /* namespace usb */
/*****************************************************************************/

#endif /* _USB_VENDOR_INTERFACE_HPP_414154C1_B04A_457C_99F7_9F0E14D6C8F1 */
