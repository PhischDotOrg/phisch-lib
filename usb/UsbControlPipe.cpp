/*-
 * $Copyright$
-*/

#include <usb/UsbControlPipe.hpp>
#include <usb/UsbCtrlOutEndpoint.hpp>

namespace usb {


/*******************************************************************************
 *
 ******************************************************************************/
UsbControlPipe::UsbControlPipe(UsbCtrlInEndpoint &p_inEndpoint, UsbCtrlOutEndpoint &p_outEndpoint)
  : m_inEndpoint(p_inEndpoint), m_outEndpoint(p_outEndpoint) {
    this->m_outEndpoint.registerCtrlPipe(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
UsbControlPipe::~UsbControlPipe() {
  this->m_outEndpoint.unregisterCtrlPipe(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
