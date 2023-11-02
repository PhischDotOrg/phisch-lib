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
class UsbBulkInEndpoint;

struct _UsbVendorInterfaceHelper {
    template<size_t p_length>
    static constexpr
    auto
    initialize_array(void) -> const std::array<uint8_t, p_length> {
        std::array<uint8_t, p_length> data {};

        for (unsigned idx = 0; idx < p_length; idx++) {
            data[idx] = idx;
        }

        return data;
    }

    template<typename T>
    static constexpr
    bool
    check_array(const T& p_array) {
        bool rc = false;

        for (unsigned idx = 0; (idx < p_array.size()) && !rc; idx++) {
            rc = (p_array[idx] == idx);
        }

        return rc;
    }
};

class UsbVendorInterface : public UsbInterface {
private:
    const UsbBulkOutEndpoint &  m_outEndpoint;
    const UsbBulkInEndpoint &   m_inEndpoint;

    static constexpr size_t     m_maxDataOutLen = 256;
    static constexpr size_t     m_maxDataInLen  = 256;

    static constexpr std::array<uint8_t, m_maxDataInLen> m_dataInBuffer = _UsbVendorInterfaceHelper::initialize_array<m_maxDataInLen>();
    std::array<uint8_t, m_maxDataInLen> m_dataOutBuffer {};

    static_assert(m_dataInBuffer[0] == 0);
    static_assert(m_dataInBuffer[62] == 62);
    static_assert(_UsbVendorInterfaceHelper::check_array(m_dataInBuffer));

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
    UsbVendorInterface(UsbBulkOutEndpoint &p_outEndpoint, const UsbBulkInEndpoint &p_inEndpoint)
      : m_outEndpoint(p_outEndpoint), m_inEndpoint(p_inEndpoint) {

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
