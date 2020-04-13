/*-
 * $Copyright$
-*/

#ifndef _GPIO_ACCESS_VIA_74X595_HPP_958b87ce_0f1a_4b3d_a826_0def3b2d3323
#define _GPIO_ACCESS_VIA_74X595_HPP_958b87ce_0f1a_4b3d_a826_0def3b2d3323

#include <stdint.h>
#include <stddef.h>

#if defined(DEBUG_BUILD) && defined(GMOCK_FOUND)
#include <gmock/gmock.h>
#endif /* defined(DEBUG_BUILD) && defined(GMOCK_FOUND) */

namespace gpio {

#if !defined(DOXYGEN)
/*
 * Template Meta-Programming Example from Wikibooks, via URL below on July 13th, 2013:
 *   http://en.wikibooks.org/wiki/C%2B%2B_Programming/Templates/Template_Meta-Programming#Example:_Compile-time_.22If.22
 *
 */
namespace AccessVia74x595T {
    
    template <bool Condition, typename TrueResult, typename FalseResult>
    struct if_;

    template <typename TrueResult, typename FalseResult>
    struct if_<true, TrueResult, FalseResult>
    {
      typedef TrueResult m_result;
    };

    template <typename TrueResult, typename FalseResult>
    struct if_<false, TrueResult, FalseResult>
    {
      typedef FalseResult m_result;
    };

    template<unsigned t_width> struct VectorChoiceT {
        typedef typename if_<(t_width > sizeof(uint8_t) * 8),
            typename if_<(t_width > sizeof(uint16_t) * 8),
              typename if_<(t_width > sizeof(uint32_t) * 8),
                typename if_<(t_width > sizeof(uint64_t) * 8), void *, uint64_t>::m_result,
              uint32_t>::m_result,
            uint16_t>::m_result,
          uint8_t>::m_result vector_t;
    };
} /* namespace AccessVia74x595T */

#endif /* !defined(DOXYGEN) */

/*******************************************************************************
 *
 ******************************************************************************/
template<typename ShiftRegisterT>
class GpioAccessVia74x595BaseT {
public:
    int reset(const bool p_reset) const;

    int enable(void) const;
    int disable(void) const;

protected:
    GpioAccessVia74x595BaseT(ShiftRegisterT &p_shiftRegister)
      : m_shiftRegister(p_shiftRegister) {
    }

    ~GpioAccessVia74x595BaseT(void) {
    }
    
    ShiftRegisterT &    m_shiftRegister;

    int writeToHardware(const size_t p_len, const void * const p_buffer) const;    
    
private:
};

/*******************************************************************************
 *
 ******************************************************************************/
template<typename ShiftRegisterT, typename vector_t>
class GpioAccessVia74x595FundamentalVectorTypeT : public GpioAccessVia74x595BaseT<ShiftRegisterT> {
    friend class GpioAccessVia74x595Test;

public:
    static const size_t m_width = sizeof(vector_t) * 8;

    int write(vector_t p_value, vector_t p_output, vector_t p_mask);
    int read(vector_t &p_vector) const;

protected:
    GpioAccessVia74x595FundamentalVectorTypeT(ShiftRegisterT &p_shiftRegister)
      : GpioAccessVia74x595BaseT<ShiftRegisterT>(p_shiftRegister), m_bitfield(0) {
          
    }

    ~GpioAccessVia74x595FundamentalVectorTypeT(void) {

    }
    
private:
    vector_t m_bitfield;
};

/*******************************************************************************
 *
 ******************************************************************************/
template<typename ShiftRegisterT, unsigned t_width = 8>
class GpioAccessVia74x595T : public GpioAccessVia74x595FundamentalVectorTypeT<ShiftRegisterT, typename AccessVia74x595T::VectorChoiceT<t_width>::vector_t > {
public:
    GpioAccessVia74x595T(ShiftRegisterT &p_shiftRegister)
      : GpioAccessVia74x595FundamentalVectorTypeT<ShiftRegisterT, typename AccessVia74x595T::VectorChoiceT<t_width>::vector_t >(p_shiftRegister) {
    }

    ~GpioAccessVia74x595T(void) {
        
    }
};

} /* namespace gpio */

#endif /* _GPIO_ACCESS_VIA_74X595_HPP_958b87ce_0f1a_4b3d_a826_0def3b2d3323 */
