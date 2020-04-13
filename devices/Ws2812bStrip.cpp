/*-
 * $Copyright$
-*/

#ifndef _WS2812BSTRIP_CPP_c699985f_00a7_46ad_953a_1fab3b14d316
#define _WS2812BSTRIP_CPP_c699985f_00a7_46ad_953a_1fab3b14d316

#include "Ws2812bStrip.hpp"

/*
 * The WS2812B wants the data in the following format:
 * 
 * MSB                          LSB  MSB                          LSB  MSB                          LSB
 * [G7][G6][G5][G4][G3][G2][G1][G0]  [R7][R6][R5][R4][R3][R2][R1][R0]  [G7][G6][G5][G4][G3][G2][G1][G0]
 * 
 */
namespace devices {

/*******************************************************************************
 *
 ******************************************************************************/
template<unsigned t_lengthInBits, typename SpiDeviceT>
void
Ws2812bStripT<t_lengthInBits, SpiDeviceT>::setPixel(const unsigned p_number, const Rgb_t &p_data) {
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

/*******************************************************************************
 *
 ******************************************************************************/
template<unsigned t_lengthInBits, typename SpiDeviceT>
typename Ws2812bStripT<t_lengthInBits, SpiDeviceT>::Rgb_t
Ws2812bStripT<t_lengthInBits, SpiDeviceT>::getPixel(const unsigned /* p_number */) {
    return 0;
}

} /* namespace devices */

#endif /* _WS2812BSTRIP_CPP_c699985f_00a7_46ad_953a_1fab3b14d316 */
    