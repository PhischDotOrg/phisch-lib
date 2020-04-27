/*-
 * $Copyright$
-*/

#include <usb/UsbHwCtrlOutEndpoint.hpp>
#include <stddef.h>
#include <assert.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
UsbHwCtrlOutEndpoint::UsbHwCtrlOutEndpoint(void) : m_endpointCallback(NULL) {

}

/*******************************************************************************
 *
 ******************************************************************************/
UsbHwCtrlOutEndpoint::~UsbHwCtrlOutEndpoint(void) {

}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbHwCtrlOutEndpoint::registerEndpointCallback(UsbHwOutEndpoint &p_endpointCallback) {
    assert(this->m_endpointCallback == NULL);
    this->m_endpointCallback = &p_endpointCallback;
};

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbHwCtrlOutEndpoint::unregisterEndpointCallback(UsbHwOutEndpoint &p_endpointCallback) {
    assert(this->m_endpointCallback == &p_endpointCallback);
    this->m_endpointCallback = NULL;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
