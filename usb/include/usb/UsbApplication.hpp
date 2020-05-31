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
    virtual void transferComplete(const size_t p_numBytes) = 0;

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

} /* namespace usb */

#endif /* __USB_APPLICATION_HPP_2F432858_61CD_42BC_9ED8_09B633CA4237 */
