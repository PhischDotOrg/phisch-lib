
/*-
 * $Copyright$
-*/

#include <usb/UsbHwOutEndpoint.hpp>
#include <stddef.h>
#include <assert.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
UsbHwOutEndpoint::UsbHwOutEndpoint(void) {

}

/*******************************************************************************
 *
 ******************************************************************************/
UsbHwOutEndpoint::~UsbHwOutEndpoint(void) {

}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbHwOutEndpoint::registerEndpointCallback(UsbOutEndpoint &p_endpointCallback) {
    assert(this->m_endpointCallback == NULL);
    this->m_endpointCallback = &p_endpointCallback;
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbHwOutEndpoint::unregisterEndpointCallback(UsbOutEndpoint &p_endpointCallback) {
    assert(this->m_endpointCallback == &p_endpointCallback);
    this->m_endpointCallback = NULL;
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
