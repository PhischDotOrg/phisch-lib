/*-
 * $Copyright$
-*/

#include <usb/UsbCtrlOutEndpoint.hpp>
#include <usb/UsbControlPipe.hpp>
#include <usb/UsbHwOutEndpoint.hpp>
#include <usb/OutEndpointViaSTM32F4.hpp>
#include <assert.h>
#include <stddef.h>

namespace usb {

/***************************************************************************//**
 * @brief Handle "Transfer Complete" (="Data Stage Complete") Situation.
 * 
 * For a Control Endpoint, this marks the end of the Data Stage.
 ******************************************************************************/
template<typename UsbHwCtrlOutEndpointT>
void
UsbCtrlOutEndpointT<UsbHwCtrlOutEndpointT>::transferComplete(const size_t p_numBytes) const {
    assert(this->m_ctrlPipe != NULL);
    this->m_ctrlPipe->transferComplete(p_numBytes);
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
template<typename UsbHwCtrlOutEndpointT>
void
UsbCtrlOutEndpointT<UsbHwCtrlOutEndpointT>::setupComplete(const UsbSetupPacket_t &p_setupPacket) const {
    this->m_ctrlPipe->setupStageComplete(p_setupPacket);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbHwCtrlOutEndpointT>
void
UsbCtrlOutEndpointT<UsbHwCtrlOutEndpointT>::setDataStageBuffer(uint32_t * const p_data, const size_t p_length) const {
    this->m_hwEndpoint.setDataStageBuffer(p_data, p_length);
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

/*******************************************************************************
 *
 ******************************************************************************/
template class ::usb::UsbCtrlOutEndpointT<::usb::stm32f4::CtrlOutEndpointViaSTM32F4>;
