/*-
 * $Copyright$
-*/

#include <usb/DefaultControlPipe.hpp>
#include <usb/GenericUsbDevice.hpp>
#include <usb/UsbTypes.hpp>

#include <usb/UsbCtrlInEndpoint.hpp>
#include <usb/UsbCtrlOutEndpoint.hpp>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include <assert.h>
#include <stddef.h>

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
DefaultControlPipe::DefaultControlPipe(UsbCtrlInEndpoint &p_inEndpoint, UsbCtrlOutEndpoint &p_outEndpoint)
  : UsbControlPipe(p_inEndpoint, p_outEndpoint) {

}

/*******************************************************************************
 *
 ******************************************************************************/
DefaultControlPipe::~DefaultControlPipe() {

}

/*******************************************************************************
 *
 ******************************************************************************/
void
DefaultControlPipe::registerUsbDevice(GenericUsbDevice &p_usbDevice) {
  assert(this->m_usbDevice == NULL);

  this->m_usbDevice = &p_usbDevice;
}

/*******************************************************************************
 *
 ******************************************************************************/
void
DefaultControlPipe::unregisterUsbDevice(GenericUsbDevice &p_usbDevice) {
  assert(this->m_usbDevice != NULL);
  assert(this->m_usbDevice == &p_usbDevice);

  this->m_usbDevice = NULL;
}


/*******************************************************************************
 *
 ******************************************************************************/
void
DefaultControlPipe::handleDeviceRequest(const UsbSetupPacket_t &p_setupPacket) const {
    UsbRequest_t request = static_cast<UsbRequest_t>(p_setupPacket.m_bRequest);

    USB_PRINTF("DefaultControlPipe::%s(): request=%d, m_wLength=%i\r\n", __func__, request, p_setupPacket.m_wLength);

    switch (request) {
    case e_SetAddress:
        this->m_usbDevice->setAddress(p_setupPacket.m_wValue & 0x7F);
        /* Acknowledge the setAddress() command on the Default Ctrl Endpoint */
        this->m_inEndpoint.write(NULL, 0, 0);
        break;
    case e_GetDescriptor:
        this->getDescriptor(p_setupPacket.m_wValue, p_setupPacket.m_wLength);
        break;
    case e_SetConfiguration:
        this->m_usbDevice->setConfiguration(p_setupPacket.m_wValue);
        this->m_inEndpoint.write(NULL, 0, 0);
        break;
    case e_GetStatus:
        this->m_usbDevice->getStatus(p_setupPacket.m_wLength);
        break;
    default:
        assert(false);
        break;
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
void
DefaultControlPipe::getDescriptor(const uint16_t p_descriptor, const size_t p_len) const {
    const UsbDescriptorTypeId_t descriptorType = static_cast<UsbDescriptorTypeId_t>((p_descriptor >> 8) & 0x07);
    uint8_t                     descriptorId = p_descriptor & 0xFF;

    USB_PRINTF("DefaultControlPipe::%s(): descriptorType=%d, descriptorId=%d\r\n", __func__, descriptorType, descriptorId);

    switch (descriptorType) {
    case UsbDescriptorTypeId_e::e_Device:
        this->getDeviceDescriptor(descriptorId, p_len);
        break;
    case UsbDescriptorTypeId_e::e_String:
        this->getStringDescriptor(descriptorId, p_len);
        break;
    case UsbDescriptorTypeId_e::e_Configuration:
        this->getConfigurationDescriptor(descriptorId, p_len);
        break;
    case UsbDescriptorTypeId_e::e_DeviceQualifier:
        this->getDeviceQualifierDescriptor(descriptorId, p_len);
        break;
    case UsbDescriptorTypeId_e::e_Interface:
    case UsbDescriptorTypeId_e::e_Endpoint:
    case UsbDescriptorTypeId_e::e_OtherSpeedConfig:
        /* Not yet implemented */
        assert(false);
        break;
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
void
DefaultControlPipe::dataStageReceive(const uint32_t /* p_rxData */) {

}

/*******************************************************************************
 *
 ******************************************************************************/
void
DefaultControlPipe::dataStageComplete(void) {

}

/*******************************************************************************
 *
 ******************************************************************************/
void
DefaultControlPipe::setupStageComplete(const UsbSetupPacket_t &p_setupPacket) const {
    const UsbRecipient_t usbRecipient = static_cast<UsbRecipient_t>(p_setupPacket.m_bmRequestType & 0x0F);

    USB_PRINTF("DefaultControlPipe::%s(): usbRecipient=%d\r\n", __func__, usbRecipient);

    switch (usbRecipient) {
    case e_Device:
        this->handleDeviceRequest(p_setupPacket);
        break;
    case e_Interface:
    case e_Endpoint:
    case e_Other:
    default:
        /* Not yet implemented */
        assert(false);
        break;
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
void
DefaultControlPipe::getDeviceDescriptor(const uint8_t p_descriptorId, const size_t p_len) const {
    assert(p_descriptorId == 0);
    USB_PRINTF("DefaultControlPipe::%s(): p_descriptorId=%d, p_len=%d\r\n", __func__, p_descriptorId, p_len);
    this->m_inEndpoint.write(reinterpret_cast<const uint8_t *>(&this->m_usbDevice->m_deviceDescriptor), sizeof(this->m_usbDevice->m_deviceDescriptor), p_len);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
DefaultControlPipe::getDeviceQualifierDescriptor(const uint8_t p_descriptorId, const size_t p_len) const {
    assert(p_descriptorId == 0);
    this->m_inEndpoint.write(reinterpret_cast<const uint8_t *>(&this->m_usbDevice->m_deviceQualifierDescriptor), sizeof(this->m_usbDevice->m_deviceQualifierDescriptor), p_len);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
DefaultControlPipe::getConfigurationDescriptor(const uint8_t p_descriptorId, const size_t p_len) const {
    assert(p_descriptorId == 0);
    this->m_inEndpoint.write(reinterpret_cast<const uint8_t *>(this->m_usbDevice->m_configurationDescriptor), this->m_usbDevice->m_configurationDescriptorSize, p_len);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
DefaultControlPipe::getStringDescriptor(const uint8_t p_descriptorId, const size_t p_len) const {
    UsbStringDescriptorId_t stringDescriptor = static_cast<UsbStringDescriptorId_t>(p_descriptorId);

    if (p_descriptorId >= e_StrDesc_Max) {
        goto out;
    }

    switch (stringDescriptor) {
    case e_StrDesc_LanguageId: {
        static uint8_t  buffer[64];
        UsbLangId_t *   cur = reinterpret_cast<UsbLangId_t *>(&buffer[2]);
        unsigned        idx;

        buffer[1] = e_String;

        for (idx = 0; idx < this->m_usbDevice->m_stringDescriptors.m_stringDescriptorTable.m_languageIds.m_numLanguages; idx++, cur++) {
            * cur = this->m_usbDevice->m_stringDescriptors.m_stringDescriptorTable.m_languageIds.m_langIds[idx];
        }

        buffer[0] = 2 + idx * sizeof(UsbLangId_t);

        this->m_inEndpoint.write(buffer, buffer[0], p_len);
    } break;
    case e_StrDesc_Manufacturer:
    case e_StrDesc_Product:
    case e_StrDesc_SerialNumber:
    case e_StrDesc_Configuration:
    case e_StrDesc_Interface:
        assert(stringDescriptor < e_StrDesc_Max);
        this->m_inEndpoint.writeString(this->m_usbDevice->m_stringDescriptors.m_stringDescriptors[stringDescriptor], p_len);
        break;
    default:
        break;
    }

out:
    return;
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
