/*-
 * $Copyright$
-*/

#include <usb/UsbConfiguration.hpp>
#include <usb/UsbInterface.hpp>
#include <sys/types.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbConfigurationDescriptorT>
UsbConfigurationT<UsbConfigurationDescriptorT>::UsbConfigurationT(const UsbConfigurationDescriptorT &p_descriptor, UsbInterface &p_interface)
  : UsbConfiguration(), m_descriptor(p_descriptor), m_interface(p_interface), m_enabled(false) {

}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbConfigurationDescriptorT>
UsbConfigurationT<UsbConfigurationDescriptorT>::~UsbConfigurationT() {

}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbConfigurationDescriptorT>
void
UsbConfigurationT<UsbConfigurationDescriptorT>::enable(void) {
    this->m_interface.enable();
    this->m_enabled = true;
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbConfigurationDescriptorT>
void
UsbConfigurationT<UsbConfigurationDescriptorT>::disable(void) {
    this->m_enabled = false;
    this->m_interface.disable();
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbConfigurationDescriptorT>
bool
UsbConfigurationT<UsbConfigurationDescriptorT>::isEnabled(void) const {
    return this->m_enabled;
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbConfigurationDescriptorT>
const void *
UsbConfigurationT<UsbConfigurationDescriptorT>::getDescriptor(void) const {
    return &this->m_descriptor;
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbConfigurationDescriptorT>
size_t
UsbConfigurationT<UsbConfigurationDescriptorT>::getDescriptorSize(void) const {
    return (this->m_descriptor.m_wTotalLength.m_hiByte << 8) | (this->m_descriptor.m_wTotalLength.m_loByte << 0);
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
