/*-
 * $Copyright$
-*/

#include <usb/CtrlOutEndpoint.hpp>
#include <usb/UsbControlPipe.hpp>
#include <usb/UsbHwCtrlOutEndpoint.hpp>
#include <assert.h>
#include <stddef.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
CtrlOutEndpoint::CtrlOutEndpoint(UsbHwCtrlOutEndpoint &p_hwEndpoint)
  : m_hwEndpoint(p_hwEndpoint) {
      this->m_hwEndpoint.registerEndpointCallback(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
CtrlOutEndpoint::~CtrlOutEndpoint() {
    this->m_hwEndpoint.unregisterEndpointCallback(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
CtrlOutEndpoint::registerCtrlPipe(UsbControlPipe &p_ctrlPipe) {
    assert(this->m_ctrlPipe == NULL);
    this->m_ctrlPipe = &p_ctrlPipe;
}

/*******************************************************************************
 *
 ******************************************************************************/
void
CtrlOutEndpoint::unregisterCtrlPipe(UsbControlPipe &p_ctrlPipe) {
    assert(this->m_ctrlPipe == &p_ctrlPipe);
    this->m_ctrlPipe = NULL;
}

/*******************************************************************************
 *
 ******************************************************************************/
void
CtrlOutEndpoint::rxData(const uint32_t p_rxData) {
    assert(this->m_ctrlPipe != NULL);

    this->m_ctrlPipe->dataStageReceive(p_rxData);
  /* TODO CtrlOutEndpoint::rxData() not implemented */
}

/*******************************************************************************
 *
 ******************************************************************************/
void
CtrlOutEndpoint::transferComplete(void) {
    /* TODO CtrlOutEndpoint::transferComplete() not implemented */
}

/*******************************************************************************
 *
 ******************************************************************************/
void
CtrlOutEndpoint::setupComplete(const void *p_setupData, const size_t p_length) {
    assert(p_setupData == &this->m_setupPacket);
    assert(p_length == sizeof(this->m_setupPacket));

    this->m_ctrlPipe->setupStageComplete(this->m_setupPacket);
}

/*******************************************************************************
 *
 ******************************************************************************/
void *
CtrlOutEndpoint::getSetupPacketBuffer(size_t * const p_length) {
    assert(p_length != NULL);

    *p_length = sizeof(this->m_setupPacket);

    return &this->m_setupPacket;
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
