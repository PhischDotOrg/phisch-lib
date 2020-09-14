/*-
 * $Copyright$
-*/

#ifndef _SPI_ACCESS_HPP_31fc33e8_6d4f_4052_9fb8_072302deb083
#define _SPI_ACCESS_HPP_31fc33e8_6d4f_4052_9fb8_072302deb083

#include <spi/Spi.hpp>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>

#if defined(DEBUG_BUILD) && defined(GMOCK_FOUND)
#include <gmock/gmock.h>
#endif /* defined(DEBUG_BUILD) && defined(GMOCK_FOUND) */

/*******************************************************************************
 * 
 ******************************************************************************/
#if !defined(SPI_ENV)
#define SPI_ENV Undefined
#endif

#include "spi/SpiAccessViaGpio.hpp"
#if defined(HAVE_I2C)
#include "spi/SpiAccessViaI2c.hpp"
#endif /* defined(HAVE_I2C) */
#if defined(HAVE_STM32)
#include "stm32/Spi.hpp"
#endif

namespace spi {

typedef enum SpiAccessImpl_e {
    Gpio,
    GTest,
    I2c,
    STM32,
    Undefined,
} SpiAccessImpl_t;

template<SpiAccessImpl_t> struct SpiAccessChoice;

class SpiAccessMock;

template<> struct SpiAccessChoice<Gpio> {
    typedef SpiAccessViaGpio m_type;
};

template<> struct SpiAccessChoice<GTest> {
    typedef SpiAccessMock m_type;
};

#if defined(HAVE_I2C)
template<> struct SpiAccessChoice<I2c> {
    typedef SpiAccessViaI2cT<i2c::I2cDevice> m_type;
};
#endif /* defined(HAVE_I2C) */

/* FIXME */
#if 0
template<> struct SpiAccessChoice<STM32> {
    typedef stm32::SpiEngine m_type;
};
#endif

template<> struct SpiAccessChoice<Undefined> {
    typedef void m_type;
};

struct SpiAccessT {
    typedef SpiAccessChoice<SPI_ENV>::m_type m_type;
};

typedef SpiAccessT::m_type SpiAccess;

} /* namespace spi */

#endif /* _SPI_ACCESS_HPP_31fc33e8_6d4f_4052_9fb8_072302deb083 */
