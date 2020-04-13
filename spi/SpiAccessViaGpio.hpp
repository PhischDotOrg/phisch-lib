/*-
 * $Copyright$
-*/

#ifndef _SPI_ACCESS_VIA_GPIO_HPP_81a209a9_5518_4e22_a8a2_1623e7697e9f
#define _SPI_ACCESS_VIA_GPIO_HPP_81a209a9_5518_4e22_a8a2_1623e7697e9f

#if defined(DEBUG_BUILD) && defined(GMOCK_FOUND)
#include <gmock/gmock.h>
#endif /* defined(DEBUG_BUILD) && defined(GMOCK_FOUND) */

#include <gpio/GpioPin.hpp>
#include <spi/Spi.hpp>

namespace spi {

template<typename PinT>
class AccessViaGpioT {
    static const int m_delay = 0;

private:
    PinT * const    m_sclk;
    PinT * const    m_nsel;
    PinT * const    m_mosi;
    PinT * const    m_miso;

    int shift(bool * const p_rx /* = NULL */, const bool p_tx /* = 0xFFu */, const spi::Mode p_mode /* = SpiMode0 */) const;

public:
    AccessViaGpioT(PinT * const p_sclk, PinT * const p_nsel, PinT * const p_mosi, PinT * const p_miso);
    ~AccessViaGpioT();

    int select(const uint8_t p_device) const;
    int deselect(const uint8_t p_device) const;
    int shift(const uint8_t p_bits, uint8_t * const p_rx = NULL, const uint8_t p_tx = 0xFFu, const spi::Mode p_mode = SpiMode0) const;

    static const unsigned SHIFT_MAX_BITS = 8;    
};

typedef AccessViaGpioT<gpio::GpioPin> SpiAccessViaGpio;

}; /* namespace spi */

#endif /* !_SPI_ACCESS_VIA_GPIO_HPP_81a209a9_5518_4e22_a8a2_1623e7697e9f */
