/*-
 * $Copyright$
-*/

#include <usb/UsbHwCtrlInEndpoint.hpp>
#include <stddef.h>
#include <assert.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
UsbHwCtrlInEndpoint::UsbHwCtrlInEndpoint(void) : m_endpointCallback(NULL) {

}

/*******************************************************************************
 *
 ******************************************************************************/
UsbHwCtrlInEndpoint::~UsbHwCtrlInEndpoint(void) {

}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbHwCtrlInEndpoint::registerEndpointCallback(InEndpoint &p_endpointCallback) {
    assert(this->m_endpointCallback == NULL);
    this->m_endpointCallback = &p_endpointCallback;
};

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbHwCtrlInEndpoint::unregisterEndpointCallback(InEndpoint &p_endpointCallback) {
    assert(this->m_endpointCallback == &p_endpointCallback);
    this->m_endpointCallback = NULL;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
