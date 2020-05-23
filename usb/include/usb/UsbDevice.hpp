/*-
 * $Copyright$
-*/

#ifndef _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B
#define _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B

#include <usb/UsbTypes.hpp>
#include <stdint.h>

namespace usb {

    class UsbConfiguration;
    class UsbHwDevice;
    class UsbControlPipe;

/*******************************************************************************
 *
 ******************************************************************************/
class UsbDevice {
    friend class UsbControlPipe;

private:
    UsbHwDevice &m_hwDevice;
    UsbControlPipe &m_ctrlPipe;

    const uint8_t                               m_maxConfigurations;
    const ::usb::UsbDeviceDescriptor_t          m_deviceDescriptor;
    const ::usb::UsbDeviceQualifierDescriptor_t m_deviceQualifierDescriptor;
    ::usb::UsbConfiguration &                   m_configuration;
    const ::usb::UsbStringDescriptors_t &       m_stringDescriptors;

public:
    UsbDevice(UsbHwDevice &p_hwDevice, UsbControlPipe &p_ctrlPipe,
      UsbConfiguration &p_configuration,
      const ::usb::UsbStringDescriptors_t &p_stringDescriptors);

    virtual ~UsbDevice();

    void    setAddress(const uint8_t p_address) const;
    void    setConfiguration(const uint8_t p_configuration) const;
    uint8_t getConfiguration(void) const;
    void    getStatus(const uint8_t p_len) const;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B */
