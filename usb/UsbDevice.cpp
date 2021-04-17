/*-
 * $Copyright$
-*/

#include <usb/UsbDevice.hpp>
#include <usb/UsbControlPipe.hpp>
#include <usb/UsbHwDevice.hpp>
#include <usb/UsbConfiguration.hpp>

#include <assert.h>

#include <algorithm>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbDevice::setAddress(const uint8_t p_address) const {
    this->m_hwDevice.setAddress(p_address);
}


/*******************************************************************************
 *:
 ******************************************************************************/
const UsbConfiguration *
UsbDevice::setConfiguration(const uint8_t p_configuration) {
    USB_PRINTF("UsbDevice::%s(): USB Configuration = %x\r\n", __func__, p_configuration);

    const UsbConfiguration *newCfg = nullptr;

    if (this->m_activeConfiguration != 0) {
        const UsbConfiguration *activeCfg = this->m_configurations[this->m_activeConfiguration - 1];
        if (activeCfg != nullptr) {
            activeCfg->disable();
        }

        this->m_activeConfiguration = 0;
    }

    if ((p_configuration > 0) && (p_configuration <= m_maxConfigurations)) {
        newCfg = this->m_configurations[p_configuration - 1];
        if (newCfg != nullptr) {
            assert(this->m_ctrlPipe != nullptr);
            
            newCfg->enable(*(this->m_ctrlPipe));
            this->m_activeConfiguration = p_configuration;
        }
    }

    return newCfg;
}

/*******************************************************************************
 *
 ******************************************************************************/
void
UsbDevice::getDescriptor(const uint16_t p_descriptor, const size_t p_len) const {
    const UsbDescriptorTypeId_t descriptorType = static_cast<UsbDescriptorTypeId_t>((p_descriptor >> 8) & 0x07);
    uint8_t                     descriptorId = p_descriptor & 0xFF;

    USB_PRINTF("UsbDevice::%s(): descriptorType=%d, descriptorId=%d\r\n", __func__, descriptorType, descriptorId);

    switch (descriptorType) {
    case UsbDescriptorTypeId_e::e_Device:
        assert(descriptorId == 0);

        assert(this->m_ctrlPipe != nullptr);
        this->m_ctrlPipe->write(reinterpret_cast<const uint8_t *>(&this->m_deviceDescriptor), std::min(sizeof(this->m_deviceDescriptor), p_len));
        break;
    case UsbDescriptorTypeId_e::e_String:
        this->getStringDescriptor(descriptorId, p_len);
        break;
    case UsbDescriptorTypeId_e::e_Configuration:
        assert(descriptorId == 0);

#if defined(USB_DEBUG)
{
        const void *addr = reinterpret_cast<const uint8_t *>(this->getConfigurationDescriptor());
        size_t len = this->getConfigurationDescriptorSize();

        USB_PRINTF("UsbDevice::%s(): addr=%p, len=%d, p_len=%d\r\n", __func__, addr, len, p_len);
}
#endif /* defined (USB_DEBUG) */

        assert(this->m_ctrlPipe != nullptr);
        this->m_ctrlPipe->write(
          reinterpret_cast<const uint8_t *>(this->getConfigurationDescriptor()),
          std::min(this->getConfigurationDescriptorSize(), p_len)
        );
        break;
    case UsbDescriptorTypeId_e::e_DeviceQualifier:
        assert(descriptorId == 0);

        assert(this->m_ctrlPipe != nullptr);
        this->m_ctrlPipe->write(reinterpret_cast<const uint8_t *>(&this->m_deviceQualifierDescriptor), std::min(sizeof(this->m_deviceQualifierDescriptor), p_len));
        break;
    case UsbDescriptorTypeId_e::e_Interface:
    case UsbDescriptorTypeId_e::e_Endpoint:
    case UsbDescriptorTypeId_e::e_OtherSpeedConfig:
        /* Not yet implemented */
        assert(false);
        break;
    }
}

/***************************************************************************//**
 * @brief Transmit String Descriptor to Host.
 * 
 * Transmits a USB String Descriptor as a response to a "Get Descriptor" request
 * from the USB Host.
 * 
 * @param p_descriptorId String Descriptor ID.
 * @param p_len Transmission Length as requested in the \c SETUP packet.
 ******************************************************************************/
void
UsbDevice::getStringDescriptor(const uint8_t p_descriptorId, const uint8_t p_len) const {
    UsbStringDescriptorId_t stringDescriptor = static_cast<UsbStringDescriptorId_t>(p_descriptorId);

    uint8_t len;

    switch (stringDescriptor) {
    case e_StrDesc_LanguageId:
    case e_StrDesc_Manufacturer:
    case e_StrDesc_Product:
    case e_StrDesc_SerialNumber:
    case e_StrDesc_Configuration:
    case e_StrDesc_Interface:
        assert(stringDescriptor < e_StrDesc_Max);

        /*
         * Some USB Hosts (Windows) attempt to read -1 (255) Bytes when requesting a String
         * Descriptor; others attempt to read the length of the Descriptor first and then
         * only request the length of the Descriptor.
         *
         * To prevent a Host from reading more data than available on the Device, we need
         * to saturate the transmit length.
         * 
         * Unfortunately, the lenght of the Descriptor is not available here in a syntactically
         * more sugary way. Instead, we need to extract it from the first Byte.
         */
        len = this->m_stringDescriptors.m_array[stringDescriptor][0];
        assert(len > 2);

        assert(this->m_ctrlPipe != nullptr);
        this->m_ctrlPipe->write(this->m_stringDescriptors.m_array[stringDescriptor], std::min(p_len, len));
        break;
    case e_StrDesc_Max:
        assert(false);
        break;
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
