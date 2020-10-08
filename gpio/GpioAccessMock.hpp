/*-
 * $Copyright$
-*/

#ifndef _GPIO_ACCESS_MOCK_HPP_e648ca25_53d4_4daa_b693_34f01075a991
#define _GPIO_ACCESS_MOCK_HPP_e648ca25_53d4_4daa_b693_34f01075a991

#if defined(UNITTEST)

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gpio/GpioAccess.hpp>

namespace gpio {

/*******************************************************************************
 *
 ******************************************************************************/
class GpioAccessMock {
public:
    enum class Mode_e {
        e_Input, 
        e_Output, 
        e_Analog, 
        e_Alternate, 
    };

    enum class Termination_e {
        e_None,
    };

    enum class Function_e : uint32_t {
    };

    static constexpr size_t m_width = 8;

    MOCK_CONST_METHOD3(write, int(uint8_t p_value, uint8_t p_output, uint8_t p_mask));
    MOCK_CONST_METHOD1(read, int(uint8_t &p_vector));

    MOCK_CONST_METHOD3(enable, void(unsigned, Mode_e, Termination_e));
    MOCK_CONST_METHOD1(disable, void(unsigned));
};

} /* namespace gpio */

#else /* defined(UNITTEST) */
namespace gpio {
    class GpioAccessMock;
} /* namespace gpio */
#endif /* defined(UNITTEST) */

#endif /* _GPIO_ACCESS_MOCK_HPP_e648ca25_53d4_4daa_b693_34f01075a991 */
