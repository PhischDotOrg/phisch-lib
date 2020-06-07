/*-
 * $Copyright$
-*/

#ifndef __USB_APPLICATION_HPP_2F432858_61CD_42BC_9ED8_09B633CA4237
#define __USB_APPLICATION_HPP_2F432858_61CD_42BC_9ED8_09B633CA4237

#include <usb/UsbInEndpoint.hpp>
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

    /**
     * @brief Obtain the Bulk OUT Buffer Address.
     * 
     * The UsbBulkOutEndpointT class uses this interface to register the RAM
     * Buffer that receives the Bulk OUT Data with the hardware-specific
     * layer.
     * 
     * \see UsbBulkOutEndpointT::registerHwEndpoint
     * 
     * @return uint32_t* Pointer to RAM Buffer that holds the Bulk OUT Data.
     */
    virtual uint32_t *  getBufferAddress(void) = 0;

    /**
     * @brief Obtain the Bulk OUT Buffer Length.
     * 
     * The UsbBulkOutEndpointT class uses this interface to register the RAM
     * Buffer that receives the Bulk OUT Data with the hardware-specific
     * layer.
     * 
     * \see UsbBulkOutEndpointT::registerHwEndpoint
     * 
     * @return size_t Length of the RAM Buffer that holdsd the Bulk OUT Data.
     */
    virtual size_t      getBufferLength(void) const = 0;
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
    const UsbBulkInEndpoint &   m_inEndpoint;

public:
    /**
     * @brief Constructor.
     * 
     * @param p_inEndpoint IN Endpoint through which the Data is sent back to the USB Host.
     */
    UsbBulkOutLoopbackApplicationT(const UsbBulkInEndpoint &p_inEndpoint)
      : m_inEndpoint(p_inEndpoint) {

    };

    /**  @brief Destructor. */
    virtual ~UsbBulkOutLoopbackApplicationT() = default;

    void transferComplete(const size_t p_numBytes) override {
        USB_PRINTF("UsbBulkOutLoopbackApplicationT<%d>::%s(p_numBytes=%d)\r\n", nBufferSz, __func__, p_numBytes);

        this->m_inEndpoint.write(this->m_dataBuffer, std::min(p_numBytes, sizeof(this->m_dataBuffer)));
    }

    uint32_t *
    getBufferAddress(void) override {
        return reinterpret_cast<uint32_t *>(this->m_dataBuffer);
    }

    size_t
    getBufferLength(void) const override {
        return sizeof(this->m_dataBuffer);
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

        assert(p_numBytes < sizeof(this->m_dataBuffer));

        for (unsigned i = 0; i < std::min(p_numBytes, sizeof(this->m_dataBuffer)); i++) {
            m_uartAccess.putf(this->m_dataBuffer.m_u8[i]);
        }
    }

    uint32_t *
    getBufferAddress(void) override {
        return &this->m_dataBuffer.m_u32;
    }

    size_t
    getBufferLength(void) const override {
        return sizeof(this->m_dataBuffer);
    }
};

} /* namespace usb */

#endif /* __USB_APPLICATION_HPP_2F432858_61CD_42BC_9ED8_09B633CA4237 */
