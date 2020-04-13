/*-
 * $Copyright$
-*/

#ifndef __UART_ACCESS_HPP_079599c5_2be2_478d_8315_cc5566d29746
#define __UART_ACCESS_HPP_079599c5_2be2_478d_8315_cc5566d29746

#include <uart/Uart.hpp>

/*******************************************************************************
 * 
 ******************************************************************************/
#if !defined(UART_ENV)
#define UART_ENV Undefined
#endif

#include "uart/UartAccessDarwin.hpp"
#include "uart/UartAccessLinux.hpp"
#include "uart/UartAccessMock.hpp"
#include "uart/UartAccessViaSTM32F4.hpp"

namespace uart {

typedef enum UartAccessImpl_e {
    Undefined,
    Darwin,
    Linux,
    STM32F4,
    GTest
} UartAccessImpl_t;

template<UartAccessImpl_t> struct UartAccessChoice;

class UartAccessMock;

template<> struct UartAccessChoice<Undefined> {
    typedef void m_type;
};

template<> struct UartAccessChoice<Darwin> {
    typedef UartAccessDarwin m_type;
};

template<> struct UartAccessChoice<Linux> {
    typedef UartAccessLinux m_type;
};

template<> struct UartAccessChoice<STM32F4> {
    typedef UartAccessViaSTM32F4 m_type;
};

template<> struct UartAccessChoice<GTest> {
    typedef UartAccessMock m_type;
};

struct UartAccessT {
    typedef UartAccessChoice<UART_ENV>::m_type m_type;
};

typedef UartAccessT::m_type UartAccess;

}; /* namespace uart */

#include <uart/UartAccess.cpp>

#endif /* __UART_ACCESS_HPP_77b2f871_582a_4028_8515_7686d0161fbb */
