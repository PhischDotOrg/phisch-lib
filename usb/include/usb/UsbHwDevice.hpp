/*-
 * $Copyright$
-*/

#ifndef _USB_HW_DEVICE_HPP_D91A525D_4D15_4A10_96BA_9D39402CFB26
#define _USB_HW_DEVICE_HPP_D91A525D_4D15_4A10_96BA_9D39402CFB26

#include <stdint.h>

namespace usb {

/***************************************************************************//**
 * @brief Interface to the Hardware-specific Driver of an USB Device.
 * 
 * This encapsulates the Hardware-specific Device Operations needed by the
 * hardware-independent implementation of the UsbControlPipe.
 * 
 * \see stm32f4::UsbDeviceViaSTM32F4
 ******************************************************************************/
class UsbHwDevice {
public:
    /**
     * @brief USB Device Speed.
     * 
     * Enum to define the list of valid USB Device Speeds in the Hardware-specific
     * layer.
     */
    enum DeviceSpeed_e {
#if 0
        /* TODO USB High Speed not yet supported. */
        /**  @brief USB High Speed. */ 
        e_UsbHighSpeed = 0x0,
#endif
        /** @brief USB Speed is unknown. */
        e_UsbUnknown   = 0x1,   
        /** @brief USB Full Speed. */
        e_UsbFullSpeed = 0x3
    };

    /**
     * @brief Construct a new USB Hardware Device Object.
     * 
     * @param p_deviceSpeed USB Speed.
     */
    constexpr UsbHwDevice(const DeviceSpeed_e &p_deviceSpeed) : m_deviceSpeed(p_deviceSpeed) {
        
    };

    /**
     * @brief Destructor.
     * 
     */
    virtual ~UsbHwDevice() {};

    /**
     * @brief Set the Address of the USB Device.
     * 
     * Used by the UsbControlPipe when the USB Host sends the _Set Address_ Control Request.
     * 
     * \see UsbControlPipe::decodeDeviceRequest
     * \see UsbControlPipe::UsbRequest_t::e_SetAddress
     * 
     * @param p_address Address of the USB Device as sent by the USB Host in the _Set Address_ Control Request.
     */
    virtual void setAddress(const uint8_t p_address, const bool p_statusStageComplete) const = 0;

    virtual void start(void) const = 0;
    virtual void stop(void) const = 0;

protected:
    /** @brief Selected USB Device Speed. */
    const DeviceSpeed_e m_deviceSpeed;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_HW_DEVICE_HPP_D91A525D_4D15_4A10_96BA_9D39402CFB26 */
