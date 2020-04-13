/*-
 * $Copyright$
-*/

#include "spi/SpiDevice.hpp"
#include "spi/Spi.hpp"

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <stddef.h>

namespace spi {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
DeviceT<AccessT>::DeviceT(AccessT * const p_access, const uint8_t p_number /* = 0 */, const spi::Mode p_mode /* = SpiMode0 */)
  : m_access(p_access), m_number(p_number), m_mode(p_mode) {

}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
DeviceT<AccessT>::~DeviceT(void) {

}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
int
DeviceT<AccessT>::select(void) const {
    assert(m_access != NULL);
    return this->m_access->select(this->m_number);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
int
DeviceT<AccessT>::deselect(void) const {
    assert(m_access != NULL);
    return this->m_access->deselect(this->m_number);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
ssize_t
DeviceT<AccessT>::shift(const uint32_t p_bits, const uint8_t * const p_tx /* = NULL */, uint8_t * const p_rx /* = NULL */) const {
    int rc = -1;

    rc = spi::ShiftHelperT< AccessT::SHIFT_MAX_BITS == 0, AccessT>::shift(this->m_access, p_bits, p_tx, p_rx, this->m_mode);

    return rc;
}

} /* namespace spi */
