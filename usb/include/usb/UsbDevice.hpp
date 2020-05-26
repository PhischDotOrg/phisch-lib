/*-
 * $Copyright$
-*/

#ifndef _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B
#define _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B

#include <usb/UsbTypes.hpp>
#include <stdint.h>

#include <usb/UsbConfiguration.hpp>

#include <initializer_list>

namespace usb {

    class UsbHwDevice;
    // class UsbControlPipe;

/*******************************************************************************
 *
 ******************************************************************************/
class UsbDevice {
    friend class UsbControlPipe;

private:
    UsbHwDevice &   m_hwDevice;
    UsbControlPipe *m_ctrlPipe;

    static const uint8_t                        m_maxConfigurations = 1;
    uint8_t                                     m_activeConfiguration;
    const ::usb::UsbConfiguration *             m_configurations[m_maxConfigurations];

    const ::usb::UsbDeviceDescriptor_t          m_deviceDescriptor;
    const ::usb::UsbDeviceQualifierDescriptor_t m_deviceQualifierDescriptor;
    const ::usb::UsbStringDescriptors_t &       m_stringDescriptors;
    const ::usb::UsbDeviceStatus_t              m_deviceStatus;

public:
    typedef const std::initializer_list<::usb::UsbConfiguration *> UsbConfigurationList_t;
    
    constexpr UsbDevice(UsbHwDevice &p_hwDevice, const ::usb::UsbStringDescriptors_t &p_stringDescriptors, UsbConfigurationList_t p_configurations)
      : m_hwDevice(p_hwDevice), m_ctrlPipe(nullptr), m_activeConfiguration(0), m_configurations {},
        m_deviceDescriptor {
            sizeof(m_deviceDescriptor),                                     /* m_bLength */
            ::usb::UsbDescriptorTypeId_t::e_Device,                         /* m_bDescriptorType */
            { 0x00, 0x02 },                                                 /* m_bLength */
            ::usb::UsbInterfaceClass_e::e_UsbInterface_Misc_EFh,            /* m_bDeviceClass */
            0x02,                                                           /* m_bDeviceSubClass */
            0x01,                                                           /* m_bDeviceProtocol */
            static_cast<uint8_t>(p_hwDevice.getDeviceSpeed() == ::usb::UsbHwDevice::DeviceSpeed_e::e_UsbFullSpeed ? 64 : 0), /* m_bMaxPacketSize0 */
            { 0xad, 0xde },                                                 /* m_idVendor */
            { 0xef, 0xbe },                                                 /* m_idProduct */
            { 0xfe, 0xca },                                                 /* m_bcdDevice */
            ::usb::UsbStringDescriptorId_t::e_StrDesc_Manufacturer,         /* e_iManufacturer */
            ::usb::UsbStringDescriptorId_t::e_StrDesc_Product,              /* e_iProduct */
            ::usb::UsbStringDescriptorId_t::e_StrDesc_SerialNumber,         /* e_iSerialNumber */
            m_maxConfigurations                                             /* e_bNumConfigurations */
        },
        m_deviceQualifierDescriptor(m_deviceDescriptor),
        m_stringDescriptors(p_stringDescriptors),
        m_deviceStatus { 1 }
    {
        unsigned idx = 0;

        /* TODO In C++14 we should be able to make this a static_assert() */
        assert(p_configurations.size() <= m_maxConfigurations);

        for (UsbConfigurationList_t::const_iterator cfg = p_configurations.begin(); cfg != p_configurations.end(); cfg++, idx++) {
            this->m_configurations[idx] = *cfg;
        }
    }    

    ~UsbDevice() {}

    void                            setAddress(const uint8_t p_address) const;
    const UsbConfiguration *        setConfiguration(const uint8_t p_configuration);

    constexpr uint8_t getActiveConfiguration(void) const {
        return this->m_activeConfiguration;
    }

    const void * getConfigurationDescriptor(void) const {
        assert(this->m_configurations[0] != nullptr);
        return this->m_configurations[0]->getDescriptor();
    }

    size_t getConfigurationDescriptorSize(void) const {
        assert(this->m_configurations[0] != nullptr);
        return this->m_configurations[0]->getDescriptorSize();
    }

    constexpr const ::usb::UsbDeviceStatus_t & getStatus(void) const {
        return this->m_deviceStatus;
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B */
