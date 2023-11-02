/*-
 * $Copyright$
-*/

#ifndef __USB_APPLICATION_HPP_2F432858_61CD_42BC_9ED8_09B633CA4237
#define __USB_APPLICATION_HPP_2F432858_61CD_42BC_9ED8_09B633CA4237

#include <usb/UsbInEndpoint.hpp>
#include <usb/UsbOutEndpoint.hpp>
#include <algorithm>
#include <cstddef>

/*******************************************************************************
 *
 ******************************************************************************/

namespace usb {

/***************************************************************************//**
 * @brief Interface for an Application that reacts to Bulk OUT transfers.
 * 
 * The UsbOutEndpoint class uses this interface to call into when a Bulk OUT
 * Transfer is complete.
 * 
 * \see UsbBulkOutEndpointT::transferComplete
 ******************************************************************************/
class UsbBulkOutApplication {
public:
    /**  @brief Destructor. */
    virtual ~UsbBulkOutApplication() = default;

    /**
     * @brief Callout for _Transfer Complete_ Events.
     * 
     * Called by UsbBulkOutEndpointT::transferComplete when a Bulk OUT transfer
     * is complete.
     * 
     * @param p_numBytes Number of Bytes transferred by the Bulk OUT request.
     */
    virtual void        transferComplete(const size_t p_numBytes) = 0;
};

/***************************************************************************//**
 * @brief Bulk OUT Application that loops back received data.
 * 
 * This sample Bulk OUT Application receives Bulk OUT data and loops it back to
 * a Bulk IN endpoint.
 ******************************************************************************/
template<size_t nBufferSz>
class UsbBulkOutLoopbackApplicationT : public UsbBulkOutApplication {
private:
    /** @brief RAM Buffer to receive Bulk OUT Data */
    uint8_t                     m_dataBuffer[nBufferSz] __attribute__((aligned(4)));
    /** @brief IN Endpoint through which the Data is sent back to the USB Host. */
    UsbBulkInEndpoint &         m_inEndpoint;

public:
    /**
     * @brief Constructor.
     * 
     * @param p_inEndpoint IN Endpoint through which the Data is sent back to the USB Host.
     */
    UsbBulkOutLoopbackApplicationT(UsbBulkInEndpoint &p_inEndpoint)
      : m_inEndpoint(p_inEndpoint) {

    };

    /**  @brief Destructor. */
    virtual ~UsbBulkOutLoopbackApplicationT() = default;

    void transferComplete(const size_t p_numBytes) override {
        USB_PRINTF("UsbBulkOutLoopbackApplicationT<%d>::%s(p_numBytes=%d)\r\n", nBufferSz, __func__, p_numBytes);

        assert(p_numBytes <= sizeof(this->m_dataBuffer));
        this->m_inEndpoint.write(this->m_dataBuffer, std::min(p_numBytes, sizeof(this->m_dataBuffer)));
    }
};

/***************************************************************************//**
 * @brief Bulk OUT Application that sends received data to a UART.
 * 
 * This sample Bulk OUT Application receives Bulk OUT data and sends it to a
 * UART Interface.
 ******************************************************************************/
template<typename UartAccess>
class UsbUartApplicationT : public UsbBulkOutApplication {
private:
    /** @brief UART Object through which received Data is sent out. */ 
    const UartAccess &  m_uartAccess;

    /** @brief RAM Buffer to receive Bulk OUT Data */
    union {
        uint8_t     m_u8[sizeof(uint32_t)];
        uint32_t    m_u32;    
    } m_dataBuffer __attribute__((aligned(4)));
    static_assert(sizeof(m_dataBuffer) == 4);

public:
    /**
     * @brief Constructor.
     * 
     * @param p_uartAccess UART Object  through which received Data is sent out.
     */
    UsbUartApplicationT(const UartAccess &p_uartAccess)
      : m_uartAccess(p_uartAccess) {

    };

    virtual ~UsbUartApplicationT() = default;

    void transferComplete(const size_t p_numBytes) override {
        USB_PRINTF("UsbUartApplicationT::%s(p_numBytes=%d): ", __func__, p_numBytes);

        assert(p_numBytes <= sizeof(this->m_dataBuffer));

        for (unsigned i = 0; i < std::min(p_numBytes, sizeof(this->m_dataBuffer)); i++) {
            m_uartAccess.putf(this->m_dataBuffer.m_u8[i]);
        }
    }
};

/***************************************************************************//**
 * @brief USB HID Application that simulates a USB Mouse.
 *
 ******************************************************************************/
template<
    typename UsbConfigurationT
>
class UsbMouseApplicationT {
    typedef struct HidData_s {
        union {
            uint8_t     m_data;
            struct {
                uint8_t m_leftButton : 1;
                uint8_t m_middleButton : 1;
                uint8_t m_rightButton : 1;
                uint8_t m_reserved : 5;
            } m_bitfield;
        }           m_button;
        int8_t      m_xAxis;
        int8_t      m_yAxis;
    } __attribute__((packed)) HidData_t;
    static_assert(sizeof(HidData_t) == 3);

private:
    const UsbConfigurationT &   m_usbConfiguration;
    HidData_t                   m_hidData;

public:
    constexpr UsbMouseApplicationT(const UsbConfigurationT &p_usbConfiguration)
      : m_usbConfiguration(p_usbConfiguration), m_hidData {} {

    }

    enum Button_e {
        e_LeftButton    = (1 << 0),
        e_MiddleButton  = (1 << 1),
        e_RightButton   = (1 << 2) 
    };

    void
    setButton(Button_e p_button, bool p_pressed) {
        switch (p_button) {
        case e_LeftButton:
            this->m_hidData.m_button.m_bitfield.m_leftButton = p_pressed;
            break;
        case e_MiddleButton:
            this->m_hidData.m_button.m_bitfield.m_middleButton = p_pressed;
            break;
        case e_RightButton:
            this->m_hidData.m_button.m_bitfield.m_rightButton = p_pressed;
            break;
        }
    }

    void
    setXAxis(int8_t m_pos) {
        this->m_hidData.m_xAxis = m_pos;
    }

    void
    setYAxis(int8_t m_pos) {
        this->m_hidData.m_yAxis = m_pos;
    }

    void
    updateHost(void) const {
        m_usbConfiguration.writeIrq(reinterpret_cast<const uint8_t *>(&this->m_hidData), sizeof(HidData_t));
    }
};

} /* namespace usb */

#endif /* __USB_APPLICATION_HPP_2F432858_61CD_42BC_9ED8_09B633CA4237 */
