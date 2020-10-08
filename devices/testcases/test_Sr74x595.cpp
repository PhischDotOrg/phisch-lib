/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <devices/Sr74x595.hpp>
#include <spi/SpiDevice.hpp>
#include <gpio/GpioPin.hpp>

using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::Pointee;
using ::testing::ElementsAreArray;

namespace gpio {

class GpioPinMock {

};

} /* namespace gpio */

namespace devices {

/*******************************************************************************
 *
 ******************************************************************************/
class Sr74x595Test : public ::testing::Test {
protected:
    typedef Sr74x595T<spi::SpiDeviceMock, gpio::GpioPinMock> Sr74x595;
    
    Sr74x595 *m_device;
    
    spi::SpiDeviceMock  m_spi;
    gpio::GpioPinMock   m_reset;
    gpio::GpioPinMock   m_enable;

public:
    Sr74x595Test() : m_device(NULL) {
        m_device = new Sr74x595(m_spi, &m_reset, &m_enable);
    }

    virtual ~Sr74x595Test() {
        delete m_device;
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(Sr74x595Test, CreateAndDelete) {

}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(Sr74x595Test, Write) {
    const uint8_t value = 0xC5;
    const uint8_t expectedValue = value;
    const ssize_t len = sizeof(value);
    const size_t bitlen = len * 8;

    InSequence s;
    EXPECT_CALL(this->m_spi, select()).Times(1).WillOnce(Return(0));
    EXPECT_CALL(this->m_spi, shift(bitlen, Pointee(expectedValue), NULL)).Times(1).WillOnce(Return(bitlen));
    EXPECT_CALL(this->m_spi, deselect()).Times(1).WillOnce(Return(0));

    int rc = this->m_device->write(&value, len);
    EXPECT_EQ(0, rc);
}


} /* namespace devices */
