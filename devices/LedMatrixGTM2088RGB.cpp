/*-
 * $Copyright$
-*/

#ifndef _LED_MATRIX_GTM2088RGB_CPP_408b88c2_b3ae_457a_9080_35feffeb13cb
#define _LED_MATRIX_GTM2088RGB_CPP_408b88c2_b3ae_457a_9080_35feffeb13cb

#include "LedMatrixGTM2088RGB.hpp"
#include <strings.h>

namespace devices {

/*******************************************************************************
 *
 ******************************************************************************/
template<unsigned t_width, unsigned t_height>
LedMatrixGtn2088RgbBaseT<t_width, t_height>::LedMatrixGtn2088RgbBaseT(void) {
    ::bzero(m_columns, sizeof(m_columns));
}

/*******************************************************************************
 *
 ******************************************************************************/
template<unsigned t_width, unsigned t_height>
int
LedMatrixGtn2088RgbBaseT<t_width, t_height>::setPixel(const unsigned p_column, const unsigned p_row,
  const RgbChannel_t p_channel, const bool p_pixel) {
    int rc = this->validatePixelAddress(p_column, p_row);

    if (!rc) {
        ColVector_t &col = this->getColumn(p_column, p_channel);
        this->setPixelInColumn(col, p_row, p_pixel);
    }

    return (rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<unsigned t_width, unsigned t_height>
int
LedMatrixGtn2088RgbBaseT<t_width, t_height>::getPixel(const unsigned p_column, const unsigned p_row,
  const RgbChannel_t p_channel, bool &p_pixel) const {
    int rc = this->validatePixelAddress(p_column, p_row);

    if (!rc) {
        const ColVector_t &col = this->getColumn(p_column, p_channel);
        this->getPixelFromColumn(col, p_row, p_pixel);
    }

    return (rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<unsigned t_width, unsigned t_height>
int
LedMatrixGtn2088RgbBaseT<t_width, t_height>::setColumn(const unsigned p_column,
  const RgbChannel_t p_channel, const ColVector_t &p_vector) {
    int rc = this->validatePixelAddress(p_column, 0);

    if (!rc) {
        ColVector_t &column = this->getColumn(p_column, p_channel);
        column = p_vector;
    }
    
    return (rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<unsigned t_width, unsigned t_height>
int
LedMatrixGtn2088RgbBaseT<t_width, t_height>::getColumn(const unsigned p_column,
  const RgbChannel_t p_channel, ColVector_t &p_vector) const {
    int rc = this->validatePixelAddress(p_column, 0);

    if (!rc) {
        p_vector = this->getColumn(p_column, p_channel);
    }

    return (rc);
}

/*******************************************************************************
 *
 ******************************************************************************/
template<typename ShiftRegisterT>
int
LedMatrixGtn2088Rgb_8x8_ViaShiftRegisterT<ShiftRegisterT>::refresh(const unsigned p_column, const RgbChannel_t p_channel) {
    int rc = this->validatePixelAddress(p_column, 0);

    if (!rc) {
        uint32_t * const buffer = reinterpret_cast<uint32_t *>(this->m_buffer);
        *buffer = 0;

        LedMatrixGtn2088RgbBase_t::ColVector_t &col = this->getColumn(p_column, p_channel);

        this->m_buffer[3] = col;
        
        switch (p_channel) {
        case LedMatrixGtn2088RgbBase_t::e_RgbChannel_Red:
            this->m_buffer[0] |= (1 << p_column);
            break;
        case LedMatrixGtn2088RgbBase_t::e_RgbChannel_Green:
            this->m_buffer[2] |= (1 << p_column);
            break;
        case LedMatrixGtn2088RgbBase_t::e_RgbChannel_Blue:
            this->m_buffer[1] |= (1 << p_column);
            break;
        }
        
        rc = this->m_sr.write(this->m_buffer, sizeof(this->m_buffer));
    }

    return (rc);
}

} /* namespace devices */

#endif /* _LED_MATRIX_GTM2088RGB_CPP_408b88c2_b3ae_457a_9080_35feffeb13cb */
