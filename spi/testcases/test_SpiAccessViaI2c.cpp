/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <spi/SpiAccessViaI2c.hpp>
#include <spi/SpiAccessViaI2c.cpp>

#include <errno.h>

using ::testing::Return;
using ::testing::NotNull;
using ::testing::StrEq;
using ::testing::SetArgPointee;
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
class SpiAccessViaI2cTest : public ::testing::Test {
protected:
    i2c::I2cDeviceMock                      m_mock;
    SpiAccessViaI2cT<i2c::I2cDeviceMock> *  m_access;

    static const uint32_t   m_base;
    static const uint32_t   REG_RXTX;
    static const uint32_t   REG_CTRL;
    static const uint32_t   REG_NSEL;
    static const uint32_t   REG_DIV;

public:
    SpiAccessViaI2cTest() {
        m_access = new spi::SpiAccessViaI2cT<i2c::I2cDeviceMock>(&m_mock, m_base);
    };

    virtual ~SpiAccessViaI2cTest() {
        delete m_access;
    };
};

const uint32_t SpiAccessViaI2cTest::m_base = 0x80000000u;
const uint32_t SpiAccessViaI2cTest::REG_RXTX = SpiAccessViaI2cTest::m_base + 0u;
const uint32_t SpiAccessViaI2cTest::REG_CTRL = SpiAccessViaI2cTest::m_base + 1u;
const uint32_t SpiAccessViaI2cTest::REG_NSEL = SpiAccessViaI2cTest::m_base + 2u;
const uint32_t SpiAccessViaI2cTest::REG_DIV = SpiAccessViaI2cTest::m_base + 3u;

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, CreateAndDelete) {

}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, Select) {
    EXPECT_CALL(m_mock, read(REG_NSEL, NotNull(), 1)).Times(1).WillOnce(DoAll(SetArgPointee<1>(0xFF), Return(1)));
    uint8_t data[] = { 0xfb };
    EXPECT_CALL(m_mock, write(REG_NSEL, IsEqualBuffer(data, sizeof(data)), sizeof(data))).Times(1).WillOnce(Return(1));

    EXPECT_EQ(0, m_access->select(2));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, SelectInvalidDevice) {
    EXPECT_EQ(EINVAL, m_access->select(8));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, SelectWithReadError) {
    EXPECT_CALL(m_mock, read(REG_NSEL, NotNull(), 1)).Times(1).WillOnce(Return(0));

    EXPECT_EQ(ENXIO, m_access->select(2));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, SelectWithWriteError) {
    EXPECT_CALL(m_mock, read(REG_NSEL, NotNull(), 1)).Times(1).WillOnce(DoAll(SetArgPointee<1>(0xFF), Return(1)));
    uint8_t data[] = { 0xfd };
    EXPECT_CALL(m_mock, write(REG_NSEL, IsEqualBuffer(data, sizeof(data)), sizeof(data))).Times(1).WillOnce(Return(0));

    EXPECT_EQ(ENXIO, m_access->select(1));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, Deselect) {
    EXPECT_CALL(m_mock, read(REG_NSEL, NotNull(), 1)).Times(1).WillOnce(DoAll(SetArgPointee<1>(0xfb), Return(1)));
    uint8_t data[] = { 0xff };
    EXPECT_CALL(m_mock, write(REG_NSEL, IsEqualBuffer(data, sizeof(data)), sizeof(data))).Times(1).WillOnce(Return(1));

    EXPECT_EQ(0, m_access->deselect(2));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, DeselectInvalidDevice) {
    EXPECT_EQ(EINVAL, m_access->deselect(8));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, DeselectWithReadError) {
    EXPECT_CALL(m_mock, read(REG_NSEL, NotNull(), 1)).Times(1).WillOnce(Return(0));

    EXPECT_EQ(ENXIO, m_access->deselect(2));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, DeselectWithWriteError) {
    EXPECT_CALL(m_mock, read(REG_NSEL, NotNull(), 1)).Times(1).WillOnce(DoAll(SetArgPointee<1>(0xfd), Return(1)));
    uint8_t data[] = { 0xff };
    EXPECT_CALL(m_mock, write(REG_NSEL, IsEqualBuffer(data, sizeof(data)), sizeof(data))).Times(1).WillOnce(Return(0));

    EXPECT_EQ(ENXIO, m_access->deselect(1));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, ShiftWithInvalidParametersZeroBits) {
    EXPECT_EQ(EINVAL, m_access->shift(0, NULL, 0xFF, SpiMode0)); // 0 Bits
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, ShiftWithInvalidParametersTooManyBits) {
    EXPECT_EQ(EINVAL, m_access->shift(9, NULL, 0xFF, SpiMode0)); // >8 Bits
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, ShiftWithInvalidParametersInvalidMode) {
    EXPECT_EQ(EINVAL, m_access->shift(9, NULL, 0xFF, (spi::Mode) 0x4711)); // Invalid Mode
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, ShiftTx) {
    uint8_t data[] = { 0xa5, 0x97 };
    EXPECT_CALL(m_mock, write(REG_RXTX, IsEqualBuffer(data, sizeof(data)), sizeof(data))).Times(1).WillOnce(Return(2));

    EXPECT_EQ(0, m_access->shift(8, NULL, 0xa5, SpiMode2));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, ShiftTxWriteError) {
    uint8_t data[] = { 0xa5, 0x8b };
    EXPECT_CALL(m_mock, write(REG_RXTX, IsEqualBuffer(data, sizeof(data)), sizeof(data))).Times(1).WillOnce(Return(1));

    EXPECT_EQ(ENXIO, m_access->shift(4, NULL, 0xa5, SpiMode1));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, ShiftRx) {
    uint8_t data[] = { 0xff, 0x87 }, rx = 0xa5, buf = 0;
    EXPECT_CALL(m_mock, write(REG_RXTX, IsEqualBuffer(data, sizeof(data)), sizeof(data))).Times(1).WillOnce(Return(2));
    EXPECT_CALL(m_mock, read(REG_RXTX, NotNull(), 1)).Times(1).WillOnce(DoAll(SetArgPointee<1>(rx), Return(1)));

    EXPECT_EQ(0, m_access->shift(8, &buf));
    EXPECT_EQ(rx, buf);
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, ShiftRxWriteError) {
    uint8_t data[] = { 0xff, 0x81 }, buf = 0;
    EXPECT_CALL(m_mock, write(REG_RXTX, IsEqualBuffer(data, sizeof(data)), sizeof(data))).Times(1).WillOnce(Return(1));

    EXPECT_EQ(ENXIO, m_access->shift(2, &buf));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiAccessViaI2cTest, ShiftRxReadError) {
    uint8_t data[] = { 0xff, 0x9d }, buf = 0;
    EXPECT_CALL(m_mock, write(REG_RXTX, IsEqualBuffer(data, sizeof(data)), sizeof(data))).Times(1).WillOnce(Return(2));
    EXPECT_CALL(m_mock, read(REG_RXTX, NotNull(), 1)).Times(1).WillOnce(Return(0));

    EXPECT_EQ(ENXIO, m_access->shift(6, &buf, 0xFF, SpiMode3));
}

} /* namespace spi */
