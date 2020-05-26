/*-
 * $Copyright$
-*/

#ifndef _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161
#define _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161

#include <usb/UsbTypes.hpp>
#include <stdint.h>

#include <usb/UsbDevice.hpp>
#include <usb/UsbInEndpoint.hpp>

namespace usb {

class UsbDevice;
class UsbConfiguration;
class UsbCtrlOutEndpoint;

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

private:
    UsbCtrlInEndpoint &m_inEndpoint;
    UsbCtrlOutEndpoint *m_outEndpoint;

    /**
     * @brief Pointer to the USB Device Object.
     * 
     * Provides access to the USB Device specific operations (e.g. Data Transmission).
     */
    UsbDevice &m_usbDevice;

    const UsbConfiguration * m_activeConfiguration;

    void getDescriptor(const uint16_t p_descriptor, const size_t p_len) const;
    void getDeviceDescriptor(const size_t p_len) const;
    void getDeviceQualifierDescriptor(const size_t p_len) const;
    void getConfigurationDescriptor(const size_t p_len) const;
    void getStringDescriptor(const uint8_t p_descriptorId, const size_t p_len) const;
    void getDeviceConfiguration(void) const;

    void decodeSetupPacket(const ::usb::UsbSetupPacket_t &p_setupPacket, const void * const p_data = nullptr, const size_t p_length = 0);
    void decodeDeviceRequest(const UsbSetupPacket_t &p_setupPacket, const void * const p_data, const size_t p_length);
    void decodeInterfaceRequest(const UsbSetupPacket_t &p_setupPacket) const;

public:
/*******************************************************************************
 *
 ******************************************************************************/
    constexpr UsbControlPipe(UsbDevice &p_usbDevice, UsbCtrlInEndpoint &p_inEndpoint)
      : m_inEndpoint(p_inEndpoint), m_outEndpoint(nullptr), m_usbDevice(p_usbDevice), m_activeConfiguration(nullptr) {
          this->m_usbDevice.m_ctrlPipe = this;
    }

    ~UsbControlPipe() {

    }

    constexpr void registerCtrlOutEndpoint(UsbCtrlOutEndpoint &p_outEndpoint) {
        assert(this->m_outEndpoint == nullptr);
        this->m_outEndpoint = &p_outEndpoint;
    }

    constexpr void unregisterCtrlOutEndpoint(void) {
        assert(this->m_outEndpoint != nullptr);

        this->m_outEndpoint = nullptr;
    }

    void    setupStageComplete(const ::usb::UsbSetupPacket_t &p_setupPacket);

    void    setDataStageBuffer(uint32_t * const p_buffer, const size_t p_length) const;

    void    transferComplete(const size_t p_numBytes) const;

    constexpr void write(const uint8_t * const p_data, const size_t p_length) const {
        this->m_inEndpoint.write(p_data, p_length);
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161 */
