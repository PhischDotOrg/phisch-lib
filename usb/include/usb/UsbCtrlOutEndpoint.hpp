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
class UsbCtrlOutEndpoint : public UsbOutEndpoint {
private:
    UsbControlPipe *    m_ctrlPipe;
    UsbSetupPacket_t    m_setupPacket;

public:
            UsbCtrlOutEndpoint(UsbHwOutEndpoint &p_hwEndpoint);
    virtual ~UsbCtrlOutEndpoint();

    void registerCtrlPipe(UsbControlPipe &p_ctrlPipe);
    void unregisterCtrlPipe(UsbControlPipe &p_ctrlPipe);

    virtual void rxData(const uint32_t p_rxData);
    virtual void transferComplete(void);
    virtual void setupComplete(const void * p_setupData, const size_t p_length);

    virtual void *getSetupPacketBuffer(size_t * const p_length);
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_CTRL_OUT_ENDPOINT_HPP_63105854_B113_4CA9_B8B4_5E6D8464A054 */
