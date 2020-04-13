/*-
 * $Copyright$
-*/

#include "spi/SpiAccessViaI2c.hpp"

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>

namespace spi {

template<typename I2cDeviceT> const uint8_t SpiAccessViaI2cT<I2cDeviceT>::REG_RXTX = 0u;
template<typename I2cDeviceT> const uint8_t SpiAccessViaI2cT<I2cDeviceT>::REG_CTRL = 1u;
template<typename I2cDeviceT> const uint8_t SpiAccessViaI2cT<I2cDeviceT>::REG_NSEL = 2u;
template<typename I2cDeviceT> const uint8_t SpiAccessViaI2cT<I2cDeviceT>::REG_DIV = 3u;

/*******************************************************************************
 *
 ******************************************************************************/
template<typename I2cDeviceT>
SpiAccessViaI2cT<I2cDeviceT>::SpiAccessViaI2cT(I2cDeviceT * const p_i2c, const uint32_t p_base)
  : m_i2c(p_i2c), m_base(p_base) {

}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename I2cDeviceT>
SpiAccessViaI2cT<I2cDeviceT>::~SpiAccessViaI2cT() {

}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename I2cDeviceT>
int
SpiAccessViaI2cT<I2cDeviceT>::update_nsel(const uint8_t p_device, const bool p_select) const {
    int rc = -1;
    uint8_t nsel;

    assert(this->m_i2c != NULL);

    if (p_device > 7) {
        rc = EINVAL;
        goto out;
    }

    rc = this->m_i2c->read(this->m_base + REG_NSEL, &nsel, sizeof(nsel));
    if (rc != sizeof(nsel)) {
        rc = ENXIO;
        goto out;
    }

    if (p_select) {
        nsel &= ~(1 << p_device);
    } else {
        nsel |= (1 << p_device);
    }

    rc = this->m_i2c->write(this->m_base + REG_NSEL, &nsel, sizeof(nsel));
    if (rc != sizeof(nsel)) {
        rc = ENXIO;
        goto out;
    }

    rc = 0;

out:
    return rc;
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename I2cDeviceT>
int
SpiAccessViaI2cT<I2cDeviceT>::select(const uint8_t p_device) const {
    return this->update_nsel(p_device, true);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename I2cDeviceT>
int
SpiAccessViaI2cT<I2cDeviceT>::deselect(const uint8_t p_device) const {
    return this->update_nsel(p_device, false);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename I2cDeviceT>
int
SpiAccessViaI2cT<I2cDeviceT>::shift(const uint8_t p_bits, uint8_t * const p_rx /* = NULL */,
  const uint8_t p_tx /* = 0xFFu */, const spi::Mode p_mode /* = SpiMode0 */) const {
    assert(this->m_i2c != NULL);
    int rc = -1;
    uint8_t data[2];

    if ((p_bits < 1) || (p_bits > 8))
        return EINVAL;

    if ((p_mode != SpiMode0) && (p_mode != SpiMode1)
      && (p_mode != SpiMode2) && (p_mode != SpiMode3))
        return EINVAL;

    data[0] = p_tx;
    data[1] = p_bits - 1;
    data[1] |= (((uint8_t) p_mode) << 3) & 0x18; // Setup SPI mode
    data[1] |= 0x80; // Set start bit

    rc = this->m_i2c->write(this->m_base + REG_RXTX, data, sizeof(data));
    if (rc != sizeof(data)) {
        rc = ENXIO;
        goto out;
    }

    if (p_rx != NULL) {
        rc = this->m_i2c->read(this->m_base + REG_RXTX, p_rx, 1);
        if (rc != 1) {
            rc = ENXIO;
            goto out;
        }
    }

    rc = 0;
out:
    return rc;
}

} /* namespace spi */
