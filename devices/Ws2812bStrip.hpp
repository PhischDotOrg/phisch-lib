/*-
 * $Copyright$
-*/

#ifndef _WS2812BSTRIP_HPP_bf8fb134_a27f_4d1a_9806_5477a515cf28
#define _WS2812BSTRIP_HPP_bf8fb134_a27f_4d1a_9806_5477a515cf28

#include <stdint.h>
#include <spi/SpiDevice.hpp>

/*****************************************************************************/
#include "Pixel.hpp"
/*****************************************************************************/

/*****************************************************************************/
namespace devices {
/*****************************************************************************/

/*****************************************************************************/
struct Ws2812bDataNonInverted {
    static constexpr uint8_t WS2812B_RESET  = 0x00;
    static constexpr uint8_t WS2812B_ON     = 0x07;
    static constexpr uint8_t WS2812B_OFF    = 0x01;
};
/*****************************************************************************/

/*****************************************************************************/
struct Ws2812bDataInverted {
    static constexpr uint8_t WS2812B_RESET  = 0xFF;
    static constexpr uint8_t WS2812B_ON     = 0x01;
    static constexpr uint8_t WS2812B_OFF    = 0x07;
};
/*****************************************************************************/

/*****************************************************************************/
template<
    unsigned t_nPixels,
    typename SpiDeviceT = spi::SpiDevice,
    typename Ws2812bDataT = Ws2812bDataInverted
>
class Ws2812bStripT : private Ws2812bDataT {
    friend class Ws2812bStripTest;

public:
    static constexpr unsigned SIZE = t_nPixels;
    static size_t size() { return SIZE; }
    
    Ws2812bStripT(SpiDeviceT &p_spiDevice)
      : m_spiDevice(p_spiDevice) {
        static_assert(sizeof(Pixel::RGB) == 3, "Expected Pixel Size to be 3 Bytes");

        for (unsigned idx = 0; idx < sizeof(this->m_spiData); idx++) {
            this->m_spiData[idx] = Ws2812bDataT::WS2812B_RESET;
        }
    }

    void
    setPixel(const unsigned p_number, const Pixel::RGB &p_data) {
        /*
        * The WS2812B wants the data in the following format:
        * 
        * MSB                          LSB  MSB                          LSB  MSB                          LSB
        * [G7][G6][G5][G4][G3][G2][G1][G0]  [R7][R6][R5][R4][R3][R2][R1][R0]  [B7][B6][B5][B4][B3][B2][B1][B0]
        * 
        */
        for (unsigned color = 0; color < this->NUMBER_OF_WS2812B_COLORS; color++) {
            for (int bit = this->NUMBER_OF_WS2812B_BITS_PER_COLOR - 1; bit >= 0;  bit--) {
                // Offset of the entire Pixel
                unsigned offs = p_number * this->SPI_BYTES_PER_WS2812B_PIXEL;

                // Offset of the color w/i the Pixel
                offs += color * this->SPI_BYTES_PER_WS2812B_COLOR;
                
                // Offset of the bit w/i the color
                offs += ((NUMBER_OF_WS2812B_BITS_PER_COLOR - bit) - 1) /  NUMBER_OF_WS2812B_BITS_PER_SPI_BYTE; 

                // Value to be written to the SPI byte
                const uint8_t value = (p_data.raw[color] & (1 << bit)) ? this->WS2812B_ON : this->WS2812B_OFF;

                switch (bit % NUMBER_OF_WS2812B_BITS_PER_SPI_BYTE) {
                case 0:
                    this->m_spiData[offs] &= ~0x0F;
                    this->m_spiData[offs] |= value;
                    break;
                case 1:
                    this->m_spiData[offs] &= ~0xF0;
                    this->m_spiData[offs] |= (value << 4);
                    break;
                default:
                    assert(false);
                }
            }
        }
    }

    const Pixel &getPixel(const unsigned p_number) const;

    int
    show(void) const {
        ssize_t len = SIZE_OF_SPI_DATA * 8;

        /*
         * Shift the data out over the SPI engine. Note that we don't need to
         * drive the select pins, but we can do it w/o significant cost (the
         * SPI Device Class requires a pin anyways). Doing so enables us to sniff
         * the SPI data easily on an oscilloscope.
         */
        m_spiDevice.select();
        ssize_t actual = m_spiDevice.shift(len, this->m_spiData);
        m_spiDevice.shift(sizeof(this->WS2812B_RESET) * 8, &this->WS2812B_RESET);
        m_spiDevice.deselect();
        return (len != actual);
    }

    void
    shutdown(void) {
        for (unsigned idx = 0; idx < t_nPixels; idx++) {
            this->setPixel(idx, Pixel::RGB(0));
        }
        show();
    }

public:
    SpiDeviceT &    m_spiDevice;

private:
    /*
     * We need to hold the data we want to push out through SPI in a static
     * buffer, so we can use DMA to shift it. The amount of space we require is
     * in bytes:
     * 
     *  Number of Pixels * (Bits per Pixel / 2)
     * 
     * because we need four SPI bits to shift one WS2812B bit, or put the other
     * way: We can store two WS2812B bits in one byte of SPI data.
     */
    static const unsigned       NUMBER_OF_WS2812B_BITS_PER_SPI_BYTE     = 2;
    static const unsigned       NUMBER_OF_WS2812B_BITS_PER_COLOR        = 8;
    static const unsigned       NUMBER_OF_WS2812B_COLORS                = 3;
    static constexpr unsigned   NUMBER_OF_WS2812B_BITS_PER_PIXEL        = NUMBER_OF_WS2812B_BITS_PER_COLOR * NUMBER_OF_WS2812B_COLORS;
    static constexpr unsigned   SPI_BYTES_PER_WS2812B_COLOR             = NUMBER_OF_WS2812B_BITS_PER_COLOR / NUMBER_OF_WS2812B_BITS_PER_SPI_BYTE;
    static constexpr unsigned   SPI_BYTES_PER_WS2812B_PIXEL             = NUMBER_OF_WS2812B_BITS_PER_PIXEL / NUMBER_OF_WS2812B_BITS_PER_SPI_BYTE;

    static constexpr unsigned   SIZE_OF_SPI_DATA                        = t_nPixels * SPI_BYTES_PER_WS2812B_PIXEL;

    uint8_t m_spiData[SIZE_OF_SPI_DATA];
};
/*****************************************************************************/

/*****************************************************************************/
} /* namespace devices */
/*****************************************************************************/

#endif /* _WS2812BSTRIP_HPP_bf8fb134_a27f_4d1a_9806_5477a515cf28 */
