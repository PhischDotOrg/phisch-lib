/*-
 * $Copyright$
-*/

#include <usb/UsbOutEndpoint.hpp>
#include <usb/UsbControlPipe.hpp>
#include <usb/UsbHwCtrlOutEndpoint.hpp>
#include <assert.h>
#include <stddef.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
UsbOutEndpoint::UsbOutEndpoint(UsbHwOutEndpoint &p_hwEndpoint)
  : m_hwEndpoint(p_hwEndpoint) {
      this->m_hwEndpoint.registerEndpointCallback(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
UsbOutEndpoint::~UsbOutEndpoint() {
    this->m_hwEndpoint.unregisterEndpointCallback(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
