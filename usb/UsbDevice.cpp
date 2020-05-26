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
} /* namespace usb */
