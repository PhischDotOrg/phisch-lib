/*-
 * $Copyright$
-*/

#include "gpio/GpioAccessViaLpt.hpp"
#include "lpt/LptAccess.hpp"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>

namespace gpio {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename LptT>
AccessViaLptT<LptT>::AccessViaLptT(LptT * const p_lpt) : m_lpt(p_lpt) {
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename LptT>
AccessViaLptT<LptT>::~AccessViaLptT() {
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename LptT>
int
AccessViaLptT<LptT>::write(uint8_t p_value, uint8_t p_output,
  uint8_t p_mask) const {
    int rc = -1;
    uint8_t data;

    assert(m_lpt != NULL);

    rc = this->m_lpt->getData(&data);
    if (rc) {
        rc = EIO;
        goto out;
    }

    data ^= 0xF0;
    data &= ~((p_mask << 4) & 0xF0);
    data |= ((p_output & p_mask) << 4) & 0xF0;
    data ^= 0xF0;

    data &= 0xF0 | (~p_mask & 0xF);
    data |= (p_value & p_mask) & 0x0F;

    rc = this->m_lpt->setData(data);
    if (rc) {
        rc = EIO;
        goto out;
    }

    rc = 0;
out:
    return (rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename LptT>
int
AccessViaLptT<LptT>::read(uint8_t &p_value) const {
    int rc = -1;
    uint8_t status;

    assert(m_lpt != NULL);

    rc = this->m_lpt->getStatus(&status);
    if (!rc) {
        status >>= 4;
        status &= 0xF;
        status ^= 0x8;

        p_value = status;
    }

    return (rc);
}

} /* namespace gpio */
