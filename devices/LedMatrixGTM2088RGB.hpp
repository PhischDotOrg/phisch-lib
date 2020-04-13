/*-
 * $Copyright$
-*/

#ifndef _LED_MATRIX_GTM2088RGB_HPP_135f86b5_3004_43e0_8db6_a89ea608355b
#define _LED_MATRIX_GTM2088RGB_HPP_135f86b5_3004_43e0_8db6_a89ea608355b

#include <stdint.h>
#include <errno.h>

namespace devices {

/*******************************************************************************
 *
 ******************************************************************************/
namespace LedMatrixGtn2088RgbTemplates {

#if !defined(DOXYGEN)
    /*
     * Template Meta-Programming Example from Wikibooks, via URL below on July 13th, 2013:
     *   http://en.wikibooks.org/wiki/C%2B%2B_Programming/Templates/Template_Meta-Programming#Example:_Compile-time_.22If.22
     *
     * This code is used to determine the width of the parameters to the write() and
     * read() methods below.
     */
    template <bool Condition, typename TrueResult, typename FalseResult>
    struct if_;

    template <typename TrueResult, typename FalseResult>
    struct if_<true, TrueResult, FalseResult>
    {
      typedef TrueResult m_result;
    };

    template <typename TrueResult, typename FalseResult>
    struct if_<false, TrueResult, FalseResult>
    {
      typedef FalseResult m_result;
    };

    template<unsigned t_height> struct ColVectorChoiceT {
        typedef typename if_<(t_height > sizeof(uint8_t) * 8),
            typename if_<(t_height > sizeof(uint16_t) * 8),
              typename if_<(t_height > sizeof(uint32_t) * 8),
                typename if_<(t_height > sizeof(uint64_t) * 8),
                  uint32_t[(t_height + 31) / 32], uint64_t>::m_result,
              uint32_t>::m_result,
            uint16_t>::m_result,
          uint8_t>::m_result ColVector_t;
    };

    template<unsigned t_width> struct RowVectorChoiceT {
        typedef typename if_<(t_width > sizeof(uint8_t) * 8),
            typename if_<(t_width > sizeof(uint16_t) * 8),
              typename if_<(t_width > sizeof(uint32_t) * 8),
                typename if_<(t_width > sizeof(uint64_t) * 8),
                  uint32_t[(t_width + 31) / 32], uint64_t>::m_result,
              uint32_t>::m_result,
            uint16_t>::m_result,
          uint8_t>::m_result RowVector_t;
    };
#endif /* !defined(DOXYGEN) */
} /* namespace LedMatrixGtn2088RgbTemplates */

/*******************************************************************************
 *
 ******************************************************************************/
template<unsigned t_width = 8, unsigned t_height = 8>
class LedMatrixGtn2088RgbBaseT {
    friend class LedMatrixGTM2088RGBBaseTest;

public:
#if defined(DOXYGEN)
    /*!
     * @brief Container for Pixels in a single Column.
     * 
     * The type \c ColVector_t is defined as an unsigned integer type that can
     * hold at least \c t_height bits.
     */
    typedef unsigned ColVector_t;

    /*!
     * @brief Container for Pixels in a single Row.
     * 
     * The type \c RowVector_t is defined as an unsigned integer type that can
     * hold at least \c t_width bits.
     */
    typedef unsigned RowVector_t;
#else
    typedef typename LedMatrixGtn2088RgbTemplates::ColVectorChoiceT<t_height>::ColVector_t ColVector_t;
    typedef typename LedMatrixGtn2088RgbTemplates::RowVectorChoiceT<t_width>::RowVector_t RowVector_t;
#endif /* defined(DOXYGEN) */

    typedef enum RgbChannel_e {
        e_RgbChannel_Red    = 0,
        e_RgbChannel_Green  = 1,
        e_RgbChannel_Blue   = 2,
    } RgbChannel_t;

