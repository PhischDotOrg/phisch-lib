/*-
 * $Copyright$
-*/

#ifndef _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510
#define _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510

#include <usb/UsbTypes.hpp>
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
class UsbBulkInEndpointT : UsbBulkInEndpoint {
private:
    UsbHwBulkInEndpointT &  m_hwEndpoint;

public:
    UsbBulkInEndpointT(UsbHwBulkInEndpointT &p_hwEndpoint) : m_hwEndpoint(p_hwEndpoint) {

    }

    virtual ~UsbBulkInEndpointT() {

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
    virtual bool isEnabled(void) const = 0;
};

template<class UsbHwIrqInEndpointT>
class UsbIrqInEndpointT : public UsbIrqInEndpoint {
private:
    UsbHwIrqInEndpointT &   m_hwEndpoint;

public:
    constexpr UsbIrqInEndpointT(UsbHwIrqInEndpointT &p_hwEndpoint) : m_hwEndpoint(p_hwEndpoint) {

    }

    virtual ~UsbIrqInEndpointT() override {

    }

    void enable(void) const override {
        this->m_hwEndpoint.enable();
    }

    void disable(void) const override {
        this->m_hwEndpoint.disable();
    }

    bool isEnabled(void) const override {
        return this->m_hwEndpoint.isEnabled();
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
    virtual ~UsbCtrlInEndpoint() {};

    virtual void write(const uint8_t * const p_data, const size_t p_length) const = 0;
    virtual void writeString(const ::usb::UsbStringDescriptor &p_string, const size_t p_len) const = 0;
};

template<class UsbHwCtrlInEndpointT>
class UsbCtrlInEndpointT : public UsbCtrlInEndpoint {
private:
    UsbHwCtrlInEndpointT &   m_hwEndpoint;

public:
    constexpr UsbCtrlInEndpointT(UsbHwCtrlInEndpointT &p_hwEndpoint) : m_hwEndpoint(p_hwEndpoint) {

    }

    ~UsbCtrlInEndpointT() {

    }
    
    void write(const uint8_t * const p_data, const size_t p_length) const override {
        this->m_hwEndpoint.write(p_data, p_length);
    }

    void writeString(const ::usb::UsbStringDescriptor &p_string, const size_t p_len) const override {
        this->m_hwEndpoint.writeString(p_string, p_len);
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USB_BULK_IN_ENDPOINT_HPP_76D17E20_63CB_4205_82BB_AEE1A21ED510 */