/*-
 * $Copyright$
-*/

#include <usb/GenericUsbDevice.hpp>
#include <usb/DefaultControlPipe.hpp>
#include <usb/UsbHwDevice.hpp>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
GenericUsbDevice::GenericUsbDevice(UsbHwDevice &p_hwDevice, DefaultControlPipe &p_ctrlPipe,
  const void * const p_configurationDescriptor, size_t p_configurationDescriptorSize,
  const ::usb::UsbStringDescriptors_t &p_stringDescriptors)
  : m_hwDevice(p_hwDevice),
    m_ctrlPipe(p_ctrlPipe),
    m_maxConfigurations(1),
    m_deviceDescriptor {
        sizeof(m_deviceDescriptor),                                     /* m_bLength */
        ::usb::UsbDescriptorTypeId_t::e_Device,                         /* m_bDescriptorType */
        { 0x00, 0x02 },                                                 /* m_bLength */
        ::usb::UsbInterfaceClass_e::e_UsbInterface_VendorSpecific,      /* m_bDeviceClass */
        0x00,                                                           /* m_bDeviceSubClass */
        0x00,                                                           /* m_bDeviceProtocol */
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
    m_configurationDescriptor(p_configurationDescriptor),
    m_configurationDescriptorSize(p_configurationDescriptorSize),
    m_stringDescriptors(p_stringDescriptors)
{
  m_ctrlPipe.registerUsbDevice(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
GenericUsbDevice::~GenericUsbDevice() {
  m_ctrlPipe.unregisterUsbDevice(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
GenericUsbDevice::setAddress(const uint8_t p_address) const {
  this->m_hwDevice.setAddress(p_address);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
GenericUsbDevice::getStatus(const uint8_t /* p_len */) const {
    // assert(this->m_inEndpoints[0] != NULL);
    // this->m_inEndpoints[0]->write(reinterpret_cast<const uint8_t *>(&this->m_deviceStatus), sizeof(this->m_deviceStatus), p_len);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
GenericUsbDevice::setConfiguration(const uint8_t /* p_configuration */) const {
    USB_PRINTF("GenericUsbDevice::%s(): USB Configuration = %x\r\n", __func__, p_configuration);

    // FIXME Actually enable configuration here.
    // this->m_outEndpoints[p_configuration]->enable();
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
