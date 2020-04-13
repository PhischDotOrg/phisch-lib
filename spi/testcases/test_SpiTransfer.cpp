/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <spi/SpiTransfer.hpp>
#include <spi/SpiTransfer.cpp>

#include <sys/types.h>
#include <stdint.h>
#include <sstream>

using ::testing::_;
using ::testing::Return;
using ::testing::IsNull;
using ::testing::NotNull;
using ::testing::StrEq;
using ::testing::SetArgPointee;
using ::testing::InSequence;
using ::testing::DoAll;

namespace spi {

/*******************************************************************************
 * 
 ******************************************************************************/
class SpiTransferTest : public ::testing::Test {
protected:
    SpiDeviceMock               m_device;
    TransferT<SpiDeviceMock> *  m_transfer;
    std::stringstream           m_ostream;

public:
    SpiTransferTest() {
        m_transfer = new spi::TransferT<SpiDeviceMock>(&m_device, m_ostream);
    };

    virtual ~SpiTransferTest() {
        delete m_transfer;
    };
};

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(SpiTransferTest, CreateAndDelete) {

}


/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(SpiTransferTest, BufferExactSize) {
    uint8_t data[] = { 0xde, 0xad, 0xc0, 0xde };
    // uint8_t rxdata[] = { 0xca, 0xfe, 0xba, 0xbe };
    ssize_t bitlen = sizeof(data) * 8;

    /* FIXME SetArgPointee<2>(rxdata) */
    EXPECT_CALL(m_device, shift(bitlen, NotNull(), NotNull())).Times(1).WillOnce(Return(bitlen));

    ssize_t rc = m_transfer->transfer(sizeof(data) * 8, data, sizeof(data));
    EXPECT_EQ(bitlen, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(SpiTransferTest, BufferLargerThanBits) {
    uint8_t data[] = { 0xde, 0xad, 0xc0, 0xde };
    // uint8_t rxdata[] = { 0xca, 0xfe, 0xba, 0xbe };
    ssize_t bitlen = (sizeof(data) * 8) - 3;

    /* FIXME SetArgPointee<2>(rxdata) */
    EXPECT_CALL(m_device, shift(bitlen, NotNull(), NotNull())).Times(1).WillOnce(Return(bitlen));

    ssize_t rc = m_transfer->transfer(bitlen, data, sizeof(data));
    EXPECT_EQ(bitlen, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(SpiTransferTest, BufferSmallerThanBits) {
    uint8_t data[] = { 0xde, 0xad, 0xc0, 0xde };
    // uint8_t rxdata[] = { 0xca, 0xfe, 0xba, 0xbe };
    ssize_t bitlen = (sizeof(data) * 8) + 3;

    /* FIXME SetArgPointee<2>(rxdata) */
    InSequence s;
    EXPECT_CALL(m_device, shift(sizeof(data) * 8, NotNull(), NotNull())).Times(1).WillOnce(Return(sizeof(data) * 8));
    EXPECT_CALL(m_device, shift(3, IsNull(), NotNull())).Times(1).WillOnce(Return(3));

    ssize_t rc = m_transfer->transfer(bitlen, data, sizeof(data));
    EXPECT_EQ(bitlen, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(SpiTransferTest, BufferIsNull) {
    ssize_t bitlen = 72;

    /* FIXME SetArgPointee<2>(rxdata) */
    InSequence s;
    EXPECT_CALL(m_device, shift(bitlen, NULL, NotNull())).Times(1).WillOnce(Return(bitlen));

    ssize_t rc = m_transfer->transfer(bitlen, NULL, 0);
    EXPECT_EQ(bitlen, rc);
}

} /* namespace spi */
