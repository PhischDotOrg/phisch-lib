/*-
 * $Copyright$
-*/

#ifndef _USB_CTRL_IN_ENDPOINT_HPP_1588B055_5CA7_45BB_B136_C87B236B998C
#define _USB_CTRL_IN_ENDPOINT_HPP_1588B055_5CA7_45BB_B136_C87B236B998C

#include <usb/UsbTypes.hpp>
#include <usb/UsbHwInEndpoint.hpp>
#include <stdint.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
class UsbCtrlInEndpoint {
private:
    UsbHwInEndpoint &m_hwEndpoint;

public:
            UsbCtrlInEndpoint(UsbHwInEndpoint &p_hwEndpoint);
    virtual ~UsbCtrlInEndpoint();

    void write(const uint8_t * const p_data, const size_t p_dataLength, const size_t p_txLength);
    void writeString(const ::usb::UsbStringDescriptor &p_string, const size_t p_len);
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_CTRL_IN_ENDPOINT_HPP_1588B055_5CA7_45BB_B136_C87B236B998C */
