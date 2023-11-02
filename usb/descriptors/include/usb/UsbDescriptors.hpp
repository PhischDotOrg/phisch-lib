/*-
 * $Copyright$
-*/

#ifndef _USB_DESCRIPTORS_HPP_169FE118_F65A_416A_BE32_20E2BE835F4E
#define _USB_DESCRIPTORS_HPP_169FE118_F65A_416A_BE32_20E2BE835F4E

#include <usb/UsbTypes.hpp>

#if !defined(HOSTBUILD)
#define FIXED_DATA   __attribute__((aligned(4), section(".fixeddata")))
#else
#define FIXED_DATA
#endif

/*****************************************************************************/
namespace usb {
    namespace descriptors {
/*****************************************************************************/

/*****************************************************************************/
namespace cdc {
/*****************************************************************************/
extern const ::usb::UsbDeviceDescriptor_t usbDeviceDescriptor;
extern const ::usb::UsbStringDescriptors_t usbStringDescriptors;
extern const ::usb::UsbConfigurationDescriptor_t * const usbConfigurationDescriptor;
/*****************************************************************************/
} /* namespace cdc */
/*****************************************************************************/

/*****************************************************************************/
namespace hid {
/*****************************************************************************/
extern const ::usb::UsbDeviceDescriptor_t usbDeviceDescriptor;
extern const ::usb::UsbStringDescriptors_t usbStringDescriptors;
extern const ::usb::UsbConfigurationDescriptor_t * const usbConfigurationDescriptor FIXED_DATA;
extern const uint8_t hidMouseReportDescriptor[50];
/*****************************************************************************/
} /* namespace hid */
/*****************************************************************************/

/*****************************************************************************/
namespace vendor {
/*****************************************************************************/
extern const ::usb::UsbDeviceDescriptor_t usbDeviceDescriptor;
extern const ::usb::UsbStringDescriptors_t usbStringDescriptors;
extern const ::usb::UsbConfigurationDescriptor_t * const usbConfigurationDescriptor;
/*****************************************************************************/
} /* namespace vendor */
/*****************************************************************************/

/*****************************************************************************/
    } /* namespace descriptors */
} /* namespace usb */
/*****************************************************************************/

#endif /* _USB_DESCRIPTORS_HPP_169FE118_F65A_416A_BE32_20E2BE835F4E */
