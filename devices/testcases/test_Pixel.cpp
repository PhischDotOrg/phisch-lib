/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <devices/Pixel.hpp>

 /*****************************************************************************/
class RgbPixelTest : public ::testing::Test {

public:
    RgbPixelTest() {
    }

    virtual ~RgbPixelTest() {
    }

protected:
    Pixel::RGB   m_rgbPixel;
};
/*****************************************************************************/

/*****************************************************************************/
TEST_F(RgbPixelTest, DefaultValues) {
    EXPECT_EQ(0, m_rgbPixel.r);
    EXPECT_EQ(0, m_rgbPixel.g);
    EXPECT_EQ(0, m_rgbPixel.b);
}

TEST_F(RgbPixelTest, RawLayout) {
    m_rgbPixel.g = 1;
    m_rgbPixel.r = 2;
    m_rgbPixel.b = 3;

    EXPECT_EQ(m_rgbPixel.raw[0], m_rgbPixel.g);
    EXPECT_EQ(m_rgbPixel.raw[1], m_rgbPixel.r);
    EXPECT_EQ(m_rgbPixel.raw[2], m_rgbPixel.b);
}

TEST_F(RgbPixelTest, SetColor) {
    unsigned color = 0x123456;

    m_rgbPixel = color;

    EXPECT_EQ(0x12, m_rgbPixel.r);
    EXPECT_EQ(0x34, m_rgbPixel.g);
    EXPECT_EQ(0x56, m_rgbPixel.b);
}

TEST_F(RgbPixelTest, GetColor) {
    m_rgbPixel.r = 0x98;
    m_rgbPixel.g = 0x76;
    m_rgbPixel.b = 0x54;

    EXPECT_EQ(0x987654, m_rgbPixel);
}
/*****************************************************************************/

/*****************************************************************************/
class HsvPixelTest : public ::testing::Test {

public:
    HsvPixelTest() {
    }

    virtual ~HsvPixelTest() {
    }

protected:
    Pixel::HSV   m_hsvPixel;
};
/*****************************************************************************/

/*****************************************************************************/
TEST_F(HsvPixelTest, DefaultValues) {
    EXPECT_EQ(0, m_hsvPixel.h);
    EXPECT_EQ(0, m_hsvPixel.s);
    EXPECT_EQ(0, m_hsvPixel.v);
}

TEST_F(HsvPixelTest, RawLayout) {
    m_hsvPixel.h = 0x1234;
    m_hsvPixel.s = 2;
    m_hsvPixel.v = 3;

    EXPECT_EQ(m_hsvPixel.raw[0], (m_hsvPixel.h >> 0) & 0xFF);
    EXPECT_EQ(m_hsvPixel.raw[1], (m_hsvPixel.h >> 8) & 0xFF);
    EXPECT_EQ(m_hsvPixel.raw[2], m_hsvPixel.s);
    EXPECT_EQ(m_hsvPixel.raw[3], m_hsvPixel.v);
}

TEST_F(HsvPixelTest, SetColor) {
    unsigned color = 0x123456;

    m_hsvPixel = color;

    EXPECT_EQ(0x3456,   m_hsvPixel.h);
    EXPECT_EQ(255,      m_hsvPixel.s);
    EXPECT_EQ(255,      m_hsvPixel.v);
}
/*****************************************************************************/

/*****************************************************************************/
class Hsv2RgbConversionTest : public ::testing::Test {

public:
    Hsv2RgbConversionTest() {
    }

    virtual ~Hsv2RgbConversionTest() {
    }

protected:
    Pixel::RGB   m_rgbPixel;
    Pixel::HSV   m_hsvPixel;


    struct Converstion_s {
        uint16_t    m_hsvDeg;
        uint32_t    m_rgbHex;
    };

    static constexpr Converstion_s m_conversion[] = {
        {   .m_hsvDeg   =   0,    .m_rgbHex = 0xFF0000 },
        {   .m_hsvDeg   =  30,    .m_rgbHex = 0xFF7F00 },
        {   .m_hsvDeg   =  60,    .m_rgbHex = 0xFFFF00 },
        {   .m_hsvDeg   =  90,    .m_rgbHex = 0x80FF00 },
        {   .m_hsvDeg   = 120,    .m_rgbHex = 0x00FF00 },
        {   .m_hsvDeg   = 150,    .m_rgbHex = 0x00FF7F },
        {   .m_hsvDeg   = 180,    .m_rgbHex = 0x00FFFF },
        {   .m_hsvDeg   = 210,    .m_rgbHex = 0x0080FF },
        {   .m_hsvDeg   = 240,    .m_rgbHex = 0x0000FF },
        {   .m_hsvDeg   = 270,    .m_rgbHex = 0x7F00FF },
        {   .m_hsvDeg   = 300,    .m_rgbHex = 0xFF00FF },
        {   .m_hsvDeg   = 330,    .m_rgbHex = 0xFF0080 },
    };
};
/*****************************************************************************/

/*****************************************************************************/
TEST_F(Hsv2RgbConversionTest, Red) {
    m_hsvPixel = 0;

    m_rgbPixel = m_hsvPixel;

    EXPECT_EQ(0xFF0000u, m_rgbPixel);
}

TEST_F(Hsv2RgbConversionTest, Green) {
    m_hsvPixel = (120.0f / 360.0f) * ((1 << 16) - 1);

    m_rgbPixel = m_hsvPixel;

    EXPECT_EQ(0x00FF00u, m_rgbPixel);
}

TEST_F(Hsv2RgbConversionTest, Blue) {
    m_hsvPixel = (240.0f / 360.0f) * ((1 << 16) - 1);

    m_rgbPixel = m_hsvPixel;

    EXPECT_EQ(0x0000FFu, m_rgbPixel);
}

TEST_F(Hsv2RgbConversionTest, Table) {
    for (unsigned idx = 0; idx < sizeof(m_conversion) / sizeof(m_conversion[0]); idx++) {
        float       hsv = m_conversion[idx].m_hsvDeg;
        uint32_t    rgb = m_conversion[idx].m_rgbHex;

        m_hsvPixel = (hsv / 360.0f) * ((1 << 16) - 1);

        m_rgbPixel = m_hsvPixel;

        EXPECT_EQ(rgb, m_rgbPixel) << "idx=" << idx;
    }
}
/*****************************************************************************/
