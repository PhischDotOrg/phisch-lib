/*-
 * $Copyright$
-*/

#ifndef _USB_CTRL_OUT_ENDPOINT_HPP_63105854_B113_4CA9_B8B4_5E6D8464A054
#define _USB_CTRL_OUT_ENDPOINT_HPP_63105854_B113_4CA9_B8B4_5E6D8464A054

#include <usb/UsbControlPipe.hpp>
#include <usb/UsbTypes.hpp>
#include <assert.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
class UsbCtrlOutEndpoint {
protected:
    UsbControlPipe &    m_ctrlPipe;

public:
    constexpr UsbCtrlOutEndpoint(UsbControlPipe &p_ctrlPipe) : m_ctrlPipe(p_ctrlPipe) {
        m_ctrlPipe.registerCtrlOutEndpoint(*this);
    }

    virtual ~UsbCtrlOutEndpoint() {
        m_ctrlPipe.unregisterCtrlOutEndpoint();
    }

    /***************************************************************************//**
    * @brief Handle "Transfer Complete" (="Data Stage Complete") Situation.
    * 
    * For a Control Endpoint, this marks the end of the Data Stage.
    ******************************************************************************/
    void transferComplete(const size_t p_numBytes) const {
        this->m_ctrlPipe.transferComplete(p_numBytes);
    }

    /***************************************************************************//**
    * @brief Handle "Setup Complete" Situation.
    * 
    * This method is called by the Hardware-dependent layer once the Control
    * Endpoint has received a Setup Packet.
    * 
    * The method notifies the Default Control Pipe about the availablilty of 
    * the Setup Packet and thus triggers decoding the Packet.
    * 
    * This marks the end of the Setup Stage.
    * 
    * \p p_setupPacket Setup Packet as received by the Hardware-dependant layer.
    * 
    * \see ::usb::stm32f4::CtrlOutEndpointViaSTM32F4::setupCompleteDeviceCallback
    ******************************************************************************/
    void setupComplete(const UsbSetupPacket_t &p_setupPacket) const {
        this->m_ctrlPipe.setupStageComplete(p_setupPacket);
    }

    virtual void setDataStageBuffer(uint32_t * const p_data, const size_t p_length) const = 0;
};

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbHwCtrlOutEndpointT>
class UsbCtrlOutEndpointT : public UsbCtrlOutEndpoint {
private:
    UsbHwCtrlOutEndpointT * m_hwEndpoint;

public:
    constexpr UsbCtrlOutEndpointT(UsbControlPipe &p_ctrlPipe) : UsbCtrlOutEndpoint(p_ctrlPipe) {

    };

    ~UsbCtrlOutEndpointT() override {

    }

    void registerHwEndpoint(UsbHwCtrlOutEndpointT &p_hwEndpoint) {
        assert(this->m_hwEndpoint == nullptr);
        this->m_hwEndpoint = &p_hwEndpoint;
    }

    void unregisterHwEndpoint() {
        assert(this->m_hwEndpoint != nullptr);
        this->m_hwEndpoint = nullptr;
    }

    void setDataStageBuffer(uint32_t * const p_data, const size_t p_length) const override {
        assert(this->m_hwEndpoint != nullptr);
        this->m_hwEndpoint->setDataStageBuffer(p_data, p_length);
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_CTRL_OUT_ENDPOINT_HPP_63105854_B113_4CA9_B8B4_5E6D8464A054 */
