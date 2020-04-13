/*-
 * $Copyright$
-*/

#ifndef _ADC_HPP_3aa14966_eec4_4be7_acea_4bb835185671
#define _ADC_HPP_3aa14966_eec4_4be7_acea_4bb835185671

#if defined(DEBUG_BUILD) && defined(GMOCK_FOUND)
#include <gmock/gmock.h>
#endif /* defined(DEBUG_BUILD) && defined(GMOCK_FOUND) */

/*******************************************************************************
 * 
 ******************************************************************************/
#if !defined(ADC_ENV)
#define ADC_ENV Undefined
#endif

#include <devices/AdcViaSTM32F4.hpp>

/*******************************************************************************
 * 
 ******************************************************************************/
namespace devices {

typedef enum AdcImpl_e {
    STM32F4,
    Undefined,
} AdcImpl_t;

template<AdcImpl_t> struct AdcChoice;

template<> struct AdcChoice<STM32F4> {
    typedef AdcViaSTM32F4 m_type;
};

template<> struct AdcChoice<Undefined> {
    typedef void m_type;
};

struct AdcChoiceT {
    typedef AdcChoice<ADC_ENV>::m_type m_type;
};

typedef AdcChoiceT::m_type Adc;

} /* namespace devices */

#endif /* _ADC_HPP_3aa14966_eec4_4be7_acea_4bb835185671 */
