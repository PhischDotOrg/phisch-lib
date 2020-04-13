/*-
 * $Copyright$
-*/

#ifndef _SPIACCESSVIAI2C_HPP_4a73e836_a840_4302_af1b_aabe30ef233f
#define _SPIACCESSVIAI2C_HPP_4a73e836_a840_4302_af1b_aabe30ef233f

#include "spi/Spi.hpp"
#include "i2c/I2cDevice.hpp"
#include <stdint.h>
#include <stddef.h>

#if defined(DEBUG_BUILD) && defined(GMOCK_FOUND)
#include <gmock/gmock.h>
#endif /* defined(DEBUG_BUILD) && defined(GMOCK_FOUND) */

namespace spi {

/*******************************************************************************
 * 
 ******************************************************************************/
template<typename I2cDeviceT>
class SpiAccessViaI2cT {
private:
    I2cDeviceT * const  m_i2c;
    const uint32_t      m_base;

    static const uint8_t    REG_RXTX;
    static const uint8_t    REG_CTRL;
    static const uint8_t    REG_NSEL;
    static const uint8_t    REG_DIV;

    int update_nsel(const uint8_t p_device, const bool p_select) const;

public:
    SpiAccessViaI2cT(I2cDeviceT * const p_i2c, const uint32_t p_base);
    ~SpiAccessViaI2cT();

    int select(const uint8_t p_device) const;
    int deselect(const uint8_t p_device) const;
    int shift(const uint8_t p_bits, uint8_t * const p_rx = NULL, const uint8_t p_tx = 0xFFu, const spi::Mode p_mode = SpiMode0) const;
    
    static const unsigned SHIFT_MAX_BITS = 8;    
};

}; /* namespace spi */

#if defined(DEBUG_BUILD) && defined(GMOCK_FOUND)
namespace i2c {
/*******************************************************************************
 * 
 ******************************************************************************/
class I2cDeviceMock {
public:
    MOCK_CONST_METHOD3(read, ssize_t(const uint32_t p_addr, uint8_t * const p_buffer, const size_t p_length));
    MOCK_CONST_METHOD3(write, ssize_t(const uint32_t p_addr, const uint8_t * const p_buffer, const size_t p_length));
};
} /* namespace i2c */
#endif /* defined(DEBUG_BUILD) && defined(GMOCK_FOUND) */

#endif /* _SPIACCESSVIAI2C_HPP_4a73e836_a840_4302_af1b_aabe30ef233f */
