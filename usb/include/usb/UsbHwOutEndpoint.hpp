/*-
 * $Copyright$
-*/
#ifndef _OUT_ENDPOINT_HPP_23F63D27_C8CD_491D_9EE6_07CF2EF8D4B3
#define _OUT_ENDPOINT_HPP_23F63D27_C8CD_491D_9EE6_07CF2EF8D4B3

#include <stdint.h>

namespace usb {

class UsbOutEndpoint;

/*******************************************************************************
 *
 ******************************************************************************/
class UsbHwOutEndpoint {
protected:
    /*******************************************************************************
     * Callback to USB-generic OUT Endpoint implementation
     ******************************************************************************/
    UsbOutEndpoint *    m_endpointCallback;

public:
            UsbHwOutEndpoint(void);
    virtual ~UsbHwOutEndpoint();

    void registerEndpointCallback(UsbOutEndpoint &p_endpointCallback);
    void unregisterEndpointCallback(UsbOutEndpoint &p_endpointCallback);
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _OUT_ENDPOINT_HPP_23F63D27_C8CD_491D_9EE6_07CF2EF8D4B3 */
