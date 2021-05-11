/*-
 * $Copyright$
-*/

#ifndef _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161
#define _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161

#include <usb/UsbTypes.hpp>
#include <stdint.h>

#include <usb/UsbDevice.hpp>
#include <usb/UsbInEndpoint.hpp>

extern "C" void update_dtog_pins(void);

/******************************************************************************/
namespace usb {
/******************************************************************************/

class UsbConfiguration;
class UsbOutEndpoint;
class UsbCtrlOutEndpoint;

class UsbStreamPipe {
    /** @brief Pointer to USB OUT Endpoint. */
    UsbOutEndpoint *m_outEndpoint;
public:
    UsbStreamPipe() : m_outEndpoint(nullptr) { }

    void registerOutEndpoint(UsbOutEndpoint &p_outEndpoint) {
        assert(this->m_outEndpoint == nullptr);
        this->m_outEndpoint = &p_outEndpoint;
    }

    void unregisterOutEndpoint(void) {
        assert(this->m_outEndpoint != nullptr);
        this->m_outEndpoint = nullptr;
    }

    virtual void notifyReadComplete(size_t p_numBytes) = 0;
};

class UsbMessagePipe : public UsbStreamPipe {
protected:
    /** @brief Pointer to USB Control OUT Endpoint. */
    UsbCtrlOutEndpoint *m_outEndpoint;

public:
    UsbMessagePipe() : m_outEndpoint(nullptr) { }

    void registerCtrlOutEndpoint(UsbCtrlOutEndpoint &p_outEndpoint) {
        assert(this->m_outEndpoint == nullptr);
        this->m_outEndpoint = &p_outEndpoint;
    }

    void unregisterCtrlOutEndpoint(void) {
        assert(this->m_outEndpoint != nullptr);
        this->m_outEndpoint = nullptr;
    }

    virtual void notifySetupPacketReceived(const ::usb::UsbSetupPacket_t &p_setupPacket) = 0;

    virtual void reset(void) = 0;
};

/***************************************************************************//**
 * @brief USB Default Control Pipe Handler.
 * 
 * This class handles Default Control Requests as specified by the USB 2.0
 * Standard.
 * 
 * Declared as \c final because the destructor is not declared \c virtual .
 ******************************************************************************/
class UsbControlPipe final : public UsbMessagePipe, public UsbInEndpointCallback {
    class State {
    public:
        enum class State_e {
            e_Undefined     = 0,
            e_Idle          = 1,
            e_SetupReceived = 2,
            e_DataIn        = 3,
            e_StatusOut     = 4,
            e_DataOut       = 5,
            e_StatusIn      = 6,
            e_Error         = 7
        };

        inline constexpr bool operator==(const State &o){ return m_currentState == o.m_currentState; }
        inline constexpr bool operator!=(const State &o){ return !(*this == o); }

        /* Setter Operation */
        inline /* constexpr */ void operator=(const State_e &p_newState) {
            for (unsigned idx = 0; idx < 3; idx++) {
                PHISCH_SETPIN(1 + idx, (static_cast<uint8_t>(p_newState) & (1 << idx)) != 0);
            }

            // USB_PRINTF("UsbControlPipe: FSM [%s] -> [%s]\r\n",
            //   to_cstr(m_currentState), to_cstr(p_newState));

            m_currentState = p_newState;
        }

        /* Getter Operation */
        explicit operator State_e (void) {
            return m_currentState;
        };

    private:
        State_e m_currentState;

        static constexpr std::array<const char * const, 8> m_stateStr {
            "e_Undefined",
            "e_Idle",
            "e_SetupReceived",
            "e_DataIn",
            "e_StatusOut",
            "e_DataOut",
            "e_StatusIn",
            "e_Error"
        };

        static constexpr const char * to_cstr(const State_e &p_state) {
            return m_stateStr[static_cast<unsigned>(p_state)];
        }
    };

    using State_e = State::State_e;

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
    enum class UsbRecipient_e : uint8_t {
        e_Device        = 0x00,
        e_Interface     = 0x01,
        e_Endpoint      = 0x02,
        e_Other         = 0x03
    };
    using UsbRecipient_t = enum UsbRecipient_e;

    /**
     * @brief USB Request.
     * 
     * This enum defines the Control Request as per Table #9-3
     * "Standard Device Requests"
     * of the
     * "Universal Serial Bus Specification"
     * Revision 2.0 from Apr 27, 2000
     */
    enum class UsbRequest_e : uint8_t {
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
    };
    using UsbRequest_t = enum UsbRequest_e;

    enum class Status_e {
        e_Ok,
        e_Error
    };

private:
    /** @brief Reference to USB Control IN Endpoint. */
    UsbCtrlInEndpoint &m_inEndpoint;

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

    State m_state;

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
      : m_inEndpoint(p_inEndpoint), m_usbDevice(p_usbDevice), m_activeConfiguration(nullptr) {
          m_usbDevice.registerUsbCtrlPipe(*this);
          m_inEndpoint.registerEndpointCallback(*this);
    }

    ~UsbControlPipe() {
        m_inEndpoint.unregisterEndpointCallback();
        m_usbDevice.unregisterUsbCtrlPipe();
    };

    void notifyWriteComplete(size_t p_numBytes) override;
    void notifyReadComplete(size_t p_numBytes) override;
    void notifySetupPacketReceived(const ::usb::UsbSetupPacket_t &p_setupPacket) override;

    void idle(void);
    void statusOut(Status_e p_status);
    template <bool isSync = false> void statusIn(Status_e p_status);
    void dataOut(uint8_t * p_data, size_t p_length);
    void dataIn(const uint8_t * p_data, size_t p_length);
    void error(void);

    void start(void);
    void stop(void);

    void reset(void) override {
        idle();
    }
};

/******************************************************************************/
} /* namespace usb */
/******************************************************************************/

#endif /* _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161 */
