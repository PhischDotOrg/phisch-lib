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

    const ::usb::UsbDeviceDescriptor_t &        m_deviceDescriptor;
    const ::usb::UsbDeviceQualifierDescriptor_t m_deviceQualifierDescriptor;
    const ::usb::UsbStringDescriptors_t &       m_stringDescriptors;
    const ::usb::UsbDeviceStatus_t              m_deviceStatus;

public:
    typedef std::initializer_list<const ::usb::UsbConfiguration *> UsbConfigurationList_t;

    /* BUG? Marking the constructor as constexpr triggers an Internal Compiler Error (ICE) when compiling with GCC 9.2.1 and > -O0. */
#if 0 /* Error message below: */
[build] /Users/phs/Sandbox/stm32f4-usbdevice/main.cpp:514:1: internal compiler error: in output_constructor_regular_field, at varasm.c:5207
[build]   514 | } /* extern "C" */
[build]       | ^
[build] libbacktrace could not find executable to open
[build] Please submit a full bug report,
[build] with preprocessed source if appropriate.
[build] See <https://gcc.gnu.org/bugs/> for instructions.
#endif
    /* constexpr */ UsbDevice(UsbHwDevice &p_hwDevice, const UsbDeviceDescriptor_t &p_deviceDescriptor, const ::usb::UsbStringDescriptors_t &p_stringDescriptors, UsbConfigurationList_t p_configurations)
      : m_hwDevice(p_hwDevice), m_ctrlPipe(nullptr), m_activeConfiguration(0), m_configurations (),
        m_deviceDescriptor(p_deviceDescriptor),
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
