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
#include <gpio/GpioAccessViaSTM32F4.hpp>
#include <gpio/GpioAccessVia74x595.hpp>

/*******************************************************************************
 *
 ******************************************************************************/
namespace gpio {

typedef enum GpioAccessImpl_e {
    Undefined,
    SR74x595,
    STM32F4,
    Lpt,
    GTest
} GpioAccessImpl_t;

template<GpioAccessImpl_t> struct GpioAccessChoice;

template<> struct GpioAccessChoice<Undefined> {
    typedef void m_type;
};

class GpioAccessViaSTM32F4;
template<> struct GpioAccessChoice<STM32F4> {
    typedef GpioAccessViaSTM32F4 m_type;
};

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
