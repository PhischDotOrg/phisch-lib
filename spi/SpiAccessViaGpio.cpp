/*-
 * $Copyright$
-*/

#include <spi/SpiAccessViaGpio.hpp>

#include <assert.h>
#include <errno.h>
#include <unistd.h>

namespace spi {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename PinT>
AccessViaGpioT<PinT>::AccessViaGpioT(PinT * const p_sclk, PinT * const p_nsel, PinT * const p_mosi, PinT * const p_miso)
  : m_sclk(p_sclk), m_nsel(p_nsel), m_mosi(p_mosi), m_miso(p_miso) {

}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename PinT>
AccessViaGpioT<PinT>::~AccessViaGpioT() {

}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename PinT>
int
AccessViaGpioT<PinT>::select(const uint8_t p_device) const {
    return this->m_nsel[p_device].set(PinT::Off);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename PinT>
int
AccessViaGpioT<PinT>::deselect(const uint8_t p_device) const {
    return this->m_nsel[p_device].set(PinT::On);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename PinT>
int
AccessViaGpioT<PinT>::shift(const uint8_t p_bits, uint8_t * const p_rx /* = NULL */,
  const uint8_t p_tx /* = 0xFFu */, const spi::Mode p_mode /* = SpiMode0 */) const {
    int rc = -1;

    if ((p_bits < 1) || (p_bits > 8))
        return EINVAL;

    assert((p_mode == SpiMode0) || (p_mode == SpiMode1)
      || (p_mode == SpiMode2) || (p_mode == SpiMode3));

    assert((p_bits > 0) && (p_bits <= 8));

    for (int i = 0; i < p_bits; i++) {
        bool tx, rx;

        tx = ((p_tx >> (7 - i)) & 1) != 0;

        rc = this->shift(&rx, tx, p_mode);
        if (rc)
            goto out;

        if (p_rx != NULL) {
            *p_rx <<= 1;
            *p_rx |= rx ? 1 : 0;
        }
    }
    rc = 0;

out:
    return (rc);
}

/*******************************************************************************
 * SPI Modes:
 *  - 0 -> CPOL 0, CPHA 0
 *  - 1 -> CPOL 0, CHPA 1
 *  - 2 -> CPOL 1, CPHA 0
 *  - 3 -> CPOL 1, CHPA 1
 *
 * CPOL = Base of SPI Clock (0=Lo, 1=Hi)
 * CPHA = Capturing Clock Edge (0=First, 1=Second)
 *
 ******************************************************************************/
template<typename PinT>
int
AccessViaGpioT<PinT>::shift(bool * const p_rx /* = NULL */,
  const bool p_tx /* = 0xFFu */, const spi::Mode p_mode /* = SpiMode0 */) const {
    int rc = -1;
    typename PinT::mode_t rx = PinT::HiZ;

    /* SPI Clock to idle state */
    if ((p_mode == SpiMode0) || (p_mode == SpiMode1)) {
        rc = this->m_sclk->set(PinT::Off);
    } else {
        rc = this->m_sclk->set(PinT::On);
    }
    if (rc)
        goto out;

    if (this->m_delay) usleep(this->m_delay);

    /* First Clock Edge */
    if ((p_mode == SpiMode0) || (p_mode == SpiMode2)) {
        rc = this->m_mosi->set(p_tx ? PinT::On : PinT::Off);
    } else {
        rc = this->m_miso->get(rx);
    }
    if (rc)
        goto out;

    if (this->m_delay) usleep(this->m_delay);

    /* SPI Clock to inverted state */
    if ((p_mode == SpiMode0) || (p_mode == SpiMode1)) {
        rc = this->m_sclk->set(PinT::On);
    } else {
        rc = this->m_sclk->set(PinT::Off);
    }
    if (rc)
        goto out;

    if (this->m_delay) usleep(this->m_delay);

    /* Second Clock Edge */
    if ((p_mode == SpiMode1) || (p_mode == SpiMode3)) {
        rc = this->m_mosi->set(p_tx ? PinT::On : PinT::Off);
    } else {
        rc = this->m_miso->get(rx);
    }
    if (rc)
        goto out;

    if (this->m_delay) usleep(this->m_delay);

    /* SPI Clock to idle state */
    if ((p_mode == SpiMode0) || (p_mode == SpiMode1)) {
        rc = this->m_sclk->set(PinT::Off);
    } else {
        rc = this->m_sclk->set(PinT::On);
    }
    if (rc)
        goto out;

    if (this->m_delay) usleep(this->m_delay);

    if (p_rx != NULL)
        *p_rx = rx == PinT::On ? true : false;

    rc = 0;
out:
    return rc;
}

}; /* namespace spi */
