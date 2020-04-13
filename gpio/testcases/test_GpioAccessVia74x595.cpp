/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <gpio/GpioAccessVia74x595.hpp>
#include <gpio/GpioPin.hpp>
#include <devices/Sr74x595.hpp>

/*******************************************************************************
 *
 ******************************************************************************/
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::Pointee;

namespace gpio {

/*******************************************************************************
 *
 ******************************************************************************/
class GpioAccessVia74x595Test : public ::testing::Test {
protected:
    typedef GpioAccessVia74x595T<devices::Sr74x595Mock> GpioAccessVia74x595;

    GpioAccessVia74x595 *   m_access;

    devices::Sr74x595Mock   m_sr;

    void setBitfield(uint8_t p_bitfield) {
        this->m_access->m_bitfield = p_bitfield;
    }

public:
    GpioAccessVia74x595Test() : m_access(NULL) {
        m_access = new GpioAccessVia74x595(m_sr);
    };

    virtual ~GpioAccessVia74x595Test() {
        delete m_access;
    };
};

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioAccessVia74x595Test, CreateAndDelete) {
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioAccessVia74x595Test, Write) {
    const uint8_t outputMask = 0xa5;
    const uint8_t value = 0xC5;
//    const uint8_t expectedValue = value & outputMask;
    const size_t len = sizeof(value);

    InSequence s;
    EXPECT_CALL(this->m_sr, write(NotNull(), len)).Times(1).WillOnce(Return(0));

    int rc = this->m_access->write(value, outputMask, outputMask);
    EXPECT_EQ(0, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioAccessVia74x595Test, Read) {
    const uint8_t outputMask = 0xa5;
    const uint8_t expectedValue = 0xC5 & outputMask;
    uint8_t value;

    this->setBitfield(expectedValue);
    int rc = this->m_access->read(value);
    EXPECT_EQ(0, rc);
    EXPECT_EQ(expectedValue, value);
}

} /* namespace gpio */
