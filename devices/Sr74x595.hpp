/*-
 * $Copyright$
-*/

#ifndef _SR_74x595_HPP_95e9842c_81a0_4379_8c05_d0d0c8c8858c
#define _SR_74x595_HPP_95e9842c_81a0_4379_8c05_d0d0c8c8858c

#include <sys/types.h>

#include <spi/SpiDevice.hpp>
#include <gpio/GpioPin.hpp>

namespace devices {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename SpiDeviceT = spi::SpiDevice, typename PinT = gpio::GpioPin>
class Sr74x595T {
public:
    Sr74x595T(SpiDeviceT &p_spiDevice, PinT * const p_reset, PinT * const p_enable)
     : m_spiDevice(p_spiDevice), m_reset(p_reset), m_enable(p_enable) {
    }

    ~Sr74x595T() {
    }

    int write(const void * const p_buffer, const size_t p_len) const;

    int reset(const bool p_reset) const;

    int enable(void) const;
    int disable(void) const;
    
protected:
    SpiDeviceT &    m_spiDevice;
    PinT * const    m_reset;
    PinT * const    m_enable;

private:
    int setFunctionPin(const PinT * const p_pin, const bool p_enable) const;
    int enable(const bool p_enable) const;
};

typedef Sr74x595T<> Sr74x595;

} /* namespace devices */

/*******************************************************************************
 *
 ******************************************************************************/
#if defined(UNITTEST)

#include <gmock/gmock.h>

namespace devices {

class Sr74x595Mock {
public:
    MOCK_CONST_METHOD0(enable, int(void));
    MOCK_CONST_METHOD0(disable, int(void));
    MOCK_CONST_METHOD1(reset, int(bool p_reset));

    MOCK_CONST_METHOD2(write, int(const void * const p_buffer, const size_t p_len));
}; /* class Sr74x595Mock */

} /* namespace devices */

#endif /* defined(UNITTEST) */

/*******************************************************************************
 *
 ******************************************************************************/
#include "Sr74x595.cpp"

#endif /* _SR_74x595_HPP_95e9842c_81a0_4379_8c05_d0d0c8c8858c */
