/*-
 * $Copyright$
-*/

#ifndef _USB_HW_DEVICE_HPP_D91A525D_4D15_4A10_96BA_9D39402CFB26
#define _USB_HW_DEVICE_HPP_D91A525D_4D15_4A10_96BA_9D39402CFB26

#include <stdint.h>

namespace usb {

/*******************************************************************************
 *
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
        /* FIXME USB High Speed not yet supported. */
#if 0
        /**  @brief USB High Speed. */ 
        e_UsbHighSpeed = 0x0,
#endif
        /** @brief USB Speed is unknown. */
        e_UsbUnknown   = 0x1,   
        /** @brief USB Full Speed. */
        e_UsbFullSpeed = 0x3
    };

    UsbHwDevice(const DeviceSpeed_e &p_deviceSpeed) : m_deviceSpeed(p_deviceSpeed) {
        
    };

    virtual ~UsbHwDevice() {};

    virtual void setAddress(const uint8_t p_address) const = 0;

    constexpr DeviceSpeed_e getDeviceSpeed(void) {
        return this->m_deviceSpeed;
    }

protected:
    const DeviceSpeed_e m_deviceSpeed;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_HW_DEVICE_HPP_D91A525D_4D15_4A10_96BA_9D39402CFB26 */
