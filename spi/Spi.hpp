/*-
 * $Copyright$
-*/

#ifndef _SPI_HPP_ec22a57c_684b_4653_9717_d58eab215715
#define _SPI_HPP_ec22a57c_684b_4653_9717_d58eab215715

namespace spi {

enum Mode {
    SpiMode0 = 0,   /* CPOL = 0, CPHA = 0 */
    SpiMode1 = 1,   /* CPOL = 0, CPHA = 1 */
    SpiMode2 = 2,   /* CPOL = 1, CPHA = 0 */
    SpiMode3 = 3    /* CPOL = 1, CPHA = 1 */
};

/*****************************************************************************/
template<bool doMultiByteTransferT, typename AccessT> class ShiftHelperT;
/*****************************************************************************/

/*****************************************************************************/
template<typename AccessT>
class ShiftHelperT<false, AccessT> {
public:
    static ssize_t shift(const AccessT * const p_access, uint32_t p_bits, const uint8_t * const p_tx /* = NULL */, uint8_t * const p_rx /* = NULL */, const spi::Mode p_mode) {
        int rc = -1;

        assert(p_bits > 0);

        uint8_t bits, rx = 0, tx;
        uint32_t remain = p_bits, idx = 0;
        do {
            if (remain < 8) {
                bits = remain;
            } else {
                bits = 8;
            }

            if (p_tx != NULL) {
                tx = p_tx[idx];
            } else {
                tx = 0xff;
            }

            rc = p_access->shift(bits, &rx, tx, p_mode);
            if (rc != 0)
                break;

            if (p_rx != NULL)
                p_rx[idx] = rx;

            remain -= bits;
            idx++;
        } while(remain > 0);

        return (p_bits - remain);
    }
};
/*****************************************************************************/

/*****************************************************************************/
template<typename AccessT>
class ShiftHelperT<true, AccessT> {
public:
    static ssize_t shift(AccessT * const p_access, const uint32_t p_bits, const uint8_t * const p_tx /* = NULL */, uint8_t * const p_rx /* = NULL */, const spi::Mode p_mode) {
        ssize_t rc = p_access->shift(p_bits, p_rx, p_tx, p_mode);
        if (rc) {
            rc = -1;
        } else {
            rc = p_bits;
        }
        return (rc);
    }
};
/*****************************************************************************/

/*****************************************************************************/
} /* namespace spi */
/*****************************************************************************/

#endif /* _SPI_HPP_ec22a57c_684b_4653_9717_d58eab215715 */
