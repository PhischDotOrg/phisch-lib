/*-
 * $Copyright$
-*/

#ifndef _USB_IN_ENDPOINT_HPP_C46B5EE8_4887_4E06_BE28_F6DBFA70470D
#define _USB_IN_ENDPOINT_HPP_C46B5EE8_4887_4E06_BE28_F6DBFA70470D

#include <usb/UsbHwInEndpoint.hpp>

namespace usb {

class UsbHwInEndpoint;

/*******************************************************************************
 *
 ******************************************************************************/
class UsbInEndpoint {
protected:
    UsbHwInEndpoint &m_hwEndpoint;

public:
    UsbInEndpoint(UsbHwInEndpoint &p_hwEndpoint);

    virtual ~UsbInEndpoint();
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_IN_ENDPOINT_HPP_C46B5EE8_4887_4E06_BE28_F6DBFA70470D */
