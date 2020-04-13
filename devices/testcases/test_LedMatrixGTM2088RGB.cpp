/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <devices/LedMatrixGTM2088RGB.hpp>
#include <devices/Sr74x595.hpp>

using ::testing::NotNull;
using ::testing::Return;

namespace devices {

/*******************************************************************************
 *
 ******************************************************************************/
static const unsigned c_width = 8;
static const unsigned c_height = 8;

/*******************************************************************************
 *
 ******************************************************************************/
class LedMatrixGTM2088RGBBaseTest : public ::testing::Test {
protected:
    typedef LedMatrixGtn2088RgbBaseT<c_width, c_height> LedMatrixGtn2088Rgb;

    LedMatrixGtn2088Rgb *   m_obj;

public:
    LedMatrixGTM2088RGBBaseTest() : m_obj(NULL) {
        m_obj = new LedMatrixGtn2088Rgb(/* m_pins, m_pinGroupContainer, m_redPins, m_greenPins, m_bluePins */);
    }

    virtual ~LedMatrixGTM2088RGBBaseTest() {
        delete m_obj;
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(LedMatrixGTM2088RGBBaseTest, CreateAndDelete) {
    
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(LedMatrixGTM2088RGBBaseTest, WidthAndHeight) {
    EXPECT_EQ(c_width, LedMatrixGtn2088Rgb::getWidth());
    EXPECT_EQ(c_height, LedMatrixGtn2088Rgb::getHeight());
}

/*******************************************************************************
 * Attempt to set a single pixel in the matrix. Ensure that initially all pixels
 * around the pixel (+/- 1) are off, then set the single pixel. Afterwards,
 * ensure only the single pixel changed.
 ******************************************************************************/
TEST_F(LedMatrixGTM2088RGBBaseTest, SetAndGetSinglePixel) {
    const unsigned x_pos = c_width - 2;
    const unsigned y_pos = 1;
    const LedMatrixGtn2088Rgb::RgbChannel_t color = LedMatrixGtn2088Rgb::e_RgbChannel_Red;
    bool value;
        
    ASSERT_GT(c_height, y_pos);
    ASSERT_GT(c_width, x_pos);

    for (int x_offs = -1; x_offs <= 1; x_offs++) {
        for (int y_offs = -1; y_offs <= 1; y_offs++) {
            EXPECT_EQ(0, m_obj->getPixel(x_pos + x_offs, y_pos + y_offs, color, value)) << "where x_offs=" << x_offs << " and y_offs=" << y_offs;
            EXPECT_FALSE(value) << "where x_offs=" << x_offs << " and y_offs=" << y_offs;
        }
    }
    
    EXPECT_EQ(0, m_obj->setPixel(x_pos, y_pos, color, true));

    for (int x_offs = -1; x_offs <= 1; x_offs++) {
        for (int y_offs = -1; y_offs <= 1; y_offs++) {
            EXPECT_EQ(0, m_obj->getPixel(x_pos + x_offs, y_pos + y_offs, color, value)) << "where x_offs=" << x_offs << " and y_offs=" << y_offs;
            if (!x_offs && !y_offs) {
                EXPECT_TRUE(value) << "where x_offs=" << x_offs << " and y_offs=" << y_offs;
            } else {
                EXPECT_FALSE(value) << "where x_offs=" << x_offs << " and y_offs=" << y_offs;
            }
        }
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(LedMatrixGTM2088RGBBaseTest, GetAndSetSinglePixelWithInvalidOffset) {
    const LedMatrixGtn2088Rgb::RgbChannel_t color = LedMatrixGtn2088Rgb::e_RgbChannel_Red;
    bool value;
    
    const unsigned x_pos[] = { static_cast<unsigned>(-1), c_width };
    const unsigned y_pos[] = { static_cast<unsigned>(-1), c_height };
    
    for (unsigned x_idx = 0; x_idx < (sizeof(x_pos) / sizeof(x_pos[0])); x_idx++) {
        for (unsigned y_idx = 0; y_idx < (sizeof(y_pos) / sizeof(y_pos[0])); y_idx++) {
            EXPECT_NE(0, m_obj->getPixel(x_pos[x_idx], y_pos[y_idx], color, value)) << "where x_pos=" << x_pos[x_idx] << " and y_pos=" << y_pos[y_idx];
            EXPECT_NE(0, m_obj->setPixel(x_pos[x_idx], y_pos[y_idx], color, true)) << "where x_pos=" << x_pos[x_idx] << " and y_pos=" << y_pos[y_idx];
        }
    }
}

/*******************************************************************************
 * Attempt to set a single column in the matrix. Ensure that initially all pixels
 * around the column (+/- 1) are off, then set a pattern in the single column.
 * Afterwards, ensure only pixels in the single column changed.
 ******************************************************************************/
TEST_F(LedMatrixGTM2088RGBBaseTest, SetAndGetColumn) {
    const unsigned x_pos = 1;
    const LedMatrixGtn2088Rgb::RgbChannel_t color = LedMatrixGtn2088Rgb::e_RgbChannel_Blue;
    const LedMatrixGtn2088Rgb::ColVector_t expected = { 0xa5 };
    const LedMatrixGtn2088Rgb::ColVector_t initial = { 0 };
    LedMatrixGtn2088Rgb::ColVector_t value;

    for (int x_offs = -1; x_offs <= 1; x_offs++) {
        EXPECT_EQ(0, m_obj->getColumn(x_pos + x_offs, color, value)) << "where x_offs=" << x_offs;
        EXPECT_EQ(initial, value) << "where x_offs=" << x_offs;
    }
    
    EXPECT_EQ(0, m_obj->setColumn(x_pos, color, expected));

    for (int x_offs = -1; x_offs <= 1; x_offs++) {
        EXPECT_EQ(0, m_obj->getColumn(x_pos + x_offs, color, value)) << "where x_offs=" << x_offs;
        EXPECT_EQ(x_offs == 0 ? expected : initial, value) << "where x_offs=" << x_offs;
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(LedMatrixGTM2088RGBBaseTest, GetAndSetColumnWithInvalidOffset) {
    const LedMatrixGtn2088Rgb::RgbChannel_t color = LedMatrixGtn2088Rgb::e_RgbChannel_Red;
    LedMatrixGtn2088Rgb::ColVector_t value = 0;
    
    const unsigned x_pos[] = { static_cast<unsigned>(-1), c_width };
    
    for (unsigned x_idx = 0; x_idx < (sizeof(x_pos) / sizeof(x_pos[0])); x_idx++) {
        EXPECT_NE(0, m_obj->setColumn(x_pos[x_idx], color, value)) << "where x_pos=" << x_pos[x_idx];
        EXPECT_NE(0, m_obj->getColumn(x_pos[x_idx], color, value)) << "where x_pos=" << x_pos[x_idx];
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST(LedMatrixGTM2088RGBTest_NoFixture, TypesOfWidthAndHeightWithNaturalTypes) {
    static const unsigned width_bytes = 4;
    static const unsigned width = width_bytes * 8;
    
    static const unsigned height_bytes = 8;
    static const unsigned height = height_bytes * 8;

    typedef LedMatrixGtn2088RgbBaseT<width, height> LedMatrixGtn2088Rgb;

    EXPECT_EQ(width, LedMatrixGtn2088Rgb::getWidth());
    EXPECT_EQ(height, LedMatrixGtn2088Rgb::getHeight());

    EXPECT_EQ(width_bytes, sizeof(LedMatrixGtn2088Rgb::RowVector_t));
    EXPECT_EQ(height_bytes, sizeof(LedMatrixGtn2088Rgb::ColVector_t));
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST(LedMatrixGTM2088RGBTest_NoFixture, TypesOfWidthAndHeightWithArrayTypes) {
    static const unsigned width_bytes = 9;
    static const unsigned width = width_bytes * 8;
    
    static const unsigned height_bytes = 16;
    static const unsigned height = height_bytes * 8;

    typedef LedMatrixGtn2088RgbBaseT<width, height> LedMatrixGtn2088Rgb;

    EXPECT_EQ(width, LedMatrixGtn2088Rgb::getWidth());
    EXPECT_EQ(height, LedMatrixGtn2088Rgb::getHeight());

    /*
     * The vector types are defined as an array of unsigned 32-bit integers if
     * the width/height exceeds a value of 64. Therefore, the size of the array
     * type must be greater than or equal to the number of bytes required to
     * store a n-bit vector.
     */
    EXPECT_GE(sizeof(LedMatrixGtn2088Rgb::RowVector_t), width_bytes);
    EXPECT_LT(sizeof(LedMatrixGtn2088Rgb::RowVector_t), width_bytes + sizeof(uint32_t));

    EXPECT_GE(sizeof(LedMatrixGtn2088Rgb::ColVector_t), height_bytes);
    EXPECT_LT(sizeof(LedMatrixGtn2088Rgb::ColVector_t), height_bytes + sizeof(uint32_t));
}


/*******************************************************************************
 *
 ******************************************************************************/
class LedMatrixGTM2088RGBViaShiftRegisterTest : public ::testing::Test {
protected:
    typedef LedMatrixGtn2088Rgb_8x8_ViaShiftRegisterT<Sr74x595Mock> LedMatrixGtn2088Rgb;

    LedMatrixGtn2088Rgb *                   m_obj;
    const unsigned                          m_col;
    const LedMatrixGtn2088Rgb::ColVector_t  m_pattern;
    Sr74x595Mock                            m_srMock;

    void mockWriteBufferToHardware(const uint8_t /*p_pattern */[]) {
        EXPECT_CALL(m_srMock, write(NotNull(), 4)).WillOnce(Return(0));
    }

    void compareBufferWithPattern(const uint8_t p_pattern[]) {
        printf("Expecting pattern [%.02x][%.02x][%.02x][%.02x]\n", p_pattern[0], p_pattern[1], p_pattern[2], p_pattern[3]);

        for (unsigned i = 0; i < sizeof(m_obj->m_buffer); i++) {
            EXPECT_EQ(p_pattern[0], m_obj->m_buffer[0]) << "for i=" << i;
        }
    }

public:
    LedMatrixGTM2088RGBViaShiftRegisterTest() : m_obj(NULL), m_col(2), m_pattern(0xa5) {
        m_obj = new LedMatrixGtn2088Rgb(m_srMock);
        
        m_obj->setColumn(m_col, LedMatrixGtn2088Rgb::LedMatrixGtn2088RgbBase_t::e_RgbChannel_Red, m_pattern);
        m_obj->setColumn(m_col, LedMatrixGtn2088Rgb::LedMatrixGtn2088RgbBase_t::e_RgbChannel_Green, m_pattern);
        m_obj->setColumn(m_col, LedMatrixGtn2088Rgb::LedMatrixGtn2088RgbBase_t::e_RgbChannel_Blue, m_pattern);
    }

    virtual ~LedMatrixGTM2088RGBViaShiftRegisterTest() {
        delete m_obj;
    }
};

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(LedMatrixGTM2088RGBViaShiftRegisterTest, CreateAndDelete) {
    
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(LedMatrixGTM2088RGBViaShiftRegisterTest, RefreshColumnInRedChannel) {
    const LedMatrixGtn2088Rgb::RgbChannel_t color = LedMatrixGtn2088Rgb::e_RgbChannel_Red;
    uint8_t pattern[4];

    pattern[3] = this->m_pattern;
    pattern[2] = 0;
    pattern[1] = 0;
    pattern[0] = (1 << m_col);

    mockWriteBufferToHardware(pattern);
    
    EXPECT_EQ(0, m_obj->refresh(m_col, color));
    this->compareBufferWithPattern(pattern);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(LedMatrixGTM2088RGBViaShiftRegisterTest, RefreshColumnInGreenChannel) {
    const LedMatrixGtn2088Rgb::RgbChannel_t color = LedMatrixGtn2088Rgb::e_RgbChannel_Green;
    uint8_t pattern[4];

    pattern[3] = this->m_pattern;
    pattern[2] = (1 << m_col);
    pattern[1] = 0;
    pattern[0] = 0;

    mockWriteBufferToHardware(pattern);
    
    EXPECT_EQ(0, m_obj->refresh(m_col, color));
    this->compareBufferWithPattern(pattern);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(LedMatrixGTM2088RGBViaShiftRegisterTest, RefreshColumnInBlueChannel) {
    const LedMatrixGtn2088Rgb::RgbChannel_t color = LedMatrixGtn2088Rgb::e_RgbChannel_Blue;
    uint8_t pattern[4];

    pattern[3] = this->m_pattern;
    pattern[2] = 0;
    pattern[1] = (1 << m_col);
    pattern[0] = 0;
    
    mockWriteBufferToHardware(pattern);

    EXPECT_EQ(0, m_obj->refresh(m_col, color));
    this->compareBufferWithPattern(pattern);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(LedMatrixGTM2088RGBViaShiftRegisterTest, RefreshInvalidColumn) {
    const LedMatrixGtn2088Rgb::RgbChannel_t color = LedMatrixGtn2088Rgb::e_RgbChannel_Red;
    const unsigned x_pos[] = { static_cast<unsigned>(-1), c_width };
    
    for (unsigned x_idx = 0; x_idx < (sizeof(x_pos) / sizeof(x_pos[0])); x_idx++) {
        EXPECT_NE(0, m_obj->refresh(x_pos[x_idx], color)) << "where x_pos=" << x_pos[x_idx];
    }
}

} /* namespace devices */
