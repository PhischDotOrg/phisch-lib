/*-
 * $Copyright$
-*/

#ifndef _WS2812BSTRIP_HPP_bf8fb134_a27f_4d1a_9806_5477a515cf28
#define _WS2812BSTRIP_HPP_bf8fb134_a27f_4d1a_9806_5477a515cf28

#include <stdint.h>
#include <spi/SpiDevice.hpp>

/*******************************************************************************
 *
 ******************************************************************************/
namespace FastLED {
    #define DUMMY_ARG
    #include "pixeltypes.h"
    #undef DUMMY_ARG
} /* namespace FastLED */

namespace devices {

/*******************************************************************************
 *
 ******************************************************************************/
template<unsigned t_nPixels, typename SpiDeviceT = spi::SpiDevice>
class Ws2812bStripT {
    friend class Ws2812bStripTest;

public:
    typedef FastLED::CRGB Rgb_t;

    static constexpr unsigned SIZE = t_nPixels;
    static size_t size() { return SIZE; }
    
    Ws2812bStripT(SpiDeviceT &p_spiDevice)
      : m_spiDevice(p_spiDevice) {
        for (unsigned idx = 0; idx < sizeof(this->m_spiData); idx++) {
            this->m_spiData[idx] = WS2812B_RESET;
        }
    }

    ~Ws2812bStripT() {

    }

    void    setPixel(const unsigned p_number, const Rgb_t &p_data);
    Rgb_t   getPixel(const unsigned p_number);

    int show(void) const {
        ssize_t len = SIZE_OF_SPI_DATA * 8;

        /*
         * Shift the data out over the SPI engine. Note that we don't need to
         * drive the select pins, but we can do it w/o significant cost (the
         * SPI Device Class requires a pin anyways). Doing so enables us to sniff
         * the SPI data easily on an oscilloscope.
         */
        m_spiDevice.select();
        ssize_t actual = m_spiDevice.shift(len, this->m_spiData);
        m_spiDevice.shift(sizeof(WS2812B_RESET) * 8, &WS2812B_RESET);
        m_spiDevice.deselect();
        return (len != actual);
    }

public:
    SpiDeviceT &    m_spiDevice;

private:
    static const uint8_t    WS2812B_RESET;//   = 0xFF;
    static const uint8_t    WS2812B_ON;   //   = 0x01;
    static const uint8_t    WS2812B_OFF;  //   = 0x07;

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

template<unsigned t_nPixels, typename SpiDeviceT> const uint8_t Ws2812bStripT<t_nPixels, SpiDeviceT>::WS2812B_RESET = 0xFF;
template<unsigned t_nPixels, typename SpiDeviceT> const uint8_t Ws2812bStripT<t_nPixels, SpiDeviceT>::WS2812B_ON    = 0x01;
template<unsigned t_nPixels, typename SpiDeviceT> const uint8_t Ws2812bStripT<t_nPixels, SpiDeviceT>::WS2812B_OFF   = 0x07;

} /* namespace devices */

#include "Ws2812bStrip.cpp"

#endif /* _WS2812BSTRIP_HPP_bf8fb134_a27f_4d1a_9806_5477a515cf28 */
