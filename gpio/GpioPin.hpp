/*-
 * $Copyright$
-*/

#ifndef _GPIO_PIN_H_045cf298_180d_4c11_871e_afdd1dadeda4
#define _GPIO_PIN_H_045cf298_180d_4c11_871e_afdd1dadeda4

#include <stddef.h>
#include <stdint.h>

#include <gpio/GpioEngine.hpp>

namespace gpio {

/******************************************************************************/
struct PinPolicy {
/******************************************************************************/

/******************************************************************************/
using Mode_e = gpio::GpioEngine::Mode_e;
using Termination_e = gpio::GpioEngine::Termination_e;
/******************************************************************************/

/******************************************************************************/
template<
    typename GpioEngineT = gpio::GpioEngine
>
class AnalogInputT {
public:
    static constexpr gpio::PinPolicy::Mode_e m_mode = gpio::PinPolicy::Mode_e::e_Analog;
}; /* class AnalogInputT */
using AnalogIn = AnalogInputT<>;

static_assert(sizeof(AnalogIn) == 1u);
/******************************************************************************/

/******************************************************************************/
template<
    typename GpioEngineT = gpio::GpioEngine
>
class DigitalInputT {
public:
    static constexpr gpio::PinPolicy::Mode_e m_mode = gpio::PinPolicy::Mode_e::e_Input;

    static constexpr
    bool
    get(const GpioEngineT &p_engine, unsigned p_pin) {
        typename GpioEngineT::vector_t vector;

        p_engine.read(vector);

        return ((vector & (1 << p_pin)) != 0);
    }
}; /* class DigitalInputT */
using DigitalIn = DigitalInputT<>;

static_assert(sizeof(DigitalIn) == 1u);
/******************************************************************************/

/******************************************************************************/
template<
    typename GpioEngineT = gpio::GpioEngine
>
class DigitalOutputT : public DigitalInputT<GpioEngineT> {
public:
    static constexpr gpio::PinPolicy::Mode_e m_mode = gpio::PinPolicy::Mode_e::e_Output;

    static constexpr
    void
    set(const GpioEngineT &p_engine, unsigned p_pin, bool p_mode) {
        p_engine.write(p_mode ? -1 : 0, (1 << p_pin), (1 << p_pin));
    }
}; /* class DigitalOutputT */
using DigitalOut = DigitalOutputT<>;

static_assert(sizeof(DigitalOut) == 1u);
/******************************************************************************/

/******************************************************************************/
template<
    typename GpioEngineT = gpio::GpioEngine
>
class AlternateFnInputT : public DigitalOutputT<GpioEngineT> {
public:
    static constexpr gpio::PinPolicy::Mode_e m_mode = gpio::PinPolicy::Mode_e::e_Input;

    template<typename EngineT>
    static constexpr void
    selectAlternateFn(const GpioEngineT &p_gpioEngine, unsigned p_pin, const EngineT &p_engine) {
        static_assert(sizeof(AlternateFnInputT) == 1u);
        p_gpioEngine.selectAlternateFn(p_pin, p_engine);
    }
}; /* class AnalogInputT */
using AlternateFnInput = AlternateFnInputT<>;
/******************************************************************************/

/******************************************************************************/
template<
    typename GpioEngineT = gpio::GpioEngine
>
class AlternateFnOutputT : public AlternateFnInputT<GpioEngineT> {
public:
    static constexpr gpio::PinPolicy::Mode_e m_mode = gpio::PinPolicy::Mode_e::e_Alternate;
}; /* class AnalogInputT */
using AlternateFnOutput = AlternateFnOutputT<>;
/******************************************************************************/

/******************************************************************************/
}; /* struct PinPoliy */
/******************************************************************************/

/******************************************************************************/
template<
    typename ReadWritePolicyT,              /* DigitalInput, DigitalOutput, AlternateOutput, AnalogInput */
    PinPolicy::Termination_e TerminationT,  /* None, PullUp, PullDown */
    typename GpioEngineT = gpio::GpioEngine
>
class GenericPinT : public ReadWritePolicyT {
    const GpioEngineT & m_engine;
    const uint8_t       m_pin;

public:
    enum class State_e : bool {
        e_Off   = false,
        e_On    = true
    };

    constexpr GenericPinT(const GpioEngineT &p_engine, const uint8_t p_pin)
      : m_engine(p_engine), m_pin(p_pin) {
          enable();
    }

    ~GenericPinT() {
        disable();
    }

