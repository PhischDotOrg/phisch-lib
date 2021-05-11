
/*-
 * $Copyright$
-*/

#include "usb/UsbDescriptors.hpp"
#include <usb/UsbTypes.hpp>

/*****************************************************************************/
namespace usb {
    namespace descriptors {
        namespace vendor {
/*****************************************************************************/

extern const ::usb::UsbDeviceDescriptor_t usbDeviceDescriptor FIXED_DATA = {
    .m_bLength              = sizeof(::usb::UsbDeviceDescriptor_t),                     /* m_bLength */
    .m_bDescriptorType      = ::usb::UsbDescriptorTypeId_t::e_Device,                   /* m_bDescriptorType */
    .m_bcdUsb               = { 0x00, 0x02 },                                           /* m_bcdUsb */
    .m_bDeviceClass         = ::usb::UsbInterfaceClass_e::e_UsbInterface_VendorSpecific,/* m_bDeviceClass */
    .m_bDeviceSubClass      = 0x02,                                                     /* m_bDeviceSubClass */
    .m_bDeviceProtocol      = 0x01,                                                     /* m_bDeviceProtocol */
    .m_bMaxPacketSize0      = 64,                                                       /* m_bMaxPacketSize0 -- Should be 64 for a USB Full Speed Device. */
    .m_idVendor             = { 0xad, 0xde },                                           /* m_idVendor */
    .m_idProduct            = { 0xef, 0xbe },                                           /* m_idProduct */
    .m_bcdDevice            = { 0xfe, 0xca },                                           /* m_bcdDevice */
    .m_iManufacturer        = ::usb::UsbStringDescriptorId_t::e_StrDesc_Manufacturer,   /* e_iManufacturer */
    .m_iProduct             = ::usb::UsbStringDescriptorId_t::e_StrDesc_Product,        /* e_iProduct */
    .m_iSerialNumber        = ::usb::UsbStringDescriptorId_t::e_StrDesc_SerialNumber,   /* e_iSerialNumber */
    .m_bNumConfigurations   = 1                                                         /* e_bNumConfigurations */
};

/*******************************************************************************
 * USB Device String Descriptors
 ******************************************************************************/
static constexpr ::usb::UsbLangId_t usbSupportedLanguageIds[] = { 0x0409, 0x0000 };
static const auto usbLangIdStringDescriptor         FIXED_DATA  = ::usb::UsbLangIdStringDescriptor(usbSupportedLanguageIds);
static const auto usbStringDescriptorManufacturer   FIXED_DATA  = ::usb::UsbStringDescriptor(u"PhiSch.org");
static const auto usbStringDescriptorSerialNumber   FIXED_DATA  = ::usb::UsbStringDescriptor(u"5D4DFA36-8E3E-44CD-96BC-26074E3CA6B6");
static const auto usbStringDescriptorProduct        FIXED_DATA  = ::usb::UsbStringDescriptor(u"USB Test Application on " STM32_BOARD " with " STM32_CPU_TYPE);
static const auto usbStringDescriptorConfiguration  FIXED_DATA  = ::usb::UsbStringDescriptor(u"PhiSch.org USB Vendor Interface Configuration");
static const auto usbStringDescriptorInterface      FIXED_DATA  = ::usb::UsbStringDescriptor(u"PhiSch.org USB Vendor Interface");

extern const ::usb::UsbStringDescriptors_t usbStringDescriptors FIXED_DATA = {
    .m_strings = {
        .m_languageIds      = usbLangIdStringDescriptor.data(),
        .m_manufacturer     = usbStringDescriptorManufacturer.data(),
        .m_product          = usbStringDescriptorProduct.data(),
        .m_serialNumber     = usbStringDescriptorSerialNumber.data(),
        .m_configuration    = usbStringDescriptorConfiguration.data(),
        .m_interface        = usbStringDescriptorInterface.data()
    }
};

extern const struct UsbConfigurationDescriptor_s {
    struct ::usb::UsbConfigurationDescriptorT<void *, 0>    m_configDescrHdr;
    struct ::usb::UsbInterfaceDescriptorT<2>                m_dataInterface;
} __attribute__((packed)) usbVendorTestConfigurationDescriptor FIXED_DATA = {
    .m_configDescrHdr = {
        .m_bLength                  = sizeof( decltype(usbVendorTestConfigurationDescriptor.m_configDescrHdr)),
        .m_bDescriptorType          = ::usb::UsbDescriptorTypeId_e::e_Configuration,
        .m_wTotalLength = {
            .m_loByte               = (sizeof(decltype(usbVendorTestConfigurationDescriptor)) >> 0),
            .m_hiByte               = (sizeof(decltype(usbVendorTestConfigurationDescriptor)) >> 8)
        },
        .m_bNumInterfaces           = 1,
        .m_bConfigurationValue      = 1,
        .m_iConfiguration           = 4, /* Index of m_configuration within usbStringDescriptors.m_stringDescriptorTable */
        .m_bmAttributes             = 0x80      // USB 2.0 Spec demands this Bit to always be set
                                    | 0x40,     // Set to 0x40 for Self-powered Device, 0x00 for Bus-powered
        .m_bMaxPower                = 5,        // Power consumption in Units of 2mA
        .m_interfaces               = {}
    },
    .m_dataInterface = {
        .m_bLength                  = sizeof(decltype(usbVendorTestConfigurationDescriptor.m_dataInterface)) - sizeof(usbVendorTestConfigurationDescriptor.m_dataInterface.m_endpoints),
        .m_bDescriptorType          = ::usb::UsbDescriptorTypeId_e::e_Interface,
        .m_bInterfaceNumber         = 0,
        .m_bAlternateSetting        = 0,
        .m_bNumEndpoints            = 2,
        .m_bInterfaceClass          = ::usb::UsbInterfaceClass_e::e_UsbInterface_VendorSpecific,
        .m_bInterfaceSubClass       = 0x10, /* Magic Number for Test Code to Identify the Loopback Interface */
        .m_bInterfaceProtocol       = 0x0b, /* Magic Number for Test Code to Identify the Loopback Interface */
        .m_iInterface               = 5, /* Index of m_interface within usbStringDescriptors.m_stringDescriptorTable */
        .m_endpoints                = {
            /* Index #0 */ {
                .m_bLength          = sizeof(decltype(usbVendorTestConfigurationDescriptor.m_dataInterface.m_endpoints[0])),
                .m_bDescriptorType  = ::usb::UsbDescriptorTypeId_e::e_Endpoint,
                .m_bEndpointAddress = 0x01, /* OUT */
                .m_bmAttributes     = 2,    // Bulk
                .m_wMaxPacketSize = {
                    .m_loByte       = 64,
                    .m_hiByte       = 0
                },
                .m_bInterval        = 0
            },
            /* Index #1 */ {
                .m_bLength          = sizeof(decltype(usbVendorTestConfigurationDescriptor.m_dataInterface.m_endpoints[1])),
                .m_bDescriptorType  = ::usb::UsbDescriptorTypeId_e::e_Endpoint,
                .m_bEndpointAddress = 0x81, /* IN */
                .m_bmAttributes     = 2,    // Bulk
                .m_wMaxPacketSize = {
                    .m_loByte       = 64,
                    .m_hiByte       = 0
                },
                .m_bInterval        = 0
            }
        }
    }    
};

const ::usb::UsbConfigurationDescriptor_t * const usbConfigurationDescriptor FIXED_DATA = reinterpret_cast<const ::usb::UsbConfigurationDescriptor_t *>(&usbVendorTestConfigurationDescriptor);

/*****************************************************************************/
        } /* namespace vendor */
    } /* namespace descriptors */
} /* namespace usb */
/*****************************************************************************/
