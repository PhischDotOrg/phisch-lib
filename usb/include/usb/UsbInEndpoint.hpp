/*-
 * $Copyright$
-*/

#ifndef _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510
#define _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510

#include <usb/UsbTypes.hpp>
#include <cassert>
#include <cstddef>

namespace usb {

/*******************************************************************************
 *
 ******************************************************************************/
class UsbBulkInEndpoint {
public:
    virtual void enable(void) const = 0;
    virtual void disable(void) const = 0;
    virtual void write(const uint8_t * const p_data, const size_t p_length) const = 0;
};

template<class UsbHwBulkInEndpointT>
class UsbBulkInEndpointT : public UsbBulkInEndpoint {
private:
    UsbHwBulkInEndpointT &  m_hwEndpoint;

public:
    UsbBulkInEndpointT(UsbHwBulkInEndpointT &p_hwEndpoint) : m_hwEndpoint(p_hwEndpoint) {

    }

    void enable(void) const override {
        this->m_hwEndpoint.enable();
    }

    void disable(void) const override {
        this->m_hwEndpoint.disable();
    }

    void write(const uint8_t * const p_data, const size_t p_length) const override {
        this->m_hwEndpoint.write(p_data, p_length);
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
class UsbIrqInEndpoint {
public:
            UsbIrqInEndpoint() = default;
    virtual ~UsbIrqInEndpoint() {};

    virtual void enable(void) const = 0;
    virtual void disable(void) const = 0;
    virtual void write(const uint8_t * const p_data, const size_t p_length) const = 0;    
};

template<class UsbHwIrqInEndpointT>
class UsbIrqInEndpointT : public UsbIrqInEndpoint {
private:
    UsbHwIrqInEndpointT &   m_hwEndpoint;

public:
    constexpr UsbIrqInEndpointT(UsbHwIrqInEndpointT &p_hwEndpoint) : m_hwEndpoint(p_hwEndpoint) {

    }

    void enable(void) const override {
        this->m_hwEndpoint.enable();
    }

    void disable(void) const override {
        this->m_hwEndpoint.disable();
    }

    void write(const uint8_t * const p_data, const size_t p_length) const override {
        this->m_hwEndpoint.write(p_data, p_length);
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
class UsbCtrlInEndpoint {
public:
            UsbCtrlInEndpoint() = default;
    virtual ~UsbCtrlInEndpoint() = default;

    virtual void write(const uint8_t * const p_data, const size_t p_length) const = 0;
};

template<class UsbHwCtrlInEndpointT>
class UsbCtrlInEndpointT : public UsbCtrlInEndpoint {
private:
    UsbHwCtrlInEndpointT &   m_hwEndpoint;

public:
    constexpr UsbCtrlInEndpointT(UsbHwCtrlInEndpointT &p_hwEndpoint) : m_hwEndpoint(p_hwEndpoint) {

    }

    void write(const uint8_t * const p_data, const size_t p_length) const override {
        this->m_hwEndpoint.write(p_data, p_length);
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510 */