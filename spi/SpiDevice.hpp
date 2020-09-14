/*-
 * $Copyright$
-*/

#ifndef __SPIDEVICE_HPP_3ae18ccb_a3d7_4367_a9e0_4d1978197f47
#define __SPIDEVICE_HPP_3ae18ccb_a3d7_4367_a9e0_4d1978197f47

#include "spi/SpiAccess.hpp"
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
class DeviceT {
public:
    DeviceT(AccessT * const p_access, const uint8_t p_number = 0, const spi::Mode p_mode = SpiMode0)
      : m_access(p_access), m_number(p_number), m_mode(p_mode) {

    }

    int select(void) const {
        assert(m_access != NULL);
        return this->m_access->select(this->m_number);
    }

    int deselect(void) const {
        assert(m_access != NULL);
        return this->m_access->deselect(this->m_number);
    }

    ssize_t shift(const uint32_t p_bits, const uint8_t * const p_tx = NULL, uint8_t * const p_rx = NULL) const {
        int rc = -1;

        rc = spi::ShiftHelperT< AccessT::SHIFT_MAX_BITS == 0, AccessT>::shift(this->m_access, p_bits, p_tx, p_rx, this->m_mode);

        return rc;
    }

protected:
    AccessT * const m_access;
    const uint8_t   m_number;
    const spi::Mode m_mode;
};

typedef DeviceT<spi::SpiAccess> SpiDevice;

} /* namespace spi */

#if defined(UNITTEST)
#include <gmock/gmock.h>

namespace spi {


/*******************************************************************************
 * 
 ******************************************************************************/
class SpiAccessMock {
public:
    MOCK_CONST_METHOD1(select, int(const uint8_t p_device));
    MOCK_CONST_METHOD1(deselect, int(const uint8_t p_device));
    MOCK_CONST_METHOD4(shift, int(const uint8_t p_bits, uint8_t * const p_rx, const uint8_t p_tx, const spi::Mode p_mode));

    static const unsigned SHIFT_MAX_BITS = 8;
};

/*******************************************************************************
 *
 ******************************************************************************/
class SpiDeviceMock {
public:
    MOCK_CONST_METHOD0(select, int());
    MOCK_CONST_METHOD0(deselect, int());
    MOCK_CONST_METHOD3(shift, ssize_t(const uint32_t p_bits, const uint8_t * const p_tx, uint8_t * const p_rx));
};

} /* namespace spi */

#endif /* defined(UNITTEST) */

#endif /* __SPIDEVICE_HPP_3ae18ccb_a3d7_4367_a9e0_4d1978197f47 */
