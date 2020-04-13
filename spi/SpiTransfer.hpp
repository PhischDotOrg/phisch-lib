/*-
 * $Copyright$
-*/

#ifndef _SPI_TRANSFER_HPP_032b50ac_0db3_484e_b3c8_cff805064f43
#define _SPI_TRANSFER_HPP_032b50ac_0db3_484e_b3c8_cff805064f43

#include <stddef.h>
#include <stdint.h>

#include <ostream>

#include <spi/SpiDevice.hpp>
#include <spi/Spi.hpp>

#if defined(DEBUG_BUILD) && defined(GMOCK_FOUND)
#include <gmock/gmock.h>
#endif /* defined(DEBUG_BUILD) && defined(GMOCK_FOUND) */

namespace spi {

/*******************************************************************************
 * 
 ******************************************************************************/
template <typename DeviceT>
class TransferT {
private:
    DeviceT *       m_device;
    std::ostream &  m_ostream;

public:
    TransferT(DeviceT * const p_device, std::ostream &p_ostream);
    ~TransferT();

    ssize_t transfer(size_t p_bits, const uint8_t * const p_buffer, const size_t p_length) const;
}; /* class TransferT */


typedef TransferT<spi::SpiDevice> SpiTransfer;

}; /* namespace spi */

#endif /* _SPI_TRANSFER_HPP_032b50ac_0db3_484e_b3c8_cff805064f43 */