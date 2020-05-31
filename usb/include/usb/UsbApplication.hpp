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

/*******************************************************************************
 *
 ******************************************************************************/
class UsbBulkOutApplication {
public:
    virtual void        transferComplete(const size_t p_numBytes) = 0;

    virtual uint32_t *  getBufferAddress(void) = 0;
    virtual size_t      getBufferLength(void) const = 0;
};

/*******************************************************************************
 *
 ******************************************************************************/
template<size_t nBufferSz>
class UsbBulkOutLoopbackApplicationT : public UsbBulkOutApplication {
private:
    uint8_t                     m_dataBuffer[nBufferSz] __attribute__((aligned(4)));
    const UsbBulkInEndpoint &   m_inEndpoint;

public:
    UsbBulkOutLoopbackApplicationT(const UsbBulkInEndpoint &p_inEndpoint)
      : m_inEndpoint(p_inEndpoint) {

    };

    ~UsbBulkOutLoopbackApplicationT() {

    };

    void transferComplete(const size_t p_numBytes) override {
        USB_PRINTF("UsbBulkOutLoopbackApplicationT<%d>::%s(p_numBytes=%d): ", nBufferSz, __func__, p_numBytes);

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

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UartAccess>
class UsbUartApplicationT : public UsbBulkOutApplication {
private:
    const UartAccess &  m_uartAccess;
    union {
        uint8_t     m_u8[sizeof(uint32_t)];
        uint32_t    m_u32;    
    } m_dataBuffer __attribute__((aligned(4)));
    static_assert(sizeof(m_dataBuffer) == 4);

public:
    UsbUartApplicationT(const UartAccess &p_uartAccess)
      : m_uartAccess(p_uartAccess) {

    };

    ~UsbUartApplicationT() {

    };

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
