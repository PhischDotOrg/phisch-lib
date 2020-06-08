/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <uart/UartAccess.hpp>
#include <uart/UartDevice.hpp>

using ::testing::InSequence;

namespace uart {

/*******************************************************************************
 *
 ******************************************************************************/
class UartDeviceTest : public ::testing::Test {
protected:
    UartAccessMock          m_access;
    DeviceT<UartAccessMock> * m_device;

public:
    UartDeviceTest() {
        m_device = new uart::DeviceT<UartAccessMock>(&m_access);
    };

    virtual ~UartDeviceTest() {
        delete m_device;
    };
};

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(UartDeviceTest, CreateAndDelete) {
    /* Intentionally left blank */
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(UartDeviceTest, PrintNumber) {
    InSequence s;
    EXPECT_CALL(m_access, putf('0')).Times(1);
    EXPECT_CALL(m_access, putf('x')).Times(1);
    EXPECT_CALL(m_access, putf('d')).Times(1);
    EXPECT_CALL(m_access, putf('e')).Times(1);
    EXPECT_CALL(m_access, putf('a')).Times(1);
    EXPECT_CALL(m_access, putf('d')).Times(1);
    EXPECT_CALL(m_access, putf('c')).Times(1);
    EXPECT_CALL(m_access, putf('0')).Times(1);
    EXPECT_CALL(m_access, putf('d')).Times(1);
    EXPECT_CALL(m_access, putf('e')).Times(1);

    m_device->printf("0x%x", 0xdeadc0de);
}

} /* namespace uart */

