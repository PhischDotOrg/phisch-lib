/*-
 * $Copyright$
-*/

#ifndef __USB_CONFIGURATION_HPP_8D6BB979_1F97_40DC_9497_FF90641EE32F
#define __USB_CONFIGURATION_HPP_8D6BB979_1F97_40DC_9497_FF90641EE32F

#include <usb/UsbTypes.hpp>
#include <sys/types.h>

#include <usb/UsbInterface.hpp>

#include <phisch/log.h>

namespace usb {

class UsbControlPipe;

/***************************************************************************//**
 * @brief Class Interface that represents a USB Configuration.
 * 
 * In the USB Stack, an USB Configuration refers to a single of set of USB
 * Interfaces that can be activated and deactivated as a whole through the
 * default control pipe.
 ******************************************************************************/
class UsbConfiguration {
private:
    /** @brief Reference to the USB Interface. */
    UsbInterface &  m_interface;

    /** @brief Reference to the USB Configuration Descriptor.
     * 
     * The format of the USB Configuration is defined by the USB Standards.
     * 
     * @see UsbConfigurationDescriptorT
     */
    const UsbConfigurationDescriptor_t & m_descriptor;

public:
    /**
     * @brief Constructor.
     * 
     * @param p_interface Reference to an object that represents an USB Interface.
     * @param p_descriptor Reference to the USB Configuration Descriptor.
     */
    constexpr UsbConfiguration(UsbInterface &p_interface, const UsbConfigurationDescriptor_t &p_descriptor)
      : m_interface(p_interface), m_descriptor(p_descriptor) {

    };

    /** @brief Destructor. */
    ~UsbConfiguration() = default;

    /**
     * @brief Enables the USB Configuration in the Device.
     * 
     * Enables the USB Configuration in the Device.
     * 
     * This method is indirectly, i.e. through the UsbDevice Class, used by the Default
     * Control Pipe to handle the _Set Configuration_ Request on the Device.
     * 
     * @param p_defaultCtrlPipe Reference to the Default Control Pipe.
     * 
     * @see UsbControlPipe::decodeDeviceRequest
     * @see UsbDevice::setConfiguration
     */
    void enable(UsbControlPipe &p_defaultCtrlPipe) const {
        this->m_interface.enable(p_defaultCtrlPipe);
    }

    /**
     * @brief Disables the USB Configuration in the Device.
     * 
     * Disables the USB Configuration in the Device.
     * 
     * This method is indirectly, i.e. through the UsbDevice Class, used by the Default
     * Control Pipe to handle the _Set Configuration_ Request on the Device.
     * 
     * @see UsbControlPipe::decodeDeviceRequest
     * @see UsbDevice::setConfiguration
     */
    void disable(void) const {
        this->m_interface.disable();
    }

    /**
     * @brief Forwards a USB Control Request to the unterlying USB Interface.
     * 
     * @param p_setupPacket Reference to the USB Setup Packet.
     */
    void handleCtrlRequest(const UsbSetupPacket_t &p_setupPacket) const {
        this->m_interface.handleCtrlRequest(p_setupPacket);
    }

    /** @brief Return the address of the USB Configuration Descriptor.
     * 
     * @return Address of the USB Configuration Descriptor.
     */
    constexpr const void * getDescriptor(void) const {
        return &this->m_descriptor;
    };

    /** @brief Get the length of the USB Configuration Descriptor.
     * 
     * @return Size of the USB Configuration Descriptor in Bytes.
     */
    constexpr size_t getDescriptorSize(void) const {
        return (this->m_descriptor.m_wTotalLength.m_hiByte << 8) | (this->m_descriptor.m_wTotalLength.m_loByte << 0);
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* __USB_CONFIGURATION_HPP_8D6BB979_1F97_40DC_9497_FF90641EE32F */
