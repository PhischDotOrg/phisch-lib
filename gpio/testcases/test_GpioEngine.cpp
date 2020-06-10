/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

##include <gpio/GpioEngine.hpp>

#include <errno.h>

using ::testing::Return;
using ::testing::_;
using ::testing::DoAll;
using ::testing::SetArgReferee;

namespace gpio {

/*******************************************************************************
 *
 ******************************************************************************/
class GpioEngineTest : public ::testing::Test {
protected:
    GpioAccessMock m_mock;
    gpio::GpioEngine *m_engine;

public:
    GpioEngineTest() : m_engine(NULL) {
        m_engine = new gpio::GpioEngine(&m_mock);
    };

    virtual ~GpioEngineTest() {
        delete m_engine;
    };
};

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioEngineTest, CreateAndDelete) {

}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioEngineTest, WriteNoError) {
    EXPECT_CALL(m_mock, write(0x12u, 0x34u, 0x56u)).Times(1).WillOnce(Return(0));
    int rc = m_engine->write(0x12u, 0x34u, 0x56u);
    EXPECT_EQ(0, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioEngineTest, WriteWithError) {
    EXPECT_CALL(m_mock, write(0x12u, 0x34u, 0x56u)).Times(1).WillOnce(Return(-1));
    int rc = m_engine->write(0x12u, 0x34u, 0x56u);
    EXPECT_EQ(EIO, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioEngineTest, ReadNoError) {
    gpio::GpioEngine::vector_t value = 0u;
    EXPECT_CALL(m_mock, read(_)).Times(1).WillOnce(DoAll(SetArgReferee<0>(0x12u), Return(0)));
    int rc = m_engine->read(value);
    EXPECT_EQ(0, rc);
    EXPECT_EQ(0x12u, value);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioEngineTest, ReadWithError) {
    gpio::GpioEngine::vector_t value = 0u;
    EXPECT_CALL(m_mock, read(_)).Times(1).WillOnce(Return(EIO));
    int rc = m_engine->read(value);
    EXPECT_EQ(EIO, rc);
    EXPECT_EQ(0u, value);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioEngineTest, Init) {
    EXPECT_CALL(m_mock, write(0, 0, -1)).Times(1).WillOnce(Return(0));

    int rc = m_engine->init();
    EXPECT_EQ(0, rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(GpioEngineTest, InitWithError) {
    EXPECT_CALL(m_mock, write(0, 0, -1)).Times(1).WillOnce(Return(4711));

    int rc = m_engine->init();
    EXPECT_EQ(EIO, rc);
}

} /* namespace gpio */
