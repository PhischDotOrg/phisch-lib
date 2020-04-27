/*-
 * $Copyright$
-*/

#ifndef _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161
#define _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161

#include <usb/UsbControlPipe.hpp>
#include <stdint.h>
namespace usb {

class UsbCtrlInEndpoint;
class UsbOutEndpoint;
class GenericUsbDevice;

/*******************************************************************************
 *
 ******************************************************************************/
class DefaultControlPipe : public UsbControlPipe {
public:
/*******************************************************************************
 *
 ******************************************************************************/
            DefaultControlPipe(UsbCtrlInEndpoint &p_inEndpoint, UsbCtrlOutEndpoint &p_outEndpoint);
    virtual ~DefaultControlPipe();

    void    registerUsbDevice(GenericUsbDevice &p_usbDevice);
    void    unregisterUsbDevice(GenericUsbDevice &p_usbDevice);

    void    setupStageComplete(const UsbSetupPacket_t &p_setupPacket) const;
    void    dataStageReceive(const uint32_t p_rxData);
    void    dataStageComplete(void);

private:
    GenericUsbDevice *m_usbDevice;

    void handleDeviceRequest(const UsbSetupPacket_t &p_setupPacket) const;

    void getDescriptor(const uint16_t p_descriptor, const size_t p_len) const;
    void getDeviceDescriptor(const uint8_t p_descriptorId, const size_t p_len) const;
    void getDeviceQualifierDescriptor(const uint8_t p_descriptorId, const size_t p_len) const;
    void getConfigurationDescriptor(const uint8_t p_descriptorId, const size_t p_len) const;
    void getStringDescriptor(const uint8_t p_descriptorId, const size_t p_len) const;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_DEFAULT_CONTROL_PIPE_HPP_FCED743E_11B7_489D_9A33_8036AB80C161 */
