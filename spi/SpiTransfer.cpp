/*-
 * $Copyright$
-*/

#include <spi/SpiTransfer.hpp>

#include <assert.h>
#include <stdio.h>

namespace spi {

/*******************************************************************************
 * 
 ******************************************************************************/
template<typename DeviceT>
TransferT<DeviceT>::TransferT(DeviceT * const p_device, std::ostream &p_ostream)
  : m_device(p_device), m_ostream(p_ostream) {

}

/*******************************************************************************
 * 
 ******************************************************************************/
template<typename DeviceT>
TransferT<DeviceT>::~TransferT() {

}

/*******************************************************************************
 * 
 ******************************************************************************/
template<typename DeviceT>
ssize_t
TransferT<DeviceT>::transfer(size_t p_bits, const uint8_t * const p_buffer, const size_t p_length) const {
    uint8_t * rxdata = NULL;
    size_t bitlen = p_length * 8, bufsz = p_length;
    ssize_t len = 0;

    if ((p_length > 0) && (p_bits > bitlen)) {
        bufsz = p_bits >> 3;
        if (p_bits % 8)
            bufsz += 1;

        rxdata = new uint8_t[bufsz];
        len = this->m_device->shift(bitlen, p_buffer, rxdata);
        if ((len > 0) && ((unsigned) len == bitlen))
            len += this->m_device->shift(p_bits - len, NULL, rxdata + (len >> 3));
    } else {
        rxdata = new uint8_t[bufsz];
        len = this->m_device->shift(p_bits, p_buffer, rxdata);
    }

    delete[] rxdata;
    return len;
}

}; /* namespace spi */
