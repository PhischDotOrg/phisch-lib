/*-
 * $Copyright$
-*/

#ifndef _SR_74x595_CPP_f28bd4ae_1bb2_476e_b609_7f9ac35ac675
#define _SR_74x595_CPP_f28bd4ae_1bb2_476e_b609_7f9ac35ac675

#include <sys/types.h>

#include "Sr74x595.hpp"

namespace devices {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename SpiDeviceT, typename PinT>
int
Sr74x595T<SpiDeviceT, PinT>::write(const void * const p_buffer, const size_t p_len) const {
    const ssize_t len = p_len * 8;
    int rc;

    /*
     * Pulse width on the chip select line must be at least 16ns. The guaranteed
     * to work shift frequency is 30MHz at 4.5V. Data is latched from the shift
     * register onto the output pins on the positive edge of the chip select
     * line. Doing the select before the shift operation seems to work and looks
     * like an easy way of ensuring the minimum pulse width.
     */
    rc = this->m_spiDevice.select();
    if (rc) {
        goto out;
    }

    rc = this->m_spiDevice.shift(len, static_cast<const uint8_t * const>(p_buffer), NULL);
    if (rc != len) {
        rc = ENXIO;
        goto out;
    }

    rc = this->m_spiDevice.deselect();
out:
    return (rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename SpiDeviceT, typename PinT>
int
Sr74x595T<SpiDeviceT, PinT>::reset(const bool p_enable) const {
    return this->setFunctionPin(this->m_reset, !p_enable);
};

/*******************************************************************************
 *
 ******************************************************************************/
template<typename SpiDeviceT, typename PinT>
int
Sr74x595T<SpiDeviceT, PinT>::setFunctionPin(const PinT * const p_pin, const bool p_enable) const {
    int rc;

    if (p_pin != NULL) {
        rc = p_pin->set(p_enable ? PinT::On : PinT::Off);
    } else {
        rc = ENXIO;
    }

    return rc;
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename SpiDeviceT, typename PinT>
int
Sr74x595T<SpiDeviceT, PinT>::enable(const bool p_enable) const {
    return this->setFunctionPin(this->m_enable, !p_enable);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename SpiDeviceT, typename PinT>
int
Sr74x595T<SpiDeviceT, PinT>::enable(void) const {
    return this->enable(true);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename SpiDeviceT, typename PinT>
int
Sr74x595T<SpiDeviceT, PinT>::disable(void) const {
    return this->enable(false);
}
} /* namespace devices */

#endif /* _SR_74x595_CPP_f28bd4ae_1bb2_476e_b609_7f9ac35ac675 */

