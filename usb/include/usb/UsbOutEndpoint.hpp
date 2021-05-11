/*-
 * $Copyright$
-*/

#ifndef _USB_CTRL_OUT_ENDPOINT_HPP_63105854_B113_4CA9_B8B4_5E6D8464A054
#define _USB_CTRL_OUT_ENDPOINT_HPP_63105854_B113_4CA9_B8B4_5E6D8464A054

#include <usb/UsbControlPipe.hpp>
#include <usb/UsbApplication.hpp>
#include <usb/UsbTypes.hpp>

#include <algorithm/fifocopy.hpp>
#include <cassert>

#include <vector>

namespace usb {

class UsbOutEndpoint;

class UsbHwOutEndpoint {
public:
    virtual void setData(unsigned p_dtog) const = 0;
    virtual bool getData(void) const = 0;
    virtual void nack(void) const = 0;
    virtual void stall(void) const = 0;
    virtual void enableRxPacket(void) const = 0;
};


/******************************************************************************/
class UsbOutEndpoint {
    const UsbHwOutEndpoint *m_hwEndpoint {};
protected:
    uint8_t *               m_begin {};
    uint8_t *               m_current {};
    uint8_t *               m_end {};

protected:
    void
    reset(void) {
        /* TODO Should we notify callback about aborted / completed transfer? */
        m_current = m_end = nullptr;
    }

    void
    enableRxPacket(void) const {
        assert(m_hwEndpoint != nullptr);
        m_hwEndpoint->enableRxPacket();
    }

    virtual void handlePacketReceived(size_t p_length) = 0;

public:
    enum Pid_e {
        e_Data0 = 0,
        e_Data1 = 1
    };

    void setPid(Pid_e p_pid) {
        assert(m_hwEndpoint != nullptr);
        m_hwEndpoint->setData(p_pid);
    }

    void stall(void) const {
        assert(m_hwEndpoint != nullptr);
        m_hwEndpoint->stall();
    }

    void nack(void) const {
        assert(m_hwEndpoint != nullptr);
        m_hwEndpoint->nack();
    }

    void
    setupRead(uint8_t * const p_buffer, const size_t p_length) {
        assert((p_length == 0) || (p_buffer != nullptr));

        assert(m_current == nullptr);
        assert(m_current == m_end);

        m_current = p_buffer;
        m_end = m_current + p_length;

        enableRxPacket();
    }

    void
    registerHwOutEndpoint(const UsbHwOutEndpoint &p_hwEndpoint) {
        assert(this->m_hwEndpoint == nullptr);
        this->m_hwEndpoint = &p_hwEndpoint;
    }

    void
    unregisterHwOutEndpoint(void) {
        this->m_hwEndpoint = nullptr;
    }

    template<size_t FifoWidth, typename FifoItT>
    size_t
    handlePacketReceived(const FifoItT p_begin, const FifoItT p_end) {
        const size_t rxBytes = stm32::copy_from_fifo<FifoWidth>(p_begin, p_end, m_current, m_end);

        handlePacketReceived(rxBytes);

        return rxBytes;
    }
};
/******************************************************************************/

/******************************************************************************/
class UsbMessagePipe;

class UsbCtrlOutEndpoint : public UsbOutEndpoint {
    UsbMessagePipe &                                m_ctrlPipe;
    std::array<uint8_t, sizeof(UsbSetupPacket_t)>   m_setupPacketBuffer {};
    bool                                            m_rxZeroLengthPacket {};

protected:
    void
    handlePacketReceived(size_t p_length) override {
        m_current += p_length;
        if (m_current >= m_end) {
            const size_t totalRxLength = m_end - m_begin;
            if (totalRxLength && (totalRxLength % 64) == 0) { /* FIXME Remove hard-coded number */
                if (!m_rxZeroLengthPacket) {
                    m_rxZeroLengthPacket = true;
                    enableRxPacket();
                } else {
                    m_ctrlPipe.notifyReadComplete(m_current == nullptr ? 0 : m_current - m_begin);
                    m_current = m_end = nullptr;
                    m_rxZeroLengthPacket = false;            
                }
            } else {
                m_ctrlPipe.notifyReadComplete(m_current == nullptr ? 0 : m_current - m_begin);
                m_current = m_end = nullptr;
            }
        } else {
            enableRxPacket();
        }
    }

public:
    void
    reset(void) {
        UsbOutEndpoint::reset();
        m_ctrlPipe.reset();
        m_setupPacketBuffer.fill(0);
    }

    void
    enableSetupPacket(void) {
        enableRxPacket();
    }

    template<size_t FifoWidth, typename FifoItT>
    void
    handleSetupPacketReceived(const FifoItT p_begin, const FifoItT p_end) {
        const size_t numBytes = stm32::copy_from_fifo<FifoWidth>(p_begin, p_end, m_setupPacketBuffer.begin(), m_setupPacketBuffer.end());
        assert(numBytes == sizeof(UsbSetupPacket_t));
        (void) numBytes;

        m_ctrlPipe.notifySetupPacketReceived(* reinterpret_cast<const UsbSetupPacket_t * const>(m_setupPacketBuffer.data()));
    }

    UsbCtrlOutEndpoint(UsbControlPipe &p_ctrlPipe) : m_ctrlPipe(p_ctrlPipe) {
        m_ctrlPipe.registerCtrlOutEndpoint(*this);
    }

    virtual ~UsbCtrlOutEndpoint() {
        m_ctrlPipe.unregisterCtrlOutEndpoint();
    }
};
/******************************************************************************/

/******************************************************************************/
class UsbBulkOutEndpoint : public UsbOutEndpoint {
private:
    UsbBulkOutApplication &     m_usbApplication;

protected:
    void handlePacketReceived(size_t /* p_length */) { /* FIXME Implement Function */ };

public:
    UsbBulkOutEndpoint(UsbBulkOutApplication &p_usbApplication)
      : m_usbApplication(p_usbApplication) {
          (void) m_usbApplication;
    }
};
/******************************************************************************/

/******************************************************************************/
} /* namespace usb */
/******************************************************************************/

#endif /* _USB_CTRL_OUT_ENDPOINT_HPP_63105854_B113_4CA9_B8B4_5E6D8464A054 */
