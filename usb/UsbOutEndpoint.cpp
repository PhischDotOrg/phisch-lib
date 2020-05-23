/*-
 * $Copyright$
-*/

#include <usb/UsbOutEndpoint.hpp>
#include <usb/UsbHwOutEndpoint.hpp>
#include <usb/UsbControlPipe.hpp>
#include <assert.h>
#include <stddef.h>

namespace usb {

#if 0
/*******************************************************************************
 *
 ******************************************************************************/
UsbOutEndpoint::UsbOutEndpoint(uint32_t * const p_buffer, const size_t p_lenInBytes)
  : m_dataStageBuffer {
      /* .m_buffer = */ p_buffer,
      /* .m_numWords = */ p_lenInBytes / sizeof(uint32_t)
  }
{

}
#endif

#if 0
/*******************************************************************************
 *
 ******************************************************************************/
UsbOutEndpoint::~UsbOutEndpoint() {
}
#endif

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */
