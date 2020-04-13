/*-
 * $Copyright$
-*/

#include <gpio/GpioPin.hpp>
#include <gpio/GpioEngine.cpp>

#include <errno.h>
#include <stddef.h>

namespace gpio {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename EngineT>
PinT<EngineT>::PinT(EngineT * const p_engine, const uint8_t p_pin)
  : Pin(p_pin), m_engine(p_engine) {

}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename EngineT>
PinT<EngineT>::~PinT() {

}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename EngineT>
int
PinT<EngineT>::set(const mode_t p_mode) const {
    int rc;
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
        goto out;
    }
    if (rc != 0)
        rc = EIO;
out:
    return (rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename EngineT>
int
PinT<EngineT>::get(mode_t &p_mode) const {
    vector_t vector;

    int rc = this->m_engine->read(vector);
    if (rc != 0) {
        rc = EIO;
        goto out;
    }

    if ((vector & (1 << this->m_pin)) != 0)
        p_mode = On;
    else
        p_mode = Off;
out:
    return (rc);
}

} /* namespace gpio */
