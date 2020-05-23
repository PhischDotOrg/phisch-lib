/*-
 * $Copyright$
-*/

#ifndef _USB_OUT_ENDPOINT_HPP_CA239521_AD89_43DD_9F28_FCCA4EF81EDC
#define _USB_OUT_ENDPOINT_HPP_CA239521_AD89_43DD_9F28_FCCA4EF81EDC

#include <usb/UsbTypes.hpp>
#include <usb/UsbHwOutEndpoint.hpp>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
class UsbOutEndpoint {
public:
    typedef struct DataBuffer_s {
        uint32_t *  m_buffer;
        size_t      m_numWords;
    } DataBuffer_t;

protected:
    size_t              m_transmitLength;

public:
    constexpr UsbOutEndpoint(void) : m_transmitLength(0) {
        /* Nothing to do. */
    }

    virtual ~UsbOutEndpoint() {};

    void packetReceived(const size_t p_numBytes) {
        this->m_transmitLength += p_numBytes;
    };

    void transferComplete(void) {
        this->transferComplete(this->m_transmitLength);

        this->m_transmitLength = 0;
    }

    virtual void transferComplete(const size_t p_numBytes) const = 0;
    virtual const DataBuffer_t &getDataBuffer(void) const = 0;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_OUT_ENDPOINT_HPP_CA239521_AD89_43DD_9F28_FCCA4EF81EDC */
