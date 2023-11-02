/*-
 * $Copyright$
-*/

#ifndef _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B
#define _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B

#include <usb/UsbTypes.hpp>
#include <cstdint>
#include <cassert>

#include <usb/UsbConfiguration.hpp>
#include <usb/UsbHwDevice.hpp>

#include <initializer_list>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
class UsbDevice {
private:
    UsbHwDevice &       m_hwDevice;
    UsbControlPipe *    m_ctrlPipe;

    static const uint8_t                        m_maxConfigurations = 1;
    uint8_t                                     m_activeConfiguration;
    const ::usb::UsbConfiguration *             m_configurations[m_maxConfigurations];

    const ::usb::UsbDeviceDescriptor_t &        m_deviceDescriptor;
    const ::usb::UsbDeviceQualifierDescriptor_t m_deviceQualifierDescriptor;
    const ::usb::UsbStringDescriptors_t &       m_stringDescriptors;
    const ::usb::UsbDeviceStatus_t              m_deviceStatus;

    void getStringDescriptor(const uint8_t p_descriptorId, const uint8_t p_len) const;

    constexpr const void * getConfigurationDescriptor(void) const {
        assert(this->m_configurations[0] != nullptr);
        return this->m_configurations[0]->getDescriptor();
    }

    constexpr size_t getConfigurationDescriptorSize(void) const {
        assert(this->m_configurations[0] != nullptr);
        return this->m_configurations[0]->getDescriptorSize();
    }

public:
    typedef std::initializer_list<const ::usb::UsbConfiguration *> UsbConfigurationList_t;

    constexpr UsbDevice(UsbHwDevice &p_hwDevice, const UsbDeviceDescriptor_t &p_deviceDescriptor, const ::usb::UsbStringDescriptors_t &p_stringDescriptors, UsbConfigurationList_t p_configurations)
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

    ~UsbDevice() = default;

    /**
     * @name Standard USB Device Requests.
     * 
     * These methods are used by the USB Default Control Pipe to serve
     * the Standard USB Device Requests.
     */
///@{
    template<bool p_statusStageComplete>
    void setAddress(const uint8_t p_address) const {
        m_hwDevice.setAddress(p_address, p_statusStageComplete);
    }

    const UsbConfiguration *        setConfiguration(const uint8_t p_configuration);

    constexpr uint8_t getActiveConfiguration(void) const {
        return this->m_activeConfiguration;
    }

    constexpr const ::usb::UsbDeviceStatus_t & getStatus(void) const {
        return this->m_deviceStatus;
    }

    void getDescriptor(const uint16_t p_descriptor, const size_t p_len) const;
///@}

    void registerUsbCtrlPipe(UsbControlPipe &p_ctrlPipe) {
        assert(this->m_ctrlPipe == nullptr);
        this->m_ctrlPipe = &p_ctrlPipe;
    }

    void unregisterUsbCtrlPipe(void) {
        assert(this->m_ctrlPipe != nullptr);
        this->m_ctrlPipe = nullptr;
    }

    void start(void) const { m_hwDevice.start(); }
    void stop(void) const {  m_hwDevice.stop(); };
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _GENERIC_USBDEVICE_HPP_74B29084_AF57_422E_B8D6_F7EBF1354A9B */
