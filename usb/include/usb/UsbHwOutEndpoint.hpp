/*-
 * $Copyright$
-*/
#ifndef _OUT_ENDPOINT_HPP_23F63D27_C8CD_491D_9EE6_07CF2EF8D4B3
#define _OUT_ENDPOINT_HPP_23F63D27_C8CD_491D_9EE6_07CF2EF8D4B3

#include <assert.h>
#include <stdint.h>

#include <usb/UsbCtrlOutEndpoint.hpp>

#if 0
namespace usb {

// class UsbOutEndpoint;
// class UsbBulkOutEndpoint;
// class UsbCtrlOutEndpoint;

/***************************************************************************//**
 * @brief Interface to the hardware-dependent OUT Endpoint.
 * 
 * This interface class is used by the hardware-independent layer to access the
 * hardware-specific implementation of an OUT Endpoint.
 * 
 * The interface is currently implemented by ::usb::stm32f4::OutEndpointViaSTM32F4.
 ******************************************************************************/
template<class UsbOutEndpointT>
class UsbHwOutEndpointT {
protected:
    /***************************************************************************//**
     * @brief Callback to USB-generic OUT Endpoint implementation.
     ******************************************************************************/
    UsbOutEndpointT *   m_endpointCallback;

public:
    UsbHwOutEndpointT(void) : m_endpointCallback(nullptr) {
    }

    virtual ~UsbHwOutEndpointT() {
    }

    virtual void registerEndpointCallback(UsbOutEndpointT &p_endpointCallback) = 0;
    virtual void unregisterEndpointCallback(UsbOutEndpointT &p_endpointCallback) = 0;
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif

#endif /* _OUT_ENDPOINT_HPP_23F63D27_C8CD_491D_9EE6_07CF2EF8D4B3 */
