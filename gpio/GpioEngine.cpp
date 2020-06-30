/*-
 * $Copyright$
-*/
#ifndef _GPIO_ENGINE_CPP_3a3e988d_b8e1_4e26_934b_5ddb36b6ae39
#define _GPIO_ENGINE_CPP_3a3e988d_b8e1_4e26_934b_5ddb36b6ae39

#include <gpio/GpioEngine.hpp>
#include <gpio/GpioAccess.hpp>

#include <errno.h>
#include <stddef.h>

namespace gpio {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
EngineT<AccessT>::EngineT(AccessT * const p_access) : m_access(p_access) {
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
EngineT<AccessT>::~EngineT() {
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
void
EngineT<AccessT>::write(EngineT::vector_t p_value, EngineT::vector_t p_output,
  EngineT::vector_t p_mask) const {
    this->m_access->write(p_value, p_output, p_mask);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
void
EngineT<AccessT>::read(EngineT::vector_t &p_value) const {
    this->m_access->read(p_value);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename AccessT>
int
EngineT<AccessT>::init(void) const {
    return this->m_access->write(0, 0, -1);
}

} /* namespace gpio */

#endif /* _GPIO_ENGINE_CPP_3a3e988d_b8e1_4e26_934b_5ddb36b6ae39 */
