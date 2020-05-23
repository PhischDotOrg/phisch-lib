/*-
 * $Copyright$
-*/

#ifndef _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161
#define _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161

#include <usb/UsbTypes.hpp>
#include <stdint.h>

#include <usb/UsbCtrlOutEndpoint.hpp>

#include <usb/UsbInEndpoint.hpp>

namespace usb {

class UsbDevice;
class UsbInterface;

/*******************************************************************************
 *
 ******************************************************************************/
class UsbControlPipe {
public:
/*******************************************************************************
 *
 ******************************************************************************/
    typedef enum UsbRecipient_e {
        e_Device        = 0x00,
        e_Interface     = 0x01,
        e_Endpoint      = 0x02,
        e_Other         = 0x03
    } UsbRecipient_t;

    typedef enum UsbRequest_e {
        e_GetStatus         = 0x00,
        e_ClearFeature      = 0x01,
        e_SetFeature        = 0x03,
        e_SetAddress        = 0x05,
        e_GetDescriptor     = 0x06,
        e_SetDescriptor     = 0x07,
        e_GetConfiguration  = 0x08,
        e_SetConfiguration  = 0x09,
        e_GetInterface      = 0x0A,
        e_SetInterface      = 0x0B,
        e_SyncFrame         = 0x0C
    } UsbRequest_t;

/*******************************************************************************
 *
 ******************************************************************************/
    UsbControlPipe(UsbCtrlInEndpoint &p_inEndpoint, UsbCtrlOutEndpoint &p_outEndpoint);
    ~UsbControlPipe();

    constexpr void registerUsbDevice(UsbDevice &p_usbDevice) {
        assert(this->m_usbDevice == nullptr);
        this->m_usbDevice = &p_usbDevice;
    }

    constexpr void unregisterUsbDevice(UsbDevice &p_usbDevice) {
        assert(this->m_usbDevice != NULL);
        assert(this->m_usbDevice == &p_usbDevice);

        this->m_usbDevice = NULL;
    }

    constexpr void registerUsbInterface(UsbInterface &p_usbInterface) {
        assert(this->m_usbInterface == nullptr);

        this->m_usbInterface = &p_usbInterface;
    }

    constexpr void unregisterUsbInterface(UsbInterface &p_usbInterface) {
        assert(this->m_usbInterface != nullptr);
        assert(this->m_usbInterface == &p_usbInterface);

        this->m_usbInterface = NULL;
    }

    void    setupStageComplete(const ::usb::UsbSetupPacket_t &p_setupPacket) const;

    void setDataStageBuffer(uint32_t * const p_buffer, const size_t p_length) const {
        this->m_outEndpoint.setDataStageBuffer(p_buffer, p_length);
    }

    void    transferComplete(const size_t p_numBytes) const;

    constexpr void write(const uint8_t * const p_data, const size_t p_length) const {
        this->m_inEndpoint.write(p_data, p_length);
    }

protected:
    UsbCtrlInEndpoint &m_inEndpoint;
    UsbCtrlOutEndpoint &m_outEndpoint;

private:
    /**
     * @brief Pointer to the USB Device Object.
     * 
     * Provides access to the USB Device specific operations (e.g. Data Transmission).
     */
    UsbDevice *m_usbDevice;

    UsbInterface *m_usbInterface;

    void getDescriptor(const uint16_t p_descriptor, const size_t p_len) const;
    void getDeviceDescriptor(const uint8_t p_descriptorId, const size_t p_len) const;
    void getDeviceQualifierDescriptor(const uint8_t p_descriptorId, const size_t p_len) const;
    void getConfigurationDescriptor(const uint8_t p_descriptorId, const size_t p_len) const;
    void getStringDescriptor(const uint8_t p_descriptorId, const size_t p_len) const;
    void getDeviceConfiguration(const uint8_t p_len) const;

    void decodeSetupPacket(const ::usb::UsbSetupPacket_t &p_setupPacket, const void * const p_data = nullptr, const size_t p_length = 0) const;
    void decodeDeviceRequest(const UsbSetupPacket_t &p_setupPacket, const void * const p_data, const size_t p_length) const;
    void decodeInterfaceRequest(const UsbSetupPacket_t &p_setupPacket, const void * const p_data, const size_t p_length) const;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161 */
