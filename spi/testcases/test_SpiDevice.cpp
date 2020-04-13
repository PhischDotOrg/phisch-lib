/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <spi/SpiDevice.hpp>
#include <spi/SpiDevice.cpp>

using ::testing::_;
using ::testing::Return;
using ::testing::NotNull;
using ::testing::StrEq;
using ::testing::SetArgPointee;
using ::testing::InSequence;

namespace spi {

/*******************************************************************************
 * 
 ******************************************************************************/
class SpiDeviceTest : public ::testing::Test {
protected:
    SpiAccessMock           m_access;
    DeviceT<SpiAccessMock> *m_device;
    static const uint8_t    m_number;
    static const spi::Mode  m_mode;

public:
    SpiDeviceTest() {
        m_device = new spi::DeviceT<SpiAccessMock>(&m_access, m_number, m_mode);
    };

    virtual ~SpiDeviceTest() {
        delete m_device;
    };
};

const uint8_t SpiDeviceTest::m_number = 3;
const spi::Mode SpiDeviceTest::m_mode = SpiMode1;

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(SpiDeviceTest, CreateAndDelete) {

}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, Select) {
    int rc = 0;

    InSequence s;
    EXPECT_CALL(m_access, select(m_number)).Times(1).WillOnce(Return(rc));
    EXPECT_EQ(rc, m_device->select());
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, SelectWithError) {
    int rc = 0x4711;

    InSequence s;
    EXPECT_CALL(m_access, select(m_number)).Times(1).WillOnce(Return(rc));
    EXPECT_EQ(rc, m_device->select());
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, Deselect) {
    int rc = 0;

    InSequence s;
    EXPECT_CALL(m_access, deselect(m_number)).Times(1).WillOnce(Return(rc));
    EXPECT_EQ(rc, m_device->deselect());
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, DeselectWithError) {
    int rc = 0x4711;

    InSequence s;
    EXPECT_CALL(m_access, deselect(m_number)).Times(1).WillOnce(Return(rc));
    EXPECT_EQ(rc, m_device->deselect());
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, ShiftSingleByteWithoutRead) {
    uint8_t data[] = { 0xa5 };

    InSequence s;
    EXPECT_CALL(m_access, shift(8, _, data[0], m_mode)).Times(1).WillOnce(Return(0));

    EXPECT_EQ(8, m_device->shift(8, data, NULL));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, ShiftTwoByteWithoutRead) {
    uint8_t data[] = { 0xa5, 0x5a };

    InSequence s;
    EXPECT_CALL(m_access, shift(8, _, data[0], m_mode)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(m_access, shift(4, _, data[1], m_mode)).Times(1).WillOnce(Return(0));

    EXPECT_EQ(12, m_device->shift(12, data, NULL));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, ShiftMultiByteWithoutRead) {
    uint8_t data[] = { 0xde, 0xad, 0xc0, 0xde };

    InSequence s;
    EXPECT_CALL(m_access, shift(8, _, data[0], m_mode)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(m_access, shift(8, _, data[1], m_mode)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(m_access, shift(8, _, data[2], m_mode)).Times(1).WillOnce(Return(0));
    EXPECT_CALL(m_access, shift(7, _, data[3], m_mode)).Times(1).WillOnce(Return(0));

    EXPECT_EQ(31, m_device->shift(31, data, NULL));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, ShiftSingleByteWithoutWrite) {
    uint8_t data[] = { 0xa5 }, rx[sizeof(data)];

    InSequence s;
    EXPECT_CALL(m_access, shift(8, NotNull(), _, m_mode)).Times(1).WillOnce(DoAll(SetArgPointee<1>(data[0]), Return(0)));

    EXPECT_EQ(8, m_device->shift(8, NULL, rx));
    EXPECT_EQ(0, bcmp(data, rx, sizeof(data)));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, ShiftTwoByteWithoutWrite) {
    uint8_t data[] = { 0xa5, 0x50 }, rx[sizeof(data)];

    InSequence s;
    EXPECT_CALL(m_access, shift(8, NotNull(), _, m_mode)).Times(1).WillOnce(DoAll(SetArgPointee<1>(data[0]), Return(0)));
    EXPECT_CALL(m_access, shift(4, NotNull(), _, m_mode)).Times(1).WillOnce(DoAll(SetArgPointee<1>(data[1]), Return(0)));

    EXPECT_EQ(12, m_device->shift(12, NULL, rx));
    EXPECT_EQ(0, bcmp(data, rx, sizeof(data)));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, ShiftMultiByteWithoutWrite) {
    uint8_t data[] = { 0xde, 0xad, 0xc0, 0xde }, rx[sizeof(data)];

    InSequence s;
    EXPECT_CALL(m_access, shift(8, NotNull(), _, m_mode)).Times(1).WillOnce(DoAll(SetArgPointee<1>(data[0]), Return(0)));
    EXPECT_CALL(m_access, shift(8, NotNull(), _, m_mode)).Times(1).WillOnce(DoAll(SetArgPointee<1>(data[1]), Return(0)));
    EXPECT_CALL(m_access, shift(8, NotNull(), _, m_mode)).Times(1).WillOnce(DoAll(SetArgPointee<1>(data[2]), Return(0)));
    EXPECT_CALL(m_access, shift(7, NotNull(), _, m_mode)).Times(1).WillOnce(DoAll(SetArgPointee<1>(data[3]), Return(0)));

    EXPECT_EQ(31, m_device->shift(31, NULL, rx));
    EXPECT_EQ(0, bcmp(data, rx, sizeof(data)));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, ShiftMultiByteWithError) {
    uint8_t data[] = { 0xde, 0xad, 0xc0, 0xde }, rx[sizeof(data)];

    InSequence s;
    EXPECT_CALL(m_access, shift(8, NotNull(), data[0], m_mode)).Times(1).WillOnce(DoAll(SetArgPointee<1>(data[0]), Return(0)));
    EXPECT_CALL(m_access, shift(8, NotNull(), data[1], m_mode)).Times(1).WillOnce(Return(-1));

    EXPECT_EQ(8, m_device->shift(31, data, rx));
    EXPECT_EQ(0, bcmp(data, rx, 1));
}

/*******************************************************************************
 * 
 ******************************************************************************/
TEST_F(SpiDeviceTest, ShiftWithOverlappingBuffers) {
    uint8_t tx[] = { 0xde, 0xad, 0xc0, 0xde };
    uint8_t rx[] = { 0xca, 0xfe, 0xba, 0xbe };

    InSequence s;
    for (unsigned i = 0; i < sizeof(tx); i++)
        EXPECT_CALL(m_access, shift(8, NotNull(), tx[i], m_mode)).Times(1).WillOnce(DoAll(SetArgPointee<1>(rx[i]), Return(0)));

    EXPECT_EQ((int) (sizeof(tx) * 8), m_device->shift(sizeof(tx) * 8, tx, tx));
    EXPECT_EQ(0, bcmp(tx, rx, sizeof(tx)));
}

}; /* namespace spi */
