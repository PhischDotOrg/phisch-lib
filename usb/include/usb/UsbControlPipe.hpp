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

/***************************************************************************//**
 * @brief USB Default Control Pipe Handler.
 * 
 * This class handles Default Control Requests as specified by the USB 2.0
 * Standard.
 * 
 * Declared as \c final because the destructor is not declared \c virtual .
 ******************************************************************************/
class UsbControlPipe final {
public:
    /**
     * @brief USB Recipient.
     * 
     * This enum defines the Control Request Recipient as per Table #9-2
     * "Format of Setup Data"
     * of the
     * "Universal Serial Bus Specification"
     * Revision 2.0 from Apr 27, 2000
     */
    typedef enum UsbRecipient_e {
        e_Device        = 0x00,
        e_Interface     = 0x01,
        e_Endpoint      = 0x02,
        e_Other         = 0x03
    } UsbRecipient_t;

    /**
     * @brief USB Request.
     * 
     * This enum defines the Control Request as per Table #9-3
     * "Standard Device Requests"
     * of the
     * "Universal Serial Bus Specification"
     * Revision 2.0 from Apr 27, 2000
     */
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
    /** @brief Reference to USB Control IN Endpoint. */
    UsbCtrlInEndpoint &m_inEndpoint;
    /** @brief Pointer to USB Control OUT Endpoint. */
    UsbCtrlOutEndpoint *m_outEndpoint;

    /**
     * @brief Pointer to the USB Device Object.
     * 
     * Provides access to the USB Device specific operations (e.g. Data Transmission).
     */
    UsbDevice &m_usbDevice;

    /** @brief Pointer to the active USB Configuration.
     * 
     * This points to the currently active USB Configuration. The pointer is \c NULL
     * if no configuration is currently active.
     */
    const UsbConfiguration * m_activeConfiguration;

    void decodeSetupPacket(const ::usb::UsbSetupPacket_t &p_setupPacket);
    void decodeDeviceRequest(const UsbSetupPacket_t &p_setupPacket);
    void decodeInterfaceRequest(const UsbSetupPacket_t &p_setupPacket) const;

public:
    /**
     * @brief Construct a new USB Control Pipe object.
     * 
     * @param p_usbDevice Reference to the USB Device Object.
     * @param p_inEndpoint Reference to the USB Control IN Enpoint.
     */
    UsbControlPipe(UsbDevice &p_usbDevice, UsbCtrlInEndpoint &p_inEndpoint)
      : m_inEndpoint(p_inEndpoint), m_outEndpoint(nullptr), m_usbDevice(p_usbDevice), m_activeConfiguration(nullptr) {
          this->m_usbDevice.registerUsbCtrlPipe(*this);
    }

    ~UsbControlPipe() {
        this->m_usbDevice.unregisterUsbCtrlPipe();
    };

    /**
     * @brief Register a Control OUT Endpoint Callback.
     * 
     * @param p_outEndpoint Reference to the Control OUT Endpoint.
     */
    void registerCtrlOutEndpoint(UsbCtrlOutEndpoint &p_outEndpoint) {
        assert(this->m_outEndpoint == nullptr);
        this->m_outEndpoint = &p_outEndpoint;
    }

    /** @brief Unregister a Control OUT Endpoint Callback. */
    void unregisterCtrlOutEndpoint(void) {
        assert(this->m_outEndpoint != nullptr);
        this->m_outEndpoint = nullptr;
    }

    void    setupStageComplete(const ::usb::UsbSetupPacket_t &p_setupPacket);

    /** @name Interface to UsbInterface. 
     * 
     * These methods form an interface to class UsbInterface.
     */
///@{
    void    setDataStageBuffer(uint32_t * const p_buffer, const size_t p_length) const;
///@}

    /** @name Interface to UsbCtrlOutEndpoint. 
     * 
     * These methods form an interface to class UsbCtrlOutEndpoint.
     */
///@{
    void transferComplete(const size_t p_numBytes) const;
///@}

    /** @name Control IN Write Interface.
     * 
     * These methods form an interface to write to the Control IN Endpoint.
     * 
     * Used by classes UsbDevice and UsbInterface.
     */
///@{
    /**
     * @brief Write Data to the Control IN Endpoint.
     * 
     * Uses the Control IN Endpoint #m_inEndpoint to send data to the USB Host.
     * 
     * @param p_data Pointer to Data to be sent.
     * @param p_length Length of Data to be sent.
     * 
     * \see CtrlInEndpoint::write
     */
    constexpr void write(const uint8_t * const p_data, const size_t p_length) const {
        this->m_inEndpoint.write(p_data, p_length);
    }

    /**
     * @brief Write a USB String Descriptor to the Control IN Endpoint.
     * 
     * Uses the Control IN Endpoint #m_inEndpoint to send a USB String Descriptor to
     * the USB Host.
     * 
     * @param p_string Reference to the USB String Descriptor.
     * @param p_length Amount of Bytes to be sent.
     * 
     * \see CtrlInEndpoint::write
     */
    constexpr void writeString(const ::usb::UsbStringDescriptor &p_string, const size_t p_length) const {
        this->m_inEndpoint.writeString(p_string, p_length);
    }
///@}
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161 */
