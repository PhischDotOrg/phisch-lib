/*-
 * $Copyright$
-*/

#include <usb/UsbControlPipe.hpp>
#include <usb/UsbDevice.hpp>
#include <usb/UsbInterface.hpp>
#include <usb/UsbTypes.hpp>

#include <usb/UsbInEndpoint.hpp>
#include <usb/UsbCtrlOutEndpoint.hpp>

#include <usb/UsbConfiguration.hpp>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include <assert.h>
#include <stddef.h>

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */

#include <algorithm>

namespace usb {


/*******************************************************************************
 *
 ******************************************************************************/
UsbControlPipe::UsbControlPipe(UsbCtrlInEndpoint &p_inEndpoint, UsbCtrlOutEndpoint &p_outEndpoint)
: m_inEndpoint(p_inEndpoint), m_outEndpoint(p_outEndpoint),
    m_usbDevice(nullptr), m_usbInterface(nullptr)
{
    this->m_outEndpoint.registerCtrlPipe(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
UsbControlPipe::~UsbControlPipe() {
    this->m_outEndpoint.unregisterCtrlPipe(*this);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbControlPipe::decodeInterfaceRequest(const UsbSetupPacket_t &p_setupPacket, const void * const p_data, const size_t p_length) const {
    assert(this->m_usbInterface != NULL);

    this->m_usbInterface->handleCtrlRequest(p_setupPacket, p_data, p_length);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbControlPipe::decodeDeviceRequest(const UsbSetupPacket_t &p_setupPacket, const void * const /* p_data */ /* = nullptr */, const size_t /* p_length */ /* = 0 */) const {
    UsbRequest_t request = static_cast<UsbRequest_t>(p_setupPacket.m_bRequest);

    USB_PRINTF("UsbControlPipe::%s(): request=%d, m_wLength=%i\r\n", __func__, request, p_setupPacket.m_wLength);

    switch (request) {
    case e_SetAddress:
        this->m_usbDevice->setAddress(p_setupPacket.m_wValue & 0x7F);
        /* Acknowledge the setAddress() command on the Default Ctrl Endpoint */
        this->write(NULL, 0);
        break;
    case e_GetDescriptor:
        this->getDescriptor(p_setupPacket.m_wValue, p_setupPacket.m_wLength);
        break;
    case e_SetConfiguration:
        this->m_usbDevice->setConfiguration(p_setupPacket.m_wValue);
        this->write(NULL, 0);
        break;
    case e_GetStatus:
        this->m_usbDevice->getStatus(p_setupPacket.m_wLength);
        break;
    case e_GetConfiguration:
        assert(p_setupPacket.m_wValue == 0);
        assert(p_setupPacket.m_wIndex == 0);

        this->getDeviceConfiguration(p_setupPacket.m_wLength);
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
UsbControlPipe::getDeviceConfiguration(const uint8_t p_len) const {
    assert(this->m_usbDevice != NULL);
    assert(p_len == sizeof(uint8_t));

    uint8_t cfg = this->m_usbDevice->getConfiguration();

    this->write(&cfg, sizeof(cfg));
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbControlPipe::getDescriptor(const uint16_t p_descriptor, const size_t p_len) const {
    const UsbDescriptorTypeId_t descriptorType = static_cast<UsbDescriptorTypeId_t>((p_descriptor >> 8) & 0x07);
    uint8_t                     descriptorId = p_descriptor & 0xFF;

    USB_PRINTF("UsbControlPipe::%s(): descriptorType=%d, descriptorId=%d\r\n", __func__, descriptorType, descriptorId);

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
UsbControlPipe::transferComplete(const size_t p_numBytes) const {
    USB_PRINTF("UsbControlPipe::%s(p_numBytes=%d)\r\n", __func__, p_numBytes);

    /*
     * If p_numBytes == 0, then we got called by the IRQ triggered by the Host's ACK packet. This
     * represents the End of the Status Stage and there's nothing left to do here.
     * 
     * If p_numBytes != 0, then we got called by the OUT Endpoint's Transfer Complete IRQ. This
     * represents the end of the Data (OUT) Stage, so we need to decode the request fully and
     * acknowledge the end of the transfer.
     */
    if (p_numBytes) {
        /* FIXME As of now, there are no requests that require further processing */
        // this->decodeSetupPacket(p_setupPacket, p_data, p_numBytes);

        /* FIXME We should probably evaluate some return code here... */
        this->m_inEndpoint.write(nullptr, 0);
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbControlPipe::setupStageComplete(const UsbSetupPacket_t &p_setupPacket) const {
    USB_PRINTF("UsbControlPipe::%s(): m_bmRequestType=0x%x m_bRequest=0x%x\r\n", __func__,
      p_setupPacket.m_bmRequestType, p_setupPacket.m_bRequest);

    this->decodeSetupPacket(p_setupPacket);
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbControlPipe::decodeSetupPacket(const UsbSetupPacket_t &p_setupPacket, const void * const p_data /* = nullptr */, const size_t p_length /* = 0 */) const {
    const UsbRecipient_t usbRecipient = static_cast<UsbRecipient_t>(p_setupPacket.m_bmRequestType & 0x0F);

    USB_PRINTF("UsbControlPipe::%s(): m_bmRequestType=0x%x p_setupPacket=0x%x\r\n", __func__,
      p_setupPacket.m_bmRequestType, p_setupPacket.m_bRequest);

    switch (usbRecipient) {
    case e_Device:
        this->decodeDeviceRequest(p_setupPacket, p_data, p_length);
        break;
    case e_Interface:
        this->decodeInterfaceRequest(p_setupPacket, p_data, p_length);
        break;
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
UsbControlPipe::getDeviceDescriptor(const uint8_t p_descriptorId, const size_t p_len) const {
    assert(p_descriptorId == 0);
    USB_PRINTF("UsbControlPipe::%s(): p_descriptorId=%d, p_len=%d\r\n", __func__, p_descriptorId, p_len);

    this->write(reinterpret_cast<const uint8_t *>(&this->m_usbDevice->m_deviceDescriptor), std::min(sizeof(this->m_usbDevice->m_deviceDescriptor), p_len));
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbControlPipe::getDeviceQualifierDescriptor(const uint8_t p_descriptorId, const size_t p_len) const {
    assert(p_descriptorId == 0);
    this->write(reinterpret_cast<const uint8_t *>(&this->m_usbDevice->m_deviceQualifierDescriptor), std::min(sizeof(this->m_usbDevice->m_deviceQualifierDescriptor), p_len));
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbControlPipe::getConfigurationDescriptor(const uint8_t p_descriptorId, const size_t p_len) const {
    assert(p_descriptorId == 0);

    const void *addr = this->m_usbDevice->m_configuration.getDescriptor();
    size_t len = this->m_usbDevice->m_configuration.getDescriptorSize();

    USB_PRINTF("UsbControlPipe::%s(): addr=%p, len=%d, p_len=%d\r\n", __func__, addr, len, p_len);

    this->write(reinterpret_cast<const uint8_t *>(addr), std::min(len, p_len));
}

/***************************************************************************//**
 * @brief Transmit String Descriptor to Host.
 * 
 * Transmits a USB String Descriptor as a response to a "Get Descriptor" request
 * from the USB Host.
 * 
 * @param p_descriptorId String Descriptor ID in #m_usbDevice.
 * @param p_len Transmission Length as requested in the \c SETUP packet.
 ******************************************************************************/
void
UsbControlPipe::getStringDescriptor(const uint8_t p_descriptorId, const size_t p_len) const {
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

        this->write(buffer, std::min<unsigned>(buffer[0], p_len));
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
