/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <spi/SpiAccessViaGpio.hpp>
#include <spi/SpiAccessViaGpio.cpp>

#include <gpio/GpioPin.hpp>

#include <errno.h>

using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgReferee;
using ::testing::InSequence;

MATCHER_P2(IsEqualBuffer, p_buffer, p_len, "") {
    if (arg == NULL && p_buffer != NULL)
        return false;

    return (bcmp(arg, p_buffer, p_len) == 0);
};

namespace spi {

/*******************************************************************************
 * 
 ******************************************************************************/
class SpiAccessViaGpioTest : public ::testing::Test {
protected:
    gpio::GpioPinMock   m_sclk;
    gpio::GpioPinMock   m_nsel;
    gpio::GpioPinMock   m_mosi;
    gpio::GpioPinMock   m_miso;

    spi::AccessViaGpioT<gpio::GpioPinMock> * m_access;
public:
    SpiAccessViaGpioTest() : m_sclk(&std::cout, "SCLK"), m_mosi(&std::cout, "MOSI"), m_access(NULL) {
        m_access = new spi::AccessViaGpioT<gpio::GpioPinMock>(&m_sclk, &m_nsel, &m_mosi, &m_miso);
    };

    virtual ~SpiAccessViaGpioTest() {
        delete m_access;
    };
};

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaGpioTest, CreateAndDelete) {

}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(SpiAccessViaGpioTest, Select) {
    EXPECT_CALL(m_nsel, _set(gpio::Pin::Off));
    int rc = m_access->select(0);
    EXPECT_EQ(0, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(SpiAccessViaGpioTest, Deselect) {
    EXPECT_CALL(m_nsel, _set(gpio::Pin::On));
    int rc = m_access->deselect(0);
    EXPECT_EQ(0, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(SpiAccessViaGpioTest, ShiftWithToManyBits) {
    size_t bitlen = 9;

    int rc = m_access->shift(bitlen);
    EXPECT_EQ(EINVAL, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(SpiAccessViaGpioTest, ShiftMode0) {
    uint8_t rx = 0;
    const uint8_t tx = 0xa6;
    size_t bitlen = sizeof(tx) * 8;

    InSequence s;
    for (unsigned i = 0; i < bitlen; i++) {
        EXPECT_CALL(m_sclk, _set(gpio::Pin::Off)).Times(1).WillOnce(Return(0));
        EXPECT_CALL(m_mosi, _set(((tx >> (7 - i)) & 1) ? gpio::Pin::On : gpio::Pin::Off)).Times(1).WillOnce(Return(0));
        EXPECT_CALL(m_sclk, _set(gpio::Pin::On)).Times(1).WillOnce(Return(0));
        EXPECT_CALL(m_miso, _get(_)).Times(1).WillOnce(
          DoAll(
            SetArgReferee<0>(((tx >> (7 - i)) & 1) ? gpio::Pin::On : gpio::Pin::Off),
            Return(0)
          )
        );
        EXPECT_CALL(m_sclk, _set(gpio::Pin::Off)).Times(1).WillOnce(Return(0));
    }

    int rc = m_access->shift(bitlen, &rx, tx, spi::SpiMode0);
    EXPECT_EQ(0, rc);
    EXPECT_EQ(rx, tx);
}

}; /* namespace spi */
