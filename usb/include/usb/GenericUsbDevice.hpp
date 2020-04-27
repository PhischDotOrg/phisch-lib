/*-
 * $Copyright$
-*/

#ifndef _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B
#define _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B

#include <usb/UsbTypes.hpp>
#include <stdint.h>

namespace usb {

    class UsbHwDevice;

    class DefaultControlPipe;

/*******************************************************************************
 *
 ******************************************************************************/
class GenericUsbDevice {
    friend class DefaultControlPipe;

private:
    UsbHwDevice &m_hwDevice;
    DefaultControlPipe &m_ctrlPipe;

    const uint8_t                               m_maxConfigurations;
    const ::usb::UsbDeviceDescriptor_t          m_deviceDescriptor;
    const ::usb::UsbDeviceQualifierDescriptor_t m_deviceQualifierDescriptor;
    const void * const                          m_configurationDescriptor;
    const size_t                                m_configurationDescriptorSize;
    const ::usb::UsbStringDescriptors_t &       m_stringDescriptors;

public:
    GenericUsbDevice(UsbHwDevice &p_hwDevice, DefaultControlPipe &p_ctrlPipe,
      const void * const p_configurationDescriptor, size_t p_configurationDescriptorSize,
      const ::usb::UsbStringDescriptors_t &p_stringDescriptors);

    virtual ~GenericUsbDevice();

    void setAddress(const uint8_t p_address) const;
    void setConfiguration(const uint8_t p_configuration) const;
    void getStatus(const uint8_t p_len) const;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B */