    bool
    get(void) const {
        return ReadWritePolicyT::get(this->m_engine, this->m_pin);
    }

    void
    set(bool p_mode) const {
        ReadWritePolicyT::set(this->m_engine, this->m_pin, p_mode);
    }

    void
    enable(void) const {
        /* TODO Maybe this can be re-worked so things can be checked at compile time */
        m_engine.enable(this->m_pin, ReadWritePolicyT::m_mode, TerminationT);
    };

    void
    disable(void) const {
        m_engine.disable(this->m_pin);
    };

    template<typename EngineT>
    void
    selectAlternateFn(const EngineT &p_engineFn) const {
        ReadWritePolicyT::selectAlternateFn(this->m_engine, this->m_pin, p_engineFn);
    }
}; /* class GenericPinT */

using GpioPin = GenericPinT<gpio::PinPolicy::DigitalOut, gpio::PinPolicy::Termination_e::e_None, gpio::GpioEngine>;
using DigitalOutPin  = GenericPinT<gpio::PinPolicy::DigitalOut, gpio::PinPolicy::Termination_e::e_None, gpio::GpioEngine>;
template<
    gpio::PinPolicy::Termination_e  TerminationT
>
using DigitalInPinT  = GenericPinT<gpio::PinPolicy::DigitalIn, TerminationT, gpio::GpioEngine>;
using DigitalInPin   = GenericPinT<gpio::PinPolicy::DigitalIn, gpio::PinPolicy::Termination_e::e_None, gpio::GpioEngine>;
using AlternateFnPin = GenericPinT<gpio::PinPolicy::AlternateFnOutput, gpio::PinPolicy::Termination_e::e_None, gpio::GpioEngine>;
template<
    gpio::PinPolicy::Termination_e  TerminationT
>
using AlternateFnInPinT = GenericPinT<gpio::PinPolicy::AlternateFnInput, TerminationT, gpio::GpioEngine>;
using AnalogInputPin = GenericPinT<gpio::PinPolicy::AnalogIn, gpio::PinPolicy::Termination_e::e_None, gpio::GpioEngine>;

/*
 * Ensure the Policy Classes for the GenericPinT class don't add to the object size. Allow for a bit of
 * padding from the compiler since member variable 'm_pin' is a uint8_t.
 */
static_assert(sizeof(GpioPin) <= 2 * sizeof(void *));
/******************************************************************************/

/******************************************************************************/
} /* namespace gpio */
/******************************************************************************/

/*******************************************************************************
 *
 ******************************************************************************/
#if defined(UNITTEST)

#include <gmock/gmock.h>

namespace gpio {
class GpioPinMock : public Pin {
private:
    std::string m_name;
    std::ostream * const m_ostream;

    mutable mode_t m_termination;
    mutable mode_t m_value;

public:
    GpioPinMock(std::ostream * const p_ostream = NULL, const std::string &p_name = "Default", mode_t p_termination = HiZ)
      : Pin(-1), m_name(p_name), m_ostream(p_ostream), m_termination(p_termination), m_value(HiZ) {
    }

    int set(const mode_t p_mode) const {
        volatile bool change = false;

        if (p_mode == HiZ) {
            change = m_value != m_termination;
            m_value = m_termination;
        } else {
            change = m_value != p_mode;
            m_value = p_mode;
        }

        if (m_ostream != NULL) {
            *m_ostream << "Set " << m_name << " => " << (m_value == Off ? "L" : (m_value == On ? "H" : "Z" ));
            if (change) {
                *m_ostream << " Change!";
            }
            *m_ostream << std::endl;
        }
        return this->_set(p_mode);
    }
    MOCK_CONST_METHOD1(_set, int(const mode_t p_mode));

    int get(mode_t &p_mode) const {
        int rc = this->_get(p_mode);

#if 0
        change = p_mode != m_value;
        m_value = p_mode;

        if (m_ostream != NULL) {
            *m_ostream << "Get " << m_name << " => " << (m_value == Off ? "L" : (m_value == On ? "H" : "Z" ));
            if (change) {
                *m_ostream << " Change!";
            }
            *m_ostream << std::endl;
        }
#endif

        return rc;
    }
    MOCK_CONST_METHOD1(_get, int(mode_t &p_mode));
}; /* GpioPinMock */

} /* namespace gpio */
#endif /* defined(UNITTEST) */

#endif /* _GPIO_PIN_H_045cf298_180d_4c11_871e_afdd1dadeda4 */
