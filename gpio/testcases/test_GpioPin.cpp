/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <gpio/GpioPin.hpp>

using ::testing::_;
using ::testing::Return;
using ::testing::_;
using ::testing::DoAll;
using ::testing::SetArgReferee;

namespace gpio {

/*******************************************************************************
 *
 ******************************************************************************/
class GpioPinTest : public ::testing::Test {
    using Engine = gpio::EngineT<gpio::GpioAccessMock>;
    using Pin = GenericPinT<gpio::PinPolicy::DigitalOut, gpio::PinPolicy::Termination_e::e_None, Engine>;

protected:
    GpioAccessMock  m_mock;
    Engine          m_engine;
    Pin             m_pin;

    static constexpr int m_number = 3;

public:
    GpioPinTest() : m_engine(&m_mock), m_pin(m_engine, m_number) {

    };

    virtual ~GpioPinTest() {

    };
};

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioPinTest, CreateAndDelete) {

}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioPinTest, SetHi) {
    EXPECT_CALL(m_mock, write(-1, (1 << m_number), (1 << m_number))).WillOnce(Return(0));
    m_pin.set(true);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioPinTest, SetLo) {
    EXPECT_CALL(m_mock, write(0, (1 << m_number), (1 << m_number))).WillOnce(Return(0));
    m_pin.set(false);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioPinTest, SetError) {
    EXPECT_CALL(m_mock, write((0 << m_number), (1 << m_number), (1 << m_number))).WillOnce(Return(EIO));
    m_pin.set(false);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioPinTest, GetOn) {
    EXPECT_CALL(m_mock, read(_)).WillOnce(DoAll(SetArgReferee<0>((1 << m_number)), Return(0)));

    EXPECT_TRUE(m_pin.get());
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioPinTest, GetOff) {
    EXPECT_CALL(m_mock, read(_)).WillOnce(DoAll(SetArgReferee<0>(0xFF & ~(1 << m_number)), Return(0)));

    EXPECT_FALSE(m_pin.get());
}

} /* namespace gpio */
