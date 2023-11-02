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
    virtual void ack(void) const = 0;
    virtual void nack(void) const = 0;
    virtual void stall(void) const = 0;
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
    ack(void) const {
        assert(m_hwEndpoint != nullptr);
        m_hwEndpoint->ack();
    }

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
    read(uint8_t * const p_buffer, const size_t p_length) {
        assert((p_length == 0) || (p_buffer != nullptr));

        assert(m_current == nullptr);
        assert(m_current == m_end);

        m_current = p_buffer;
        m_end = m_current + p_length;

        ack();
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

    template<size_t FifoWidth, typename CopyPolicyT, bool p_signalTransferComplete, typename FifoItT>
    void
    handlePacketReceived(const FifoItT p_begin, const FifoItT p_end) {
        const size_t rxBytes = stm32::copy_from_fifo<FifoWidth, CopyPolicyT>(p_begin, p_end, m_current, m_end);

        USB_PRINTF("UsbOutEndpoint::%s(p_begin=%p, p_end=%p) rxBytes = %d\r\n", __func__, p_begin, p_end, rxBytes);

        m_current += rxBytes;
        if (m_current < m_end) {
            ack();
        } else if constexpr (p_signalTransferComplete) {
            /*
             * On STM32F4, the Hardware generates an Interrupt when an OUT Transfer is complete. On STM32F1,
             * this is not possible. Instead, we get an IRQ whenever a packet is received.
             *
             * To handle this difference, we're using the p_signalTransferComplete template parameter. Making
             * it a template parameter allows the difference to be evaluated at compile time instead of at
             * run time.
             */
            handleTransferComplete();
        }
    }

    virtual void handleTransferComplete(void) = 0;
};
/******************************************************************************/

/******************************************************************************/
class UsbMessagePipe;

class UsbCtrlOutEndpoint : public UsbOutEndpoint {
    UsbMessagePipe &                                m_ctrlPipe;
    std::array<uint8_t, sizeof(UsbSetupPacket_t)>   m_setupPacketBuffer {};

protected:
    void
    handleTransferComplete(void) override {
        m_ctrlPipe.notifyReadComplete(m_current == nullptr ? 0 : m_current - m_begin);
        m_current = m_end = nullptr;
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
        ack();
    }

    template<size_t FifoWidth, typename CopyPolicyT, typename FifoItT>
    void
    handleSetupPacketReceived(const FifoItT p_begin, const FifoItT p_end) {
        const size_t numBytes = stm32::copy_from_fifo<FifoWidth, CopyPolicyT>(p_begin, p_end, m_setupPacketBuffer.begin(), m_setupPacketBuffer.end());
        assert(numBytes == sizeof(UsbSetupPacket_t));
        (void) numBytes;
    }

    void
    notifySetupPacketReceived(void) const {
        m_ctrlPipe.notifySetupPacketReceived(* reinterpret_cast<const UsbSetupPacket_t *>(m_setupPacketBuffer.data()));
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
    void handleTransferComplete(void) override { assert(false); /* FIXME Implement Function */ };

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
