/*-
 * $Copyright$
-*/

#include <usb/UsbDevice.hpp>
#include <usb/UsbControlPipe.hpp>
#include <usb/UsbHwDevice.hpp>
#include <usb/UsbConfiguration.hpp>

#include <assert.h>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
UsbDevice::UsbDevice(UsbHwDevice &p_hwDevice, UsbControlPipe &p_ctrlPipe,
  UsbConfiguration &p_configuration,
  const ::usb::UsbStringDescriptors_t &p_stringDescriptors)
  : m_hwDevice(p_hwDevice),
    m_ctrlPipe(p_ctrlPipe),
    m_maxConfigurations(1),
    m_deviceDescriptor {
        sizeof(m_deviceDescriptor),                                     /* m_bLength */
        ::usb::UsbDescriptorTypeId_t::e_Device,                         /* m_bDescriptorType */
        { 0x00, 0x02 },                                                 /* m_bLength */
        ::usb::UsbInterfaceClass_e::e_UsbInterface_Misc_EFh,            /* m_bDeviceClass */
        0x02,                                                           /* m_bDeviceSubClass */
        0x01,                                                           /* m_bDeviceProtocol */
        static_cast<uint8_t>(p_hwDevice.getDeviceSpeed() == ::usb::UsbHwDevice::DeviceSpeed_e::e_UsbFullSpeed ? 64 : 0), /* m_bMaxPacketSize0 */
        { 0xad, 0xde },                                                 /* m_idVendor */
        { 0xef, 0xbe },                                                 /* m_idProduct */
        { 0xfe, 0xca },                                                 /* m_bcdDevice */
        ::usb::UsbStringDescriptorId_t::e_StrDesc_Manufacturer,         /* e_iManufacturer */
        ::usb::UsbStringDescriptorId_t::e_StrDesc_Product,              /* e_iProduct */
        ::usb::UsbStringDescriptorId_t::e_StrDesc_SerialNumber,         /* e_iSerialNumber */
        m_maxConfigurations                                             /* e_bNumConfigurations */
    },
    m_deviceQualifierDescriptor(m_deviceDescriptor),
    m_configuration(p_configuration),
    m_stringDescriptors(p_stringDescriptors),
    m_deviceStatus { 1 }
{
  m_ctrlPipe.registerUsbDevice(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
UsbDevice::~UsbDevice() {
  m_ctrlPipe.unregisterUsbDevice(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbDevice::setAddress(const uint8_t p_address) const {
  this->m_hwDevice.setAddress(p_address);
}

/*******************************************************************************
 *
 ******************************************************************************/
const ::usb::UsbDeviceStatus_t &    
UsbDevice::getStatus(void) const {
    return this->m_deviceStatus;
}

/*******************************************************************************
 *:
 ******************************************************************************/
void
UsbDevice::setConfiguration(const uint8_t p_configuration) const {
    USB_PRINTF("UsbDevice::%s(): USB Configuration = %x\r\n", __func__, p_configuration);

    /* TODO This piece of code only supports a single USB configuration per Device. Does it make to assert() for the right configuration number here? */
    if (p_configuration != 0) {
        this->m_configuration.enable();
    } else {
        this->m_configuration.disable();
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
uint8_t
UsbDevice::getConfiguration(void) const {
  uint8_t cfg = this->m_configuration.isEnabled() ? 1 : 0;

  return cfg;
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
