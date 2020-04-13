/*-
 * $Copyright$
-*/

#ifndef _GPIO_ACCESS_MOCK_HPP_e648ca25_53d4_4daa_b693_34f01075a991
#define _GPIO_ACCESS_MOCK_HPP_e648ca25_53d4_4daa_b693_34f01075a991

#if defined(DEBUG_BUILD) && defined(GMOCK_FOUND)

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gpio/GpioAccess.hpp>

namespace gpio {

/*******************************************************************************
 *
 ******************************************************************************/
class GpioAccessMock : public GpioAccessInterface {
public:
    static const size_t m_width = 8;

    MOCK_CONST_METHOD3(write, int(uint8_t p_value, uint8_t p_output, uint8_t p_mask));
    MOCK_CONST_METHOD1(read, int(uint8_t &p_vector));
};

} /* namespace gpio */
#else /* defined(DEBUG_BUILD) && defined(GMOCK_FOUND) */
namespace gpio {
    class GpioAccessMock;
} /* namespace gpio */
#endif /* defined(DEBUG_BUILD) && defined(GMOCK_FOUND) */

#endif /* _GPIO_ACCESS_MOCK_HPP_e648ca25_53d4_4daa_b693_34f01075a991 */
