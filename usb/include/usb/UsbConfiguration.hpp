/*-
 * $Copyright$
-*/

#ifndef __USB_CONFIGURATION_HPP_8D6BB979_1F97_40DC_9497_FF90641EE32F
#define __USB_CONFIGURATION_HPP_8D6BB979_1F97_40DC_9497_FF90641EE32F

#include <usb/UsbTypes.hpp>
#include <sys/types.h>

namespace usb {

class UsbInterface;

/*******************************************************************************
 *
 ******************************************************************************/
class UsbConfiguration {
public:
            UsbConfiguration(void) {};
    virtual ~UsbConfiguration() {};

    virtual void            enable(void) = 0;
    virtual void            disable(void) = 0;

    virtual bool            isEnabled(void) const = 0;

    virtual const void *    getDescriptor(void) const = 0;
    virtual size_t          getDescriptorSize(void) const = 0;
};

/*******************************************************************************
 *
 ******************************************************************************/
template<typename UsbConfigurationDescriptorT>
class UsbConfigurationT : public UsbConfiguration {
private:
    const UsbConfigurationDescriptorT & m_descriptor;
    UsbInterface &                      m_interface;
    bool                                m_enabled;

public:
            UsbConfigurationT(const UsbConfigurationDescriptorT &p_descriptor, UsbInterface &p_interface);
    virtual ~UsbConfigurationT();

    virtual void            enable(void);
    virtual void            disable(void);

    virtual bool            isEnabled(void) const;

    virtual const void *    getDescriptor(void) const;
    virtual size_t          getDescriptorSize(void) const;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#include <usb/UsbConfiguration.cpp>

#endif /* __USB_CONFIGURATION_HPP_8D6BB979_1F97_40DC_9497_FF90641EE32F */
