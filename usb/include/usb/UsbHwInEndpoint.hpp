/*-
 * $Copyright$
-*/

#ifndef _USB_HW_CTRL_IN_ENDPOINT_HPP_7E4C459F_E88C_4C2B_8197_16A54BCE8AF0
#define _USB_HW_CTRL_IN_ENDPOINT_HPP_7E4C459F_E88C_4C2B_8197_16A54BCE8AF0

#include <usb/UsbTypes.hpp>
#include <stdint.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
class UsbHwInEndpoint {
protected:

public:
            UsbHwInEndpoint(void);
    virtual ~UsbHwInEndpoint();

    virtual void write(const uint8_t * const p_data, const size_t p_dataLength, const size_t p_txLength) = 0;
    virtual void writeString(const ::usb::UsbStringDescriptor &p_string, const size_t p_len) = 0;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_HW_CTRL_IN_ENDPOINT_HPP_7E4C459F_E88C_4C2B_8197_16A54BCE8AF0 */
