/*-
 * $Copyright$
-*/

#include <usb/UsbCtrlOutEndpoint.hpp>
#include <usb/UsbControlPipe.hpp>
#include <assert.h>
#include <stddef.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
UsbCtrlOutEndpoint::UsbCtrlOutEndpoint(UsbHwOutEndpoint &p_hwEndpoint)
  : UsbOutEndpoint(p_hwEndpoint) {

}

/*******************************************************************************
 *
 ******************************************************************************/
UsbCtrlOutEndpoint::~UsbCtrlOutEndpoint() {

}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbCtrlOutEndpoint::registerCtrlPipe(UsbControlPipe &p_ctrlPipe) {
    assert(this->m_ctrlPipe == NULL);
    this->m_ctrlPipe = &p_ctrlPipe;
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbCtrlOutEndpoint::unregisterCtrlPipe(UsbControlPipe &p_ctrlPipe) {
    assert(this->m_ctrlPipe == &p_ctrlPipe);
    this->m_ctrlPipe = NULL;
}


/*******************************************************************************
 *
 ******************************************************************************/
void
UsbCtrlOutEndpoint::rxData(const uint32_t p_rxData) {
    assert(this->m_ctrlPipe != NULL);

    this->m_ctrlPipe->dataStageReceive(p_rxData);
  /* TODO UsbOutEndpoint::rxData() not implemented */
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbCtrlOutEndpoint::transferComplete(void) {
    /* TODO UsbOutEndpoint::transferComplete() not implemented */
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbCtrlOutEndpoint::setupComplete(const void *p_setupData, const size_t p_length) {
    assert(p_setupData == &this->m_setupPacket);
    assert(p_length == sizeof(this->m_setupPacket));

    this->m_ctrlPipe->setupStageComplete(this->m_setupPacket);
}

/*******************************************************************************
 *
 ******************************************************************************/
void *
UsbCtrlOutEndpoint::getSetupPacketBuffer(size_t * const p_length) {
    assert(p_length != NULL);

    *p_length = sizeof(this->m_setupPacket);

    return &this->m_setupPacket;
}

/*******************************************************************************
 *
 ******************************************************************************/

} /* namespace usb */
