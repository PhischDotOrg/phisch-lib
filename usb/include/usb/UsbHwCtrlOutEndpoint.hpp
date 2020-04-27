/*-
 * $Copyright$
-*/
#ifndef _USB_HW_CTRL_OUT_ENDPOINT_HPP_34794027_EB99_4B80_9FD2_AF640AE161FE
#define _USB_HW_CTRL_OUT_ENDPOINT_HPP_34794027_EB99_4B80_9FD2_AF640AE161FE

#include <usb/UsbHwOutEndpoint.hpp>
#include <stdint.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
class UsbHwCtrlOutEndpoint {
protected:
    UsbHwOutEndpoint *m_endpointCallback;

public:
    UsbHwCtrlOutEndpoint();
    virtual ~UsbHwCtrlOutEndpoint();

    void registerEndpointCallback(UsbHwOutEndpoint &p_endpointCallback);
    void unregisterEndpointCallback(UsbHwOutEndpoint &p_endpointCallback);
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_HW_CTRL_OUT_ENDPOINT_HPP_34794027_EB99_4B80_9FD2_AF640AE161FE */
