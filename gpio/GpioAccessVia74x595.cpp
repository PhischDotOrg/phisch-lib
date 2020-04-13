/*-
 * $Copyright$
-*/

#ifndef _GPIO_ACCESS_74x595_CPP_6bb28ad3_4f7b_4ffa_87b4_7f53a28c0ad8
#define _GPIO_ACCESS_74x595_CPP_6bb28ad3_4f7b_4ffa_87b4_7f53a28c0ad8

#include "GpioAccessVia74x595.hpp"

#include <assert.h>
#include <errno.h>
#include <unistd.h>

namespace gpio {

/*******************************************************************************
 *
 ******************************************************************************/
template<typename ShiftRegisterT, typename vector_t>
int
GpioAccessVia74x595FundamentalVectorTypeT<ShiftRegisterT, vector_t>::write(vector_t p_value, vector_t p_output, vector_t p_mask) {
    /*
     * The Shift Register 74x595 doesn't support single Hi-Z output pins. Instead,
     * all output pins may be set to Hi-Z via the disable() method.
     */
    assert((p_output & p_mask) == p_mask);
    
    this->m_bitfield &= ~p_mask;
    this->m_bitfield |= (p_value & p_mask);

    return this->writeToHardware(sizeof(m_bitfield), &this->m_bitfield);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename ShiftRegisterT, typename vector_t>
int
GpioAccessVia74x595FundamentalVectorTypeT<ShiftRegisterT, vector_t>::read(vector_t &p_vector) const {
    p_vector = this->m_bitfield;

    return (0);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename ShiftRegisterT>
int
GpioAccessVia74x595BaseT<ShiftRegisterT>::reset(const bool p_enable) const {
    return this->m_shiftRegister.reset(p_enable);
};

/*******************************************************************************
 *
 ******************************************************************************/
template<typename ShiftRegisterT>
int
GpioAccessVia74x595BaseT<ShiftRegisterT>::enable(void) const {
    return this->m_shiftRegister.enable();
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename ShiftRegisterT>
int
GpioAccessVia74x595BaseT<ShiftRegisterT>::disable(void) const {
    return this->m_shiftRegister.disable();
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename ShiftRegisterT>
int
GpioAccessVia74x595BaseT<ShiftRegisterT>::writeToHardware(const size_t p_len, const void * const p_buffer) const {
    return this->m_shiftRegister.write(static_cast<const uint8_t *>(p_buffer), p_len);
}

} /* namespace gpio */

#endif /* _GPIO_ACCESS_74x595_CPP_6bb28ad3_4f7b_4ffa_87b4_7f53a28c0ad8 */
