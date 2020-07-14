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
class GpioEngineMock {
public:
    static const size_t m_width = 4;

    typedef unsigned vector_t;

    MOCK_CONST_METHOD3(write, int(unsigned, unsigned, unsigned));
    MOCK_CONST_METHOD1(read, int(unsigned &));
};

/*******************************************************************************
 *
 ******************************************************************************/
class GpioPinTest : public ::testing::Test {
protected:
    GpioEngineMock m_mock;
    gpio::PinT<GpioEngineMock> *m_pin;

    static const int m_number = 3;

public:
    GpioPinTest() : m_pin(NULL) {
        m_pin = new PinT<GpioEngineMock>(&m_mock, m_number);
    };

    virtual ~GpioPinTest() {
        delete m_pin;
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
    m_pin->set(gpio::Pin::On);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioPinTest, SetLo) {
    EXPECT_CALL(m_mock, write(0, (1 << m_number), (1 << m_number))).WillOnce(Return(0));
    m_pin->set(gpio::Pin::Off);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioPinTest, SetZ) {
    EXPECT_CALL(m_mock, write(_, (0 << m_number), (1 << m_number))).WillOnce(Return(0));
    m_pin->set(gpio::Pin::HiZ);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioPinTest, SetError) {
    EXPECT_CALL(m_mock, write((0 << m_number), (1 << m_number), (1 << m_number))).WillOnce(Return(EIO));
    m_pin->set(gpio::Pin::Off);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioPinTest, GetOn) {
    EXPECT_CALL(m_mock, read(_)).WillOnce(DoAll(SetArgReferee<0>((1 << m_number)), Return(0)));

    gpio::Pin::mode_t mode;
    m_pin->get(mode);

    EXPECT_EQ(gpio::Pin::On, mode);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioPinTest, GetOff) {
    EXPECT_CALL(m_mock, read(_)).WillOnce(DoAll(SetArgReferee<0>(0xFF & ~(1 << m_number)), Return(0)));

    gpio::Pin::mode_t mode;
    m_pin->get(mode);

    EXPECT_EQ(gpio::Pin::Off, mode);
}

} /* namespace gpio */
