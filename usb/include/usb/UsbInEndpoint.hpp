/*-
 * $Copyright$
-*/

#ifndef _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510
#define _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510

#include <usb/UsbTypes.hpp>
#include <cassert>
#include <cstddef>

#include <algorithm/fifocopy.hpp>

/******************************************************************************/
namespace usb {
/******************************************************************************/
class UsbInEndpoint;

class UsbHwInEndpoint {
public:
    virtual size_t enableTxPacket(void) const = 0;
    virtual void stall() const = 0;
    virtual void nack() const = 0;

    virtual void registerEndpointCallback(UsbInEndpoint &p_endpointCallout) = 0;
    virtual void unregisterEndpointCallback(void) = 0;

    virtual void setData(unsigned p_dtog) const = 0;
    virtual bool getData(void) const = 0;

    virtual void write(const uint8_t *p_data, const size_t p_length) const = 0;
};

class UsbInEndpointCallback {
public:
    virtual void notifyWriteComplete(size_t p_numBytes) = 0;
};

/******************************************************************************/
class UsbInEndpoint {
protected:
    UsbHwInEndpoint &       m_hwEndpoint;
    UsbInEndpointCallback * m_endpointCallback {};
    const uint8_t *         m_begin {};
    const uint8_t *         m_current {};
    const uint8_t *         m_end {};
    bool                    m_dtog {};

public:
    UsbInEndpoint(UsbInEndpoint &) = delete;

    constexpr UsbInEndpoint(UsbHwInEndpoint &p_hwEndpoint)
      : m_hwEndpoint(p_hwEndpoint) {
          m_hwEndpoint.registerEndpointCallback(*this);
    }

    virtual ~UsbInEndpoint() {
          m_hwEndpoint.unregisterEndpointCallback();
    }

    void registerEndpointCallback(UsbInEndpointCallback &p_endpointCallback) {
        assert(m_endpointCallback == nullptr);
        m_endpointCallback = &p_endpointCallback;
    }

    void unregisterEndpointCallback(void) {
        assert(m_endpointCallback != nullptr);
        m_endpointCallback = nullptr;
    }

    enum Pid_e {
        e_Data0 = 0,
        e_Data1 = 1,
    };

    void setPid(Pid_e p_pid) {
        m_dtog = (p_pid == e_Data1);

        m_hwEndpoint.setData(p_pid);
    }

    /* FIXME USB calls this "NAK", not "NACK" */
    void nack(void) const {
        m_hwEndpoint.nack();
    }

    void stall(void) const {
        m_hwEndpoint.stall();
    }

    void setupWrite(const uint8_t * p_data, size_t p_length) {
        assert(m_current == nullptr);
        assert(m_end == nullptr);

        m_begin = p_data;
        m_current = m_begin;
        m_end = m_begin + p_length;

        m_hwEndpoint.enableTxPacket();
    }

    template<size_t FifoWidth, typename FifoItT>
    size_t
    handlePacketRead(const FifoItT p_begin, const FifoItT p_end) {
        // bool hwDtog = m_hwEndpoint.getData();
        // assert(hwDtog == m_dtog);
        // m_dtog = !m_dtog;

        return stm32::copy_to_fifo<FifoWidth>(m_current, m_end, p_begin, p_end);
    }

    virtual void handlePacketTransmitted(size_t p_txLength) = 0;

    void
    reset(void) {
        m_current = m_end = nullptr;
    }

    void
    write(const uint8_t * const p_data, const size_t p_length) {
        /* TODO Assert is harmful for IRQ Endpoints */
        // assert(m_current == nullptr);
        // assert(m_end == nullptr);

        m_current = p_data;
        m_end = m_current + p_length;

        m_hwEndpoint.write(p_data, p_length);
    }
};
/******************************************************************************/

/******************************************************************************/
class UsbCtrlInEndpoint : public UsbInEndpoint {
public:
    UsbCtrlInEndpoint(UsbCtrlInEndpoint &) = delete;

    UsbCtrlInEndpoint(UsbHwInEndpoint &p_hwEndpoint)
      : UsbInEndpoint(p_hwEndpoint) {

    }

    void
    handlePacketTransmitted(size_t p_txLength) override {
        m_current += p_txLength;

        /* FIXME Write Complete Handling */
        if (m_current < m_end) {
            m_hwEndpoint.enableTxPacket();
        } else {
            assert(m_endpointCallback != nullptr);
            m_endpointCallback->notifyWriteComplete(m_current == nullptr ? 0 : std::min(m_current, m_end) - m_begin);
            m_current = m_end = nullptr;
        }
    }
};
/******************************************************************************/

/******************************************************************************/
class UsbBulkInEndpoint : public UsbInEndpoint {
public:
    UsbBulkInEndpoint(UsbBulkInEndpoint &) = delete;

    constexpr UsbBulkInEndpoint(UsbHwInEndpoint &p_hwEndpoint)
      : UsbInEndpoint(p_hwEndpoint) {
    }

    void
    handlePacketTransmitted(size_t p_txLength) override {
        m_current += p_txLength;

        /* FIXME Write Complete Handling */
        if (m_current < m_end) {
            m_hwEndpoint.enableTxPacket();
        } else {
            assert(m_endpointCallback != nullptr);
            m_endpointCallback->notifyWriteComplete(m_current == nullptr ? 0 : std::min(m_current, m_end) - m_begin);
            m_current = m_end = nullptr;
        }
    }
};
/******************************************************************************/

/******************************************************************************/
class UsbIrqInEndpoint : public UsbInEndpoint {
public:
    UsbIrqInEndpoint(UsbIrqInEndpoint &) = delete;

    constexpr UsbIrqInEndpoint(UsbHwInEndpoint &p_hwEndpoint)
      : UsbInEndpoint(p_hwEndpoint) {

    }

    void handlePacketTransmitted(size_t /* p_txLength */) override {
        /* Nothing to do */
    };
};
/******************************************************************************/

/******************************************************************************/
} /* namespace usb */
/******************************************************************************/

#endif /* _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510 */