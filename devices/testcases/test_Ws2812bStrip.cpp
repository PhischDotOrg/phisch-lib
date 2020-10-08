/*-
 * $Copyright$
-*/

#include <gmock/gmock.h>

#include <devices/Ws2812bStrip.hpp>
#include <spi/SpiDevice.hpp>

namespace devices {

/*******************************************************************************
 *
 ******************************************************************************/
class Ws2812bStripTest : public ::testing::Test {
protected:
    static const unsigned m_length = 3;
    
    typedef Ws2812bStripT<m_length, spi::SpiDeviceMock> Ws2812bStrip;

    static constexpr uint8_t WS2812B_ON     = Ws2812bStrip::WS2812B_ON;
    static constexpr uint8_t WS2812B_OFF    = Ws2812bStrip::WS2812B_OFF;

    static const unsigned NUMBER_OF_WS2812B_BITS_PER_PIXEL;
    static const unsigned SPI_BYTES_PER_WS2812B_COLOR;
    static const unsigned SPI_BYTES_PER_WS2812B_PIXEL;

    Ws2812bStrip *          m_device;
    spi::SpiDeviceMock      m_spi;

    size_t getSizeOfSpiData(void) const {
        return sizeof(m_device->m_spiData);
    }
    
    void testConstants(void) const {
        EXPECT_EQ(4u, m_device->SPI_BYTES_PER_WS2812B_COLOR);
    }

    uint8_t getSpiByte(const unsigned p_offs) const {
        return this->m_device->m_spiData[p_offs];
    }

public:
    Ws2812bStripTest() : m_device(NULL) {
        m_device = new Ws2812bStrip(m_spi);
    }

    virtual ~Ws2812bStripTest() {
        delete m_device;
    }
};

const unsigned Ws2812bStripTest::NUMBER_OF_WS2812B_BITS_PER_PIXEL  = Ws2812bStripTest::Ws2812bStrip::NUMBER_OF_WS2812B_BITS_PER_PIXEL;
const unsigned Ws2812bStripTest::SPI_BYTES_PER_WS2812B_COLOR       = Ws2812bStripTest::Ws2812bStrip::SPI_BYTES_PER_WS2812B_COLOR;
const unsigned Ws2812bStripTest::SPI_BYTES_PER_WS2812B_PIXEL       = Ws2812bStripTest::Ws2812bStrip::SPI_BYTES_PER_WS2812B_PIXEL;

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(Ws2812bStripTest, CreateAndDelete) {
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(Ws2812bStripTest, Constants) {
    EXPECT_EQ(24u, NUMBER_OF_WS2812B_BITS_PER_PIXEL);
    EXPECT_EQ(4u, SPI_BYTES_PER_WS2812B_COLOR);
    EXPECT_EQ(12u, SPI_BYTES_PER_WS2812B_PIXEL);
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(Ws2812bStripTest, SizesOfDataTypes) {
    EXPECT_EQ(3u, sizeof(Pixel::RGB));
    EXPECT_EQ(this->m_length * 3 * (8/2), getSizeOfSpiData());
}

/*******************************************************************************
 *
 ******************************************************************************/
TEST_F(Ws2812bStripTest, SetFirstPixel) {
    const uint8_t red = 0x12;   // [7:0] = b00010010
    const uint8_t green = 0x34; // [7:0] = b00110100
    const uint8_t blue = 0x56;  // [7:0] = b01010110

    const unsigned number = 0;
    const unsigned base = 12 * number;

    Pixel::RGB pixel(red, green, blue);

    m_device->setPixel(number, pixel);
    
    /* Validate Green Bits */
    EXPECT_EQ((WS2812B_OFF << 4) | (WS2812B_OFF), this->getSpiByte(base + 0 + 0));
    EXPECT_EQ((WS2812B_ON  << 4) | (WS2812B_ON ), this->getSpiByte(base + 0 + 1));
    EXPECT_EQ((WS2812B_OFF << 4) | (WS2812B_ON ), this->getSpiByte(base + 0 + 2));
    EXPECT_EQ((WS2812B_OFF << 4) | (WS2812B_OFF), this->getSpiByte(base + 0 + 3));

    /* Validate Red Bits */
    EXPECT_EQ((WS2812B_OFF << 4) | (WS2812B_OFF), this->getSpiByte(base + 4 + 0));
    EXPECT_EQ((WS2812B_OFF << 4) | (WS2812B_ON ), this->getSpiByte(base + 4 + 1));
    EXPECT_EQ((WS2812B_OFF << 4) | (WS2812B_OFF), this->getSpiByte(base + 4 + 2));
    EXPECT_EQ((WS2812B_ON  << 4) | (WS2812B_OFF), this->getSpiByte(base + 4 + 3));

    /* Validate Blue Bits */
    EXPECT_EQ((WS2812B_OFF << 4) | (WS2812B_ON ), this->getSpiByte(base + 8 + 0));
    EXPECT_EQ((WS2812B_OFF << 4) | (WS2812B_ON ), this->getSpiByte(base + 8 + 1));
    EXPECT_EQ((WS2812B_OFF << 4) | (WS2812B_ON ), this->getSpiByte(base + 8 + 2));
    EXPECT_EQ((WS2812B_ON  << 4) | (WS2812B_OFF), this->getSpiByte(base + 8 + 3));
}

} /* namespace devices */
