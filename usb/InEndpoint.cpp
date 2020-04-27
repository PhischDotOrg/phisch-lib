/*-
 * $Copyright$
-*/

#include <usb/InEndpoint.hpp>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
InEndpoint::InEndpoint(UsbHwCtrlInEndpoint &p_hwEndpoint) : m_hwEndpoint(p_hwEndpoint) {
    this->m_hwEndpoint.registerEndpointCallback(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
InEndpoint::~InEndpoint() {
    this->m_hwEndpoint.unregisterEndpointCallback(*this);
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
