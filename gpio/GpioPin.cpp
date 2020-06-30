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
constexpr void
PinT<EngineT>::set(const mode_t p_mode) const {
    switch (p_mode) {
    case On:
        this->m_engine->write(-1, (1 << this->m_pin), (1 << this->m_pin));
        break;
    case Off:
        this->m_engine->write(0, (1 << this->m_pin), (1 << this->m_pin));
        break;
    case HiZ:
        this->m_engine->write(0, 0, (1 << this->m_pin));
        break;
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename EngineT>
constexpr void
PinT<EngineT>::get(mode_t &p_mode) const {
    vector_t vector;

    this->m_engine->read(vector);
    if ((vector & (1 << this->m_pin)) != 0) {
        p_mode = On;
    } else {
        p_mode = Off;
    }
}

} /* namespace gpio */
