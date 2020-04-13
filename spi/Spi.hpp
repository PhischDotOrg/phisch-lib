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

} /* namespace spi */

#endif /* _SPI_HPP_ec22a57c_684b_4653_9717_d58eab215715 */
