/*-
 * $Copyright$
-*/

#ifndef __USB_CONTROL_PIPE_HPP_D85427DC_5445_4D98_83FB_E26E85331942
#define __USB_CONTROL_PIPE_HPP_D85427DC_5445_4D98_83FB_E26E85331942

#include <usb/UsbCtrlOutEndpoint.hpp>
#include <stdint.h>

namespace usb {

class UsbCtrlInEndpoint;

/*******************************************************************************
 *
 ******************************************************************************/
class UsbControlPipe {
public:

    typedef enum UsbRecipient_e {
        e_Device        = 0x00,
        e_Interface     = 0x01,
        e_Endpoint      = 0x02,
        e_Other         = 0x03
    } UsbRecipient_t;

    typedef enum UsbRequest_e {
        e_GetStatus         = 0x00,
        e_ClearFeature      = 0x01,
        e_SetFeature        = 0x03,
        e_SetAddress        = 0x05,
        e_GetDescriptor     = 0x06,
        e_SetDescriptor     = 0x07,
        e_GetConfiguration  = 0x08,
        e_SetConfiguration  = 0x09,
        e_GetInterface      = 0x0A,
        e_SetInterface      = 0x0B,
        e_SyncFrame         = 0x0C
    } UsbRequest_t;

protected:
    UsbCtrlInEndpoint &m_inEndpoint;
    UsbCtrlOutEndpoint &m_outEndpoint;

public:
            UsbControlPipe(UsbCtrlInEndpoint &p_inEndpoint, UsbCtrlOutEndpoint &p_outEndpoint);
    virtual ~UsbControlPipe();

    virtual void setupStageComplete(const UsbSetupPacket_t &p_setupPacket) const = 0;
    virtual void dataStageReceive(const uint32_t p_rxData) = 0;
    virtual void dataStageComplete(void) = 0;
};

} /* namespace usb */

#endif /* __USB_CONTROL_PIPE_HPP_D85427DC_5445_4D98_83FB_E26E85331942 */
