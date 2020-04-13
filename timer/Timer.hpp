/*-
 * $Copyright$
-*/

#ifndef _TIMER_HPP_8ea449ec_4acd_4181_95f9_dce70e61ecd7
#define _TIMER_HPP_8ea449ec_4acd_4181_95f9_dce70e61ecd7

#if defined(DEBUG_BUILD) && defined(GMOCK_FOUND)
#include <gmock/gmock.h>
#endif /* defined(DEBUG_BUILD) && defined(GMOCK_FOUND) */

/*******************************************************************************
 * 
 ******************************************************************************/
#if !defined(TIMER_ENV)
#define TIMER_ENV Undefined
#endif

#include <timer/TimerViaSTM32F4.hpp>

/*******************************************************************************
 * 
 ******************************************************************************/
namespace timer {

typedef enum TimerImpl_e {
    STM32F4,
    Undefined,
} TimerImpl_t;

template<TimerImpl_t> struct TimerChoice;

template<> struct TimerChoice<STM32F4> {
    typedef TimerViaSTM32F4 m_type;
};

template<> struct TimerChoice<Undefined> {
    typedef void m_type;
};

struct TimerChoiceT {
    typedef TimerChoice<TIMER_ENV>::m_type m_type;
};

typedef TimerChoiceT::m_type Timer;

} /* namespace timer */

#endif /* _TIMER_HPP_8ea449ec_4acd_4181_95f9_dce70e61ecd7 */
