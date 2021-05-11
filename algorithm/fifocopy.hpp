/*-
 * $Copyright$
 */

#ifndef _FIFOCOPY_HPP_03F980FB_ECA3_4CAC_98C7_287C37363F1C
#define _FIFOCOPY_HPP_03F980FB_ECA3_4CAC_98C7_287C37363F1C

/******************************************************************************/
namespace stm32 {
/******************************************************************************/

template<
    size_t fifoWidth,
    typename InputItT,
    typename OutputItT
>
size_t
copy_from_fifo(const InputItT p_inputBegin, const InputItT p_inputEnd, OutputItT p_outputBegin, OutputItT p_outputEnd) {
    static_assert(1 == sizeof(typename std::iterator_traits<OutputItT>::value_type));

    auto i = p_inputBegin;
    auto o = p_outputBegin;
    typename std::iterator_traits<InputItT>::value_type v;

    constexpr auto fifo_width = fifoWidth;

    if ((p_inputBegin >= p_inputEnd) || (p_outputBegin >= p_outputEnd)) return  0;

    while (i != p_inputEnd) {
        static_assert(
                 (fifo_width == sizeof(uint8_t))
              || (fifo_width == sizeof(uint16_t))
              || (fifo_width == sizeof(uint32_t))
        );

        v = *i++;

        if constexpr(fifo_width >= sizeof(uint8_t)) {
            *(o) = (v >> 0) & 0xFF;
            if ((++o) == p_outputEnd) break;
        }

        if constexpr(fifo_width >= sizeof(uint16_t)) {
            *(o) = (v >> 8) & 0xFF;
            if ((++o) == p_outputEnd) break;
        }

        if constexpr(fifo_width >= sizeof(uint32_t)) {
            *(o) = (v >> 16) & 0xFF;
            if ((++o) == p_outputEnd) break;

            *(o) = (v >> 24) & 0xFF;
            if ((++o) == p_outputEnd) break;
        }

        if constexpr(fifo_width > 4) {
            static_assert(fifo_width == 0, "FIFO Width beyond 32-Bit / 4 Bytes not implemented.");
        }
    };

    return o - p_outputBegin;
}

template<
    size_t fifoWidth,
    typename InputT,
    typename OutputT
>
size_t
copy_from_fifo(const InputT &p_input, OutputT &p_output, size_t p_bytes) {
    static_assert(1 == sizeof(typename OutputT::value_type));
    return copy_from_fifo<fifoWidth>(p_input.cbegin(), p_input.cend(), p_output.begin(), std::min(p_output.end(), p_output.begin() + p_bytes));
}

template<
    size_t fifoWidth,
    typename InputItT,
    typename OutputItT
>
size_t
copy_to_fifo(const InputItT p_inputBegin, const InputItT p_inputEnd, OutputItT p_outputBegin, OutputItT p_outputEnd) {
    static_assert(1 == sizeof(typename std::iterator_traits<InputItT>::value_type));

    auto i = p_inputBegin;
    auto o = p_outputBegin;

    constexpr auto fifo_width = fifoWidth;

    if ((p_inputBegin >= p_inputEnd) || (p_outputBegin >= p_outputEnd)) return (0);

    while (o != p_outputEnd) {
        static_assert(
                 (fifo_width == sizeof(uint8_t))
              || (fifo_width == sizeof(uint16_t))
              || (fifo_width == sizeof(uint32_t))
        );

        typename std::iterator_traits<OutputItT>::value_type &v = *o++;
        
        if constexpr(fifo_width >= sizeof(uint8_t)) {
            v = *i;
            if ((++i) == p_inputEnd) break;
        }

        if constexpr(fifo_width >= sizeof(uint16_t)) {
            v = (*i << 8) | (v & 0x00FFu);
            if ((++i) == p_inputEnd) break;
        }

        if constexpr(fifo_width >= sizeof(uint32_t)) {
            v = (*i << 16) | (v & 0x0000FFFFu);
            if ((++i) == p_inputEnd) break;

            v = (*i << 24) | (v & 0x00FFFFFFu);
            if ((++i) == p_inputEnd) break;
        }

        if constexpr(fifo_width > 4) {
            static_assert(fifo_width == 0, "FIFO Width beyond 32-Bit / 4 Bytes not implemented.");
        }
    }

    return i - p_inputBegin;
}

template<
    size_t fifoWidth,
    typename InputT,
    typename OutputT
>
size_t
copy_to_fifo(const InputT &p_input, OutputT &p_output, size_t p_bytes) {
    return copy_to_fifo<fifoWidth>(p_input.cbegin(), std::min(p_input.cend(), p_input.cbegin() + p_bytes), p_output.begin(), p_output.end());
}

/******************************************************************************/
} /* namespace stm32 */
/******************************************************************************/

#endif /* _FIFOCOPY_HPP_03F980FB_ECA3_4CAC_98C7_287C37363F1C */
