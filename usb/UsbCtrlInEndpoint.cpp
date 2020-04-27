/*-
 * $Copyright$
-*/

#include <usb/UsbCtrlInEndpoint.hpp>
#include <usb/UsbHwInEndpoint.hpp>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
UsbCtrlInEndpoint::UsbCtrlInEndpoint(UsbHwInEndpoint &p_hwEndpoint)
  : m_hwEndpoint(p_hwEndpoint) {

}

/*******************************************************************************
 *
 ******************************************************************************/
UsbCtrlInEndpoint::~UsbCtrlInEndpoint() {

}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbCtrlInEndpoint::write(const uint8_t * const p_data, const size_t p_dataLength, const size_t p_txLength) {
    this->m_hwEndpoint.write(p_data, p_dataLength, p_txLength);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbCtrlInEndpoint::writeString(const ::usb::UsbStringDescriptor &p_string, const size_t p_len) {
    this->m_hwEndpoint.writeString(p_string, p_len);
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
