/*-
 * $Copyright$
-*/

#ifndef _GPIO_ACCESS_HPP_15f2273a_caed_4203_a6f1_0b499584a266
#define _GPIO_ACCESS_HPP_15f2273a_caed_4203_a6f1_0b499584a266

#include <type_traits>

/*******************************************************************************
 *
 ******************************************************************************/
#if !defined(GPIO_ENV)
#error GPIO_ENV is not defined!
#define GPIO_ENV Undefined
#endif

/*******************************************************************************
 *
 ******************************************************************************/
namespace gpio {
class GpioAccessInterface {
public:
    static const size_t m_width = 32;

    virtual int write(uint8_t p_value, uint8_t p_output, uint8_t p_mask) const = 0;
    virtual int read(uint8_t &p_vector) const = 0;
}; /* class GpioAccessInterface */
} /* namespace gpio */

/*******************************************************************************
 *
 ******************************************************************************/
#if defined(HAVE_LPT)
#include <lpt/LptAccess.hpp>
#endif /* defined(HAVE_LPT) */

#include <gpio/GpioAccessMock.hpp>
#include <gpio/GpioAccessViaLpt.hpp>
#include <gpio/GpioAccessVia74x595.hpp>

#if defined(HAVE_STM32)
#include <stm32/Cpu.hpp>
#endif /* defined(HAVE_STM32) */

/*******************************************************************************
 *
 ******************************************************************************/
namespace gpio {

typedef enum GpioAccessImpl_e {
    Undefined,
    SR74x595,
    STM32,
    Lpt,
    GTest
} GpioAccessImpl_t;

template<GpioAccessImpl_t> struct GpioAccessChoice;

template<> struct GpioAccessChoice<Undefined> {
    typedef void m_type;
};

#if defined(HAVE_STM32)
template<> struct GpioAccessChoice<STM32> {
    typedef stm32::GpioEngine m_type;
};
#endif /* defined(HAVE_STM32) */

#if defined(HAVE_LPT)
template<> struct GpioAccessChoice<Lpt> {
    typedef class AccessViaLptT< typename lpt::LptAccess > m_type;
}
#endif /* defined(HAVE_LPT) */

template<> struct GpioAccessChoice<GTest> {
    typedef class GpioAccessMock m_type;
};

struct GpioAccessT {
    typedef GpioAccessChoice<GPIO_ENV>::m_type m_type;
};

typedef GpioAccessT::m_type GpioAccess;

} /* namespace gpio */

#endif /* _GPIO_ACCESS_HPP_15f2273a_caed_4203_a6f1_0b499584a266 */
