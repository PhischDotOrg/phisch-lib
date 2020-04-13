/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <gpio/GpioAccessViaLpt.hpp>
#include <gpio/GpioAccessViaLpt.cpp>

#include <lpt/LptAccessMock.hpp>

#include <errno.h>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::NotNull;

namespace gpio {

/*******************************************************************************
 *
 ******************************************************************************/
class GpioAccessViaLptTest : public ::testing::Test {
protected:
    lpt::LptAccessMock m_mock;
    AccessViaLptT<lpt::LptAccessMock> *m_access;

public:
    GpioAccessViaLptTest() : m_access(NULL) {
        m_access = new AccessViaLptT<lpt::LptAccessMock>(&m_mock);
    };

    virtual ~GpioAccessViaLptTest() {
        delete m_access;
    };
};

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioAccessViaLptTest, CreateAndDelete) {
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioAccessViaLptTest, Read) {
    uint8_t vector = 0;

    InSequence s;
    EXPECT_CALL(m_mock, getStatus(_)).Times(1).WillOnce(DoAll(SetArgPointee<0>(0xF0), Return(0)));

    int rc = m_access->read(vector);
    EXPECT_EQ(0, rc);
    EXPECT_EQ(0x7u, vector);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioAccessViaLptTest, ReadWithError) {
    uint8_t vector = 0;

    InSequence s;
    EXPECT_CALL(m_mock, getStatus(_)).Times(1).WillOnce(DoAll(SetArgPointee<0>(0xF0), Return(-1)));

    int rc = m_access->read(vector);
    EXPECT_NE(0, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioAccessViaLptTest, WriteAllOutputs) {
    InSequence s;

    EXPECT_CALL(m_mock, getData(NotNull())).Times(1).WillOnce(DoAll(SetArgPointee<0>(-1), Return(0)));
    EXPECT_CALL(m_mock, setData(0x05)).Times(1).WillOnce(Return(0));

    int rc = m_access->write(0x5, 0xF, 0xF);
    EXPECT_EQ(0, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioAccessViaLptTest, WriteAllInputs) {
    InSequence s;

    EXPECT_CALL(m_mock, getData(NotNull())).Times(1).WillOnce(DoAll(SetArgPointee<0>(-1), Return(0)));
    EXPECT_CALL(m_mock, setData(0xF0)).Times(1).WillOnce(Return(0));

    int rc = m_access->write(0, 0x00, 0x0F);
    EXPECT_EQ(0, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioAccessViaLptTest, WriteGetError) {
    InSequence s;

    EXPECT_CALL(m_mock, getData(NotNull())).Times(1).WillOnce(DoAll(SetArgPointee<0>(-1), Return(-1)));
    EXPECT_CALL(m_mock, setData(0xFF)).Times(0);

    int rc = m_access->write(0, 0x00, 0x0F);
    EXPECT_EQ(EIO, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioAccessViaLptTest, WriteSetError) {
    InSequence s;

    EXPECT_CALL(m_mock, getData(NotNull())).Times(1).WillOnce(DoAll(SetArgPointee<0>(-1), Return(0)));
    EXPECT_CALL(m_mock, setData(0x05)).Times(1).WillOnce(Return(-1));

    int rc = m_access->write(0x5, 0xF, 0xF);
    EXPECT_EQ(EIO, rc);
}

} /* namespace gpio */
