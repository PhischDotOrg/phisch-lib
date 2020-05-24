/*-
 * $Copyright$
-*/

#ifndef _GPIO_PIN_H_045cf298_180d_4c11_871e_afdd1dadeda4
#define _GPIO_PIN_H_045cf298_180d_4c11_871e_afdd1dadeda4

#include <stddef.h>
#include <stdint.h>

#include <gpio/GpioEngine.hpp>

namespace gpio {

class Pin {
public:
    typedef enum mode_e {
        HiZ = -1,
        Off = 0,
        On  = 1
    } mode_t;
    
        Pin(const uint8_t p_pin) : m_pin(p_pin) {};
    ~Pin() {};

protected:
    const uint8_t m_pin;
};

/*******************************************************************************
 *
 ******************************************************************************/
namespace _Pin {
    template<typename T> struct VectorChoiceT {
        typedef typename T::vector_t vector_t;
    };
    template<> struct VectorChoiceT<unsigned> {
        typedef unsigned vector_t;
    };
} /* namespace _Pin */

template <typename EngineT = gpio::GpioEngine>
class PinT : public Pin {
private:
    EngineT * const m_engine;

    typedef typename EngineT::vector_t vector_t;

public:
    constexpr PinT(EngineT * const p_engine, const uint8_t p_pin)
      : Pin(p_pin), m_engine(p_engine) {

    }

    ~PinT() {

    }

    constexpr int set(const mode_t p_mode) const;
    constexpr int get(mode_t &p_mode) const;

    template<typename Mode_e, typename Termination_e, typename Function_e>
    constexpr int enable(Mode_e p_mode, const Termination_e p_termination, const Function_e p_function) const {
        return this->m_engine->enable(this->m_pin, p_mode, p_termination, p_function);
    };

    constexpr int disable(void) const {
        return this->m_engine->disable(this->m_pin);
    };
}; /* class PinT */

typedef PinT<> GpioPin;

} /* namespace gpio */

/*******************************************************************************
 *
 ******************************************************************************/
#if defined(GMOCK_FOUND) && defined(DEBUG_BUILD)

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
#endif /* defined(GMOCK_FOUND) && defined(DEBUG_BUILD) */

#include <gpio/GpioPin.cpp>

#endif /* _GPIO_PIN_H_045cf298_180d_4c11_871e_afdd1dadeda4 */
