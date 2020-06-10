/*-
 * $Copyright$
-*/

#ifndef _GPIO_PIN
#define _GPIO_PIN
#else
#error Foobar
#endif

#include <gpio/GpioPin.hpp>

#include <errno.h>
#include <stddef.h>

namespace gpio {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename EngineT>
constexpr int
PinT<EngineT>::set(const mode_t p_mode) const {
    int rc = 0;
    switch (p_mode) {
    case On:
        rc = this->m_engine->write(-1, (1 << this->m_pin), (1 << this->m_pin));
        break;
    case Off:
        rc = this->m_engine->write(0, (1 << this->m_pin), (1 << this->m_pin));
        break;
    case HiZ:
        rc = this->m_engine->write(0, 0, (1 << this->m_pin));
        break;
    default:
        rc = EINVAL;
    }
    return (rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename EngineT>
constexpr int
PinT<EngineT>::get(mode_t &p_mode) const {
    vector_t vector;

    int rc = this->m_engine->read(vector);
    if (!rc) {
        if ((vector & (1 << this->m_pin)) != 0) {
            p_mode = On;
        } else {
            p_mode = Off;
        }
    }

    return (rc);
}

} /* namespace gpio */
