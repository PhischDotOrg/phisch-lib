/*-
 * $Copyright$
-*/

#ifndef _USB_OUT_ENDPOINT_HPP_CA239521_AD89_43DD_9F28_FCCA4EF81EDC
#define _USB_OUT_ENDPOINT_HPP_CA239521_AD89_43DD_9F28_FCCA4EF81EDC

#include <usb/UsbHwOutEndpoint.hpp>

namespace usb {

    class UsbHwOutEndpoint;

/*******************************************************************************
 *
 ******************************************************************************/
class UsbOutEndpoint {
private:
    UsbHwOutEndpoint &m_hwEndpoint;

public:
            UsbOutEndpoint(UsbHwOutEndpoint &p_hwEndpoint);
    virtual ~UsbOutEndpoint();

    virtual void rxData(const uint32_t p_rxData) = 0;
    virtual void transferComplete(void) = 0;
    virtual void setupComplete(const void * p_setupData, const size_t p_length) = 0;

    virtual void *getSetupPacketBuffer(size_t * const p_length) = 0;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_OUT_ENDPOINT_HPP_CA239521_AD89_43DD_9F28_FCCA4EF81EDC */
