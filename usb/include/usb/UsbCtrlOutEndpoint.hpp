/*-
 * $Copyright$
-*/

#ifndef _USB_CTRL_OUT_ENDPOINT_HPP_63105854_B113_4CA9_B8B4_5E6D8464A054
#define _USB_CTRL_OUT_ENDPOINT_HPP_63105854_B113_4CA9_B8B4_5E6D8464A054

#include <usb/UsbTypes.hpp>
#include <usb/UsbOutEndpoint.hpp>

namespace usb {

    class UsbControlPipe;

/*******************************************************************************
 *
 ******************************************************************************/
    namespace stm32f4 {
        class BulkOutEndpointViaSTM32F4;
        class CtrlOutEndpointViaSTM32F4;
    }


/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbHwCtrlOutEndpointT>
class UsbCtrlOutEndpointT {
private:
    UsbHwCtrlOutEndpointT & m_hwEndpoint;
    UsbControlPipe *        m_ctrlPipe;

public:
    UsbCtrlOutEndpointT(UsbHwCtrlOutEndpointT &p_hwEndpoint) : m_hwEndpoint(p_hwEndpoint) {
        m_hwEndpoint.registerEndpointCallback(*this);
    };

    ~UsbCtrlOutEndpointT() {
        m_hwEndpoint.unregisterEndpointCallback(*this);
    }

    void registerCtrlPipe(UsbControlPipe &p_ctrlPipe) {
        assert(this->m_ctrlPipe == nullptr);
        this->m_ctrlPipe = &p_ctrlPipe;
    }

    void unregisterCtrlPipe(UsbControlPipe &p_ctrlPipe) {
        assert(this->m_ctrlPipe == &p_ctrlPipe);
        this->m_ctrlPipe = nullptr;
    }

    void transferComplete(const size_t p_numBytes) const;
    void setupComplete(const UsbSetupPacket_t &p_setupPacket) const;

    void setDataStageBuffer(uint32_t * const p_data, const size_t p_length) const;
};

typedef UsbCtrlOutEndpointT<::usb::stm32f4::CtrlOutEndpointViaSTM32F4> UsbCtrlOutEndpoint;

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_CTRL_OUT_ENDPOINT_HPP_63105854_B113_4CA9_B8B4_5E6D8464A054 */
