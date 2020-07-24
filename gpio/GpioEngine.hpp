/*-
 * $Copyright$
-*/

#ifndef _GPIO_ENGINE_HPP_a7b29109_9d79_486c_98ea_8cf0df5b29d8
#define _GPIO_ENGINE_HPP_a7b29109_9d79_486c_98ea_8cf0df5b29d8

#include <stddef.h>
#include <stdint.h>

#include <gpio/GpioAccess.hpp>

namespace gpio {

#if !defined(DOXYGEN)
/*
 * Template Meta-Programming Example from Wikibooks, via URL below on July 13th, 2013:
 *   http://en.wikibooks.org/wiki/C%2B%2B_Programming/Templates/Template_Meta-Programming#Example:_Compile-time_.22If.22
 *
 * This code is used to determine the width of the parameters to the write() and
 * read() methods below.
 */
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

namespace Engine {
    template<typename T> struct VectorChoiceT {
        typedef typename if_<(T::m_width > sizeof(uint8_t) * 8),
            typename if_<(T::m_width > sizeof(uint16_t) * 8),
              typename if_<(T::m_width > sizeof(uint32_t) * 8),
                typename if_<(T::m_width > sizeof(uint64_t) * 8), void *, uint64_t>::m_result,
              uint32_t>::m_result,
            uint16_t>::m_result,
          uint8_t>::m_result vector_t;
    };

    template<> struct VectorChoiceT<void> {
        typedef unsigned vector_t;
    };
    
    template<typename T> struct WidthChoiceT {
        static const size_t m_width = T::m_width;
    };
    template<> struct WidthChoiceT<void> {
        static const size_t m_width = sizeof(unsigned);
    };
    
} /* namespace Engine */

#endif /* !defined(DOXYGEN) */

template<typename AccessT = gpio::GpioAccess>
class EngineT {
private:
    AccessT * const m_access;

public:
    using Mode_e = typename AccessT::Mode_e;
    using Termination_e = typename AccessT::Termination_e;
    using Function_e = typename AccessT::Function_e;

    /*!
     * @brief C'tor
     *
     * Public constructor. If \c p_access is \c NULL, the object's behavior is
     * undefined.
     *
     * @param p_access Pointer to actual GPIO engine implementation.
     */
    constexpr EngineT(AccessT * const p_access) : m_access(p_access) {};

    /*!
     * @brief Number of pins this engine instantiation can support.
     *
     * Directly depends on how many pins the underlying engine implementation
     * can support.
     */
    static const size_t m_width = Engine::WidthChoiceT<AccessT>::m_width;

    /*!
     * @brief Vector type for GPIO engine.
     * The width of the vector type is determined at compile time, depending on
     * the value of \c AccessT::m_width.
     *
     * The algorithm is this:
     * \code{.cpp}
     *  switch(m_width) {
     *  case <= 8:
     *    typedef uint8_t vector;
     *    break;
     *  case <= 16:
     *    typedef uint16_t vector;
     *    break;
     *  case <= 32:
     *    typedef uint32_t vector;
     *    break;
     *  case <= 64:
     *    typedef uint64_t vector;
     *    break;
     *  default:
     *    typedef void * vector;
     *    break;
     *  }
     * \endcode
     */
#if defined(DOXYGEN)
    typedef unsigned vector_t;
#else
    typedef typename Engine::VectorChoiceT<AccessT>::vector_t vector_t;
#endif /* defined(DOXYGEN) */

    /*!
     * @brief Write GPIO status to actual engine.
     *
     * Parameters are treated as bit-vectors. Writes \c p_value to those pins
     * where \c p_mask and \c p_output is true. Alters output-enable bit on
     * those pins where \c p_mask is true.
     *
     * The example code listed below will:
     * - Set-up Pin 0 as an output pin and set it to the \c gpio::On state.
     * - Set-up Pin 1 as an output pin and set it to the \c gpio::Lo state.
     * - Set-up Pin 2 as an input pin, effectively setting it to the \c gpio::HiZ state.
     * - Not alter any other pins.
     *
     * Example:
     * \code{.cpp}
     *   gpio::EngineT<typename AccessT>::write(0x01, 0x03, 0x07)
     * \endcode
     * 
     * @param p_value   Bit-vector [MSB:0] of output pin values.
     * @param p_output  Bit-vector [MSB:0] of output enable values.
     * @param p_mask    Bit-vector [MSB:0] that masks other two parameters.
     *
     */
    void write(EngineT::vector_t p_value, EngineT::vector_t p_output, EngineT::vector_t p_mask) const {
        this->m_access->write(p_value, p_output, p_mask);
    }

    /*!
     * @brief Read GPIO status from actual engine.
     *
     * Parameters are treated as bit-vectors. Reads pin values into \c p_vector
     * from underlying implementation. The implementation may or may not read
     * back output pins.
     *
     * @param[out] p_vector Pointer to Bit-vector [MSB:0] of input pin values.
     *
     */
    void read(EngineT::vector_t &p_vector) const {
        this->m_access->read(p_vector);
    }

    /*!
     * @brief Initializes GPIO engine.
     * 
     * Configures all pins as output pins, i.e. makes sure they are driven at
     * the Hi-Z state.
     *
     * @returns Zero on success.
     * @returns \c EIO in case of an error from the hardware.
     */
    int init(void) const {
        return this->m_access->write(0, 0, -1);
    }

    void
    enable(const uint8_t p_pin, Mode_e p_mode, const Termination_e p_termination) const {
        this->m_access->enable(p_pin, p_mode, p_termination);
    };

    void
    disable(const uint8_t p_pin) const {
        this->m_access->disable(p_pin);
    };

    template<typename EngineT>
    void
    selectAlternateFn(const uint8_t p_pin, const EngineT &p_engine) const {
        this->m_access->selectAlternateFn(p_pin, p_engine);
    };
}; /* class EngineT */

typedef EngineT< > GpioEngine;

}; /* namespace gpio */

#endif /* _GPIO_ENGINE_HPP_a7b29109_9d79_486c_98ea_8cf0df5b29d8 */
