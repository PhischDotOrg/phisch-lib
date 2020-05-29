/*-
 * $Copyright$
-*/

#ifndef _USB_BULK_OUT_ENDPOINT_HPP_74676FA3_9207_4D3B_B704_CBEA94B67142
#define _USB_BULK_OUT_ENDPOINT_HPP_74676FA3_9207_4D3B_B704_CBEA94B67142

#include <usb/UsbTypes.hpp>
#include <cstddef>

namespace usb {

    namespace stm32f4 {
        class BulkOutEndpointViaSTM32F4;
    }

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbHwBulkOutEndpointT>
class UsbBulkOutEndpointT {
private:
    UsbHwBulkOutEndpointT &                     m_hwEndpoint;
    uint32_t                                    m_inData;

public:
            UsbBulkOutEndpointT(UsbHwBulkOutEndpointT &p_hwEndpoint);
    virtual ~UsbBulkOutEndpointT();

    void disable(void) const;
    void enable(void) const;

    void transferComplete(const size_t p_numBytes) const;
};

/*******************************************************************************
 *
 ******************************************************************************/
typedef UsbBulkOutEndpointT<::usb::stm32f4::BulkOutEndpointViaSTM32F4> UsbBulkOutEndpoint;

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_BULK_OUT_ENDPOINT_HPP_74676FA3_9207_4D3B_B704_CBEA94B67142 */
