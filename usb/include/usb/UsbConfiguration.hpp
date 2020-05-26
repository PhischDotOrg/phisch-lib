/*-
 * $Copyright$
-*/

#ifndef __USB_CONFIGURATION_HPP_8D6BB979_1F97_40DC_9497_FF90641EE32F
#define __USB_CONFIGURATION_HPP_8D6BB979_1F97_40DC_9497_FF90641EE32F

#include <usb/UsbTypes.hpp>
#include <sys/types.h>

#include <usb/UsbInterface.hpp>

namespace usb {

class UsbInterface;

/*******************************************************************************
 *
 ******************************************************************************/
class UsbConfiguration {
private:
    UsbInterface &  m_interface;

public:
    constexpr UsbConfiguration(UsbInterface &p_interface) : m_interface(p_interface) {

    };

    virtual ~UsbConfiguration() {};

    void enable(UsbControlPipe &p_defaultCtrlPipe) const {
        this->m_interface.enable(p_defaultCtrlPipe);
    }

    void disable(void) const {
        this->m_interface.disable();
    }

    void handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) const {
        this->m_interface.handleCtrlRequest(p_setupPacket);
    }

    virtual const void *    getDescriptor(void) const = 0;
    virtual size_t          getDescriptorSize(void) const = 0;
};

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbConfigurationDescriptorT>
class UsbConfigurationT : public UsbConfiguration {
private:
    const UsbConfigurationDescriptorT & m_descriptor;

public:
    constexpr UsbConfigurationT(UsbInterface &p_interface, const UsbConfigurationDescriptorT &p_descriptor)
      : UsbConfiguration(p_interface), m_descriptor(p_descriptor) {

    }

    virtual ~UsbConfigurationT() {
    }

    virtual const void * getDescriptor(void) const override {
        return &this->m_descriptor;
    }

    virtual size_t getDescriptorSize(void) const override {
        return (this->m_descriptor.m_wTotalLength.m_hiByte << 8) | (this->m_descriptor.m_wTotalLength.m_loByte << 0);
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* __USB_CONFIGURATION_HPP_8D6BB979_1F97_40DC_9497_FF90641EE32F */