    static unsigned getWidth(void) { return m_width; }
    static unsigned getHeight(void) { return m_height; }

    int setRow(const unsigned p_row, const RgbChannel_t p_channel, const RowVector_t &p_vector);
    int getRow(const unsigned p_row, const RgbChannel_t p_channel, RowVector_t &p_vector) const;

    int setColumn(const unsigned p_column, const RgbChannel_t p_channel, const ColVector_t &p_vector);
    int getColumn(const unsigned p_column, const RgbChannel_t p_channel, ColVector_t &p_vector) const;

    int setPixel(const unsigned p_column, const unsigned p_row, const RgbChannel_t p_channel, const bool p_pixel);
    int getPixel(const unsigned p_column, const unsigned p_row, const RgbChannel_t p_channel, bool &p_pixel) const;

    static const unsigned m_width = t_width;

protected:
    static const unsigned m_height = t_height;
    static const unsigned m_nColors = 3;

    LedMatrixGtn2088RgbBaseT(void);
    
    ~LedMatrixGtn2088RgbBaseT() {
    };
    
    int validatePixelAddress(const unsigned p_column, const unsigned p_row) const {
        return (p_column < this->m_width) && (p_row < this->m_height) ? 0 : -1;
    }
    
    ColVector_t &getColumn(const unsigned p_row, const RgbChannel_t p_channel) {
        return this->m_columns[p_channel][p_row];
    }
    const ColVector_t &getColumn(const unsigned p_row, const RgbChannel_t p_channel) const {
        return this->m_columns[p_channel][p_row];
    }
    
private:
    /*!
     * @brief Container for Pixels.
     * 
     * The array holds the pixels, organized as bits in a width x height
     * matrix.
     * 
     * The type \c ColVector_t is defined as an unsigned integer type that can
     * hold at least \c t_height bits. Therefore, the pixels of a single columns
     * of the display are stored in a single \c ColVector_t.
     * 
     * The size of the array is defined by the width of the display.
     */
    ColVector_t m_columns[m_nColors][t_width];
    
    void setPixelInColumn(ColVector_t &p_column, const unsigned p_row, const bool p_pixel) const {
        /* FIXME This won't work for the vector types! */
        if (p_pixel) {
            p_column |= (1 << p_row);
        } else {
            p_column &= ~(1 << p_row);
        }
    };
    
    void getPixelFromColumn(const ColVector_t &p_column, const unsigned p_row, bool &p_pixel) const {
        /* FIXME This won't work for the vector types! */
        p_pixel = (p_column & (1 << p_row)) != 0;
    };
}; /* class LedMatrixGtn2088RgbBaseT */

/*******************************************************************************
 *
 ******************************************************************************/
template<typename ShiftRegisterT>
class LedMatrixGtn2088Rgb_8x8_ViaShiftRegisterT : public LedMatrixGtn2088RgbBaseT<8, 8> {
    friend class LedMatrixGTM2088RGBViaShiftRegisterTest;

    typedef LedMatrixGtn2088RgbBaseT<8, 8>  LedMatrixGtn2088RgbBase_t;
public:
    typedef typename LedMatrixGtn2088RgbBase_t::RgbChannel_t RgbChannel_t;
    
    LedMatrixGtn2088Rgb_8x8_ViaShiftRegisterT(ShiftRegisterT &p_sr) : m_sr(p_sr) {
    };
    
    ~LedMatrixGtn2088Rgb_8x8_ViaShiftRegisterT() {
    };

    int refresh(const unsigned p_column, const RgbChannel_t p_channel);

private:
    ShiftRegisterT &    m_sr;

    uint8_t             m_buffer[4];
}; /* class LedMatrixGtn2088RgbT */

} /* namespace devices */

#include "LedMatrixGTM2088RGB.cpp"

#endif /* _LED_MATRIX_GTM2088RGB_HPP_135f86b5_3004_43e0_8db6_a89ea608355b */