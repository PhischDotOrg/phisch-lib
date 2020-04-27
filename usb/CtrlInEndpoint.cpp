/*-
 * $Copyright$
-*/

#include <usb/CtrlInEndpoint.hpp>
#include <usb/UsbHwCtrlInEndpoint.hpp>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
CtrlInEndpoint::CtrlInEndpoint(UsbHwCtrlInEndpoint &p_hwEndpoint)
  : InEndpoint(p_hwEndpoint) {

}

/*******************************************************************************
 *
 ******************************************************************************/
CtrlInEndpoint::~CtrlInEndpoint() {

}

/*******************************************************************************
 *
 ******************************************************************************/
void
CtrlInEndpoint::write(const uint8_t * const p_data, const size_t p_dataLength, const size_t p_txLength) {
    this->m_hwEndpoint.write(p_data, p_dataLength, p_txLength);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
CtrlInEndpoint::writeString(const ::usb::UsbStringDescriptor &p_string, const size_t p_len) {
    this->m_hwEndpoint.writeString(p_string, p_len);
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
