/*-
 * $Copyright$
-*/

#ifndef _USBTYPES_HPP_6F681E9D_BBA0_4F3F_A86C_C5464EF17DB6
#define _USBTYPES_HPP_6F681E9D_BBA0_4F3F_A86C_C5464EF17DB6

#include <array>
#include <cstdint>

#if defined(USB_DEBUG)
#include <phisch/log.h>

#define USB_PRINTF(...)  PHISCH_LOG("[USB] " __VA_ARGS__)
#else
#define USB_PRINTF(...)  do { } while (0)
#endif /* defined(USB_DEBUG) */

#include "f1usb/src/usb_desc.hh"

namespace usb {

typedef struct UsbDeviceDescriptor_s UsbDeviceDescriptor_t;
typedef struct UsbDeviceQualifierDescriptor_s UsbDeviceQualifierDescriptor_t;

/******************************************************************************/
static constexpr const unsigned MAX_CTRL_PACKET_SIZE = 64;
/******************************************************************************/


/*******************************************************************************
 *
 ******************************************************************************/
typedef enum UsbDescriptorTypeId_e : uint8_t {
    e_Device                = 0x01,
    e_Configuration         = 0x02,
    e_String                = 0x03,
    e_Interface             = 0x04,
    e_Endpoint              = 0x05,
    e_DeviceQualifier       = 0x06,
    e_OtherSpeedConfig      = 0x07,
    // e_InterfaceAssociation  = 0x0B,
} UsbDescriptorTypeId_t;

#if defined(__cplusplus)
static_assert(sizeof(UsbDescriptorTypeId_e) == 1u);
#endif /* defined(__cplusplus) */

/*******************************************************************************
 *
 ******************************************************************************/
typedef enum UsbInterfaceClass_e : uint8_t {
    e_UsbInterface_BaseClass                = 0x00,
    e_UsbInterface_AudioDevice              = 0x01,
    e_UsbInterface_CommunicationDeviceClass = 0x02,
    e_UsbInterface_HumanInterfaceDevice     = 0x03,
    e_UsbInterface_PhysicalDevice           = 0x05,
    e_UsbInterface_StillImagingDevice       = 0x06,
    e_UsbInterface_Printer                  = 0x07,
    e_UsbInterface_MassStorageDevice        = 0x08,
    e_UsbInterface_Hub                      = 0x09,
    e_UsbInterface_CdcData                  = 0x0A,
    e_UsbInterface_SmartCard                = 0x0B,
    e_UsbInterface_ContentSecurityDevice    = 0x0D,
    e_UsbInterface_VideoDevice              = 0x0E,
    e_UsbInterface_PersonalHealthcareDevice = 0x0F,
    e_UsbInterface_AudioVideoDevice         = 0x10,
    e_UsbInterface_TypeC_Bridge             = 0x12,
    e_UsbInterface_DiagnosticDevice         = 0xDC,
    e_UsbInterface_WirelessController       = 0xE0,
    e_UsbInterface_Misc                     = 0xE0,
    e_UsbInterface_Misc_EFh                 = 0xEF,
    e_UsbInterface_ApplicationSpecific      = 0xFE,
    e_UsbInterface_VendorSpecific           = 0xFF,
} UsbInterfaceClass_t;
static_assert(sizeof(UsbInterfaceClass_t) == 1);

/***************************************************************************//**
 * \brief USB Endpoint Descriptor
 * 
 * This structure defines the Endpoint Descriptor as per Table #9-13
 * "Standard Endpoint Descriptor"
 * of the
 * "Universal Serial Bus Specification"
 * Revision 2.0 from Apr 27, 2000
 ******************************************************************************/
struct UsbEndpointDescriptor {
    /**
     * \brief Size of this descriptor in bytes.
     * 
     * Should be 7 Bytes for USB 2.0
     */
    uint8_t                 m_bLength;

    /**
     * \brief Endpoint Descriptor Type.
     * 
     * Should be ::usb::UsbDescriptorTypeId_e::e_Endpoint for USB 2.0
     */
    UsbDescriptorTypeId_e   m_bDescriptorType;

    /**
     * \brief The address of the endpoint on the USB device described by this descriptor.
     * 
     * The address is encoded as follows:
     * - Bit 3...0: The endpoint number
     * - Bit 6...4: Reserved, reset to zero
     * - Bit 7: Direction, ignored for control endpoints
     *   + 0 = OUT endpoint
     *   + 1 = IN endpoint
     */
    uint8_t                 m_bEndpointAddress;
    /**
     * \brief This field describes the endpoint’s attributes when it is configured using the bConfigurationValue.
     * 
     * - Bits 1..0: Transfer Type
     *   + b00 = Control
     *   + b01 = Isochronous
     *   + b10 = Bulk
     *   + b11 = Interrupt
     *
     * If not an isochronous endpoint, bits 5..2 are reserved and must be set to zero.
     * If isochronous, they are defined as follows:
     * - Bits 3..2: Synchronization Type
     *   + b00 = No Synchronization
     *   + b01 = Asynchronous
     *   + b10 = Adaptive
     *   + b11 = Synchronous
     *
     * - Bits 5..4: Usage Type
     *   + b00 = Data endpoint
     *   + b01 = Feedback endpoint
     *   + b10 = Implicit feedback Data endpoint
     *   + b11 = Reserved
     */
    uint8_t                 m_bmAttributes;

    /**
     * \brief Maximum Packet Size (in Bytes).
     * 
     * Maximum packet size this endpoint is capable of sending or receiving when this configuration is selected.
     * 
     * - Bits 10..0: Maximum Packet Size (in Bytes).
     */
    struct {
      uint8_t               m_loByte;
      uint8_t               m_hiByte;
    }                       m_wMaxPacketSize;
    uint8_t                 m_bInterval;
} __attribute__((packed));

#if defined(__cplusplus)
static_assert(sizeof(UsbEndpointDescriptor) == 7);
#endif /* defined(__cplusplus) */

/*******************************************************************************
 * USB Interface Descriptor -- 9 Bytes
 ******************************************************************************/
template<unsigned nEndpoints>
struct UsbInterfaceDescriptorT {
    uint8_t                         m_bLength;
    uint8_t                         m_bDescriptorType;
    uint8_t                         m_bInterfaceNumber;
    uint8_t                         m_bAlternateSetting;
    uint8_t                         m_bNumEndpoints;
    UsbInterfaceClass_t             m_bInterfaceClass;
    uint8_t                         m_bInterfaceSubClass;
    uint8_t                         m_bInterfaceProtocol;
    uint8_t                         m_iInterface;
    ::usb::UsbEndpointDescriptor    m_endpoints[nEndpoints];
} __attribute__((packed));
static_assert(sizeof(struct ::usb::UsbInterfaceDescriptorT<0>) == 9);

/*******************************************************************************
 * USB Configuration Descriptor -- 9 Bytes 
 ******************************************************************************/
template <typename UsbInterfaceDescriptorT, unsigned nInterfaces = 1>
struct UsbConfigurationDescriptorT {
    uint8_t                         m_bLength;
    uint8_t                         m_bDescriptorType;
    struct {
      uint8_t                       m_loByte;
      uint8_t                       m_hiByte;
    }  m_wTotalLength;
    uint8_t                         m_bNumInterfaces;
    uint8_t                         m_bConfigurationValue;
    uint8_t                         m_iConfiguration;
    uint8_t                         m_bmAttributes;
    uint8_t                         m_bMaxPower;
    UsbInterfaceDescriptorT         m_interfaces[nInterfaces];
} __attribute__((packed));
static_assert(sizeof(struct UsbConfigurationDescriptorT<void *, 0>) == 9);

typedef ::usb::UsbConfigurationDescriptorT<void *, 0> UsbConfigurationDescriptor_t;

/*******************************************************************************
 *
 ******************************************************************************/
struct UsbDeviceDescriptor_s {
    uint8_t     m_bLength;
    uint8_t     m_bDescriptorType;
    uint8_t     m_bcdUsb[2];
    uint8_t     m_bDeviceClass;
    uint8_t     m_bDeviceSubClass;
    uint8_t     m_bDeviceProtocol;
    uint8_t     m_bMaxPacketSize0;
    uint8_t     m_idVendor[2];
    uint8_t     m_idProduct[2];
    uint8_t     m_bcdDevice[2];
    uint8_t     m_iManufacturer;
    uint8_t     m_iProduct;
    uint8_t     m_iSerialNumber;
    uint8_t     m_bNumConfigurations;
} __attribute__((packed));

/*******************************************************************************
 *
 ******************************************************************************/
struct UsbDeviceQualifierDescriptor_s {
    uint8_t     m_bLength;
    uint8_t     m_bDescriptorType;
    uint8_t     m_bcdUsb[2];
    uint8_t     m_bDeviceClass;
    uint8_t     m_bDeviceSubClass;
    uint8_t     m_bDeviceProtocol;
    uint8_t     m_bMaxPacketSize0;
    uint8_t     m_bNumConfigurations;
    uint8_t     m_bReserved;
    
    constexpr UsbDeviceQualifierDescriptor_s(const UsbDeviceDescriptor_t &p_deviceDescriptor)
      : m_bLength(p_deviceDescriptor.m_bLength),
      m_bDescriptorType(e_DeviceQualifier),
      m_bcdUsb { p_deviceDescriptor.m_bcdUsb[0], m_bcdUsb[1] = p_deviceDescriptor.m_bcdUsb[1] },
      m_bDeviceClass(p_deviceDescriptor.m_bDeviceClass),
      m_bDeviceSubClass(p_deviceDescriptor.m_bDeviceSubClass),
      m_bDeviceProtocol(p_deviceDescriptor.m_bDeviceProtocol),
      m_bMaxPacketSize0(p_deviceDescriptor.m_bMaxPacketSize0),
      m_bNumConfigurations(p_deviceDescriptor.m_bNumConfigurations),
      m_bReserved(0)
    {
    };

private:
    UsbDeviceQualifierDescriptor_s();
} __attribute__((packed));

/*******************************************************************************
 *
 ******************************************************************************/
typedef enum UsbStringDescriptorId_e {
    e_StrDesc_Min           = 0x00,
    e_StrDesc_LanguageId    = 0x00,
    e_StrDesc_Manufacturer  = 0x01,
    e_StrDesc_Product       = 0x02,
    e_StrDesc_SerialNumber  = 0x03,
    e_StrDesc_Configuration = 0x04,
    e_StrDesc_Interface     = 0x05,
    e_StrDesc_Max           = 0x06
} UsbStringDescriptorId_t;

/******************************************************************************/
using UsbStringDescriptor_t = const uint8_t * const;

static constexpr
std::size_t
getUsbStrLen(const std::size_t p_length) {
    return (2 + 2 * (sizeof(uint8_t) * (p_length - 1)));
}

template <typename CharT, size_t LengthT>
constexpr
std::array<uint8_t, getUsbStrLen(LengthT)>
UsbStringDescriptor (const CharT (&p_str) [LengthT]) {
    /*
     * First Byte of String is length of Descriptor in Bytes. Hence, the
     * Length must not exceed 255 Bytes.
     */
    static_assert(getUsbStrLen(LengthT) < 256);

    return Util::encodeMulti (
            static_cast<uint8_t> (getUsbStrLen(LengthT)),
            UsbDescriptorTypeId_t::e_String,
            Util::encodeString (p_str)
    );
}
/******************************************************************************/

/******************************************************************************/
using UsbLangId_t = uint16_t;

static_assert(sizeof(UsbLangId_t) == 2);
/******************************************************************************/

/******************************************************************************/
using UsbLangIdStringDescriptor_t = const uint8_t * const;
/******************************************************************************/

/******************************************************************************/
union UsbStringDescriptors_u {
    struct UsbStringDescriptorTable_s {
        const UsbLangIdStringDescriptor_t   m_languageIds;
        const UsbStringDescriptor_t         m_manufacturer;
        const UsbStringDescriptor_t         m_product;
        const UsbStringDescriptor_t         m_serialNumber;
        const UsbStringDescriptor_t         m_configuration;
        const UsbStringDescriptor_t         m_interface;
    } __attribute__((aligned(sizeof(uint32_t))))    m_strings;
    const UsbStringDescriptor_t             m_array[e_StrDesc_Max];

    static_assert(sizeof(struct UsbStringDescriptorTable_s) == sizeof(m_array));
} __attribute__((aligned(sizeof(uint32_t))));

using UsbStringDescriptors_t = UsbStringDescriptors_u;
/******************************************************************************/

/******************************************************************************/
template <size_t LengthT, typename UsbLangIdT>
constexpr
std::array<uint8_t, 2 + LengthT * sizeof(UsbLangIdT)>
UsbLangIdStringDescriptor (const UsbLangIdT (&p_langIds) [LengthT]) {
    static_assert(LengthT > 0);
    static_assert(LengthT < 256);   /* USB "only" allows us to announce 255 Languages */

    return Util::encodeMulti (
            static_cast<uint8_t> (2 + LengthT),
            UsbDescriptorTypeId_t::e_String,
            Util::encode(p_langIds)
    );
}
/******************************************************************************/

/*******************************************************************************
 * Typedefs for Setup Packet
 ******************************************************************************/
struct UsbSetupPacket_s {
    uint8_t m_bmRequestType;
    uint8_t m_bRequest;
    uint16_t m_wValue;
    uint16_t m_wIndex;
    uint16_t m_wLength;

#if defined(__cplusplus)
    /**
     * @brief Allows word-wise Access via operator[] to Setup Packet Buffer.
     * 
     * This allows ::usb::stm32f4::CtrlOutEndpointViaSTM32F4::setupDataReceivedDeviceCallback to
     * copy the received setup data into the structure.
     * 
     * @param p_idx Word-offset within Setup Packet Buffer.
     * @return constexpr uint32_t& Reference to Word that is stored at offset \p p_idx from Setup Packet.
     */
    uint32_t &operator[](const int p_idx) {
        uint32_t * const ptr = reinterpret_cast<uint32_t *>(this);

        return ptr[p_idx];
    };

    /**
     * @brief Allows Half-Word wise Access via operator[] to Setup Packet Buffer.
     * 
     * This allows ::usb::stm32f4::CtrlOutEndpointViaSTM32F4::setupDataReceivedDeviceCallback to
     * copy the received setup data into the structure.
     * 
     * @param p_idx Half-Word Offset within Setup Packet Buffer.
     * @return constexpr uint16_t& Reference to Word that is stored at offset \p p_idx from Setup Packet.
     */
    uint16_t & getUint16(const int p_idx) {
        uint16_t * const ptr = reinterpret_cast<uint16_t *>(this);

        return ptr[p_idx];
    };
#endif
} __attribute__((packed, aligned(4)));
typedef struct UsbSetupPacket_s UsbSetupPacket_t;

static_assert(sizeof(UsbSetupPacket_t) == 8);

/***************************************************************************//**
 * \brief Communications Class Subclass Codes
 * 
 * This enum is according to Table #4 "Class Subclass Code" of the  Universal
 * Serial Bus Class Definitions for Communications Devices Specification
 * Revision 1.2 (Errata 1) from Nov 3rd, 2010.
 ******************************************************************************/
enum UsbCdc_SubclassCode_e : uint8_t {
    e_UsbCdcSubclass_Reserved                   = 0x00,
    e_UsbCdcSubclass_DirectLineControl          = 0x01,
    e_UsbCdcSubclass_AbstractControl            = 0x02,
    e_UsbCdcSubclass_TelephoneControl           = 0x03,
    e_UsbCdcSubclass_MultiChannelControl        = 0x04,
    e_UsbCdcSubclass_CapiControl                = 0x05,
    e_UsbCdcSubclass_EthernetNetworkingControl  = 0x06,
    e_UsbCdcSubclass_AtmNetworkingControl       = 0x07,
    e_UsbCdcSubclass_WirelessHandsetControl     = 0x08,
    e_UsbCdcSubclass_DeviceManagement           = 0x09,
    e_UsbCdcSubclass_ModileDirectLine           = 0x0A,
    e_UsbCdcSubclass_OBEX                       = 0x0B,
    e_UsbCdcSubclass_EthernetEmulation          = 0x0C,
    e_UsbCdcSubclass_NetworkControl             = 0x0D,
    e_UsbCdcSubclass_VendorSpecific             = 0xFF,
};
#if defined(__cplusplus)
static_assert(sizeof(UsbCdc_SubclassCode_e) == 1u);
#endif /* defined(__cplusplus) */

/***************************************************************************//**
 * \brief Communications Class Protocol Codes
 * 
 * This enum is according to Table #5 "Communications Interface Class Control
 * Protocol Code" of the  Universal Serial Bus Class Definitions for Communications
 * Devices Specification Revision 1.2 (Errata 1) from Nov 3rd, 2010.
 ******************************************************************************/
enum UsbCdc_ProtocolCode_e : uint8_t {
    e_UsbCdcProto_None                  = 0x00,
    e_UsbCdcProto_AT_V250               = 0x01,
    e_UsbCdcProto_AT_PCCA101            = 0x02,
    e_UsbCdcProto_AT_PCCA101_Annex      = 0x03,
    e_UsbCdcProto_AT_GSM                = 0x04,
    e_UsbCdcProto_AT_3GPP               = 0x05,
    e_UsbCdcProto_AT_CDMA               = 0x06,
    e_UsbCdcProto_USB_EthernetEmulation = 0x07,
    e_UsbCdcProto_External              = 0xFE,
    e_UsbCdcProto_VendorSpecific        = 0xFF,
};
#if defined(__cplusplus)
static_assert(sizeof(UsbCdc_ProtocolCode_e) == 1u);
#endif /* defined(__cplusplus) */

/***************************************************************************//**
 * \brief USB Communication Device Class (CDC) Descriptor Type
 * 
 * This template structure defines the Functional Descriptor Type values as per
 * Table #12 "Type Values for the bDescriptorType Field" of the Universal Serial
 * Bus Class Definitions for Communications Devices Revision 1.2 (Errata 1) from
 * Nov 3rd, 2010.
 ******************************************************************************/
enum UsbCdcFunctionalDescriptorType_e : uint8_t {
    e_UsbDec_DescrType_Interface    = 0x24,
    e_UsbDec_DescrType_Endpoint     = 0x25,
};
#if defined(__cplusplus)
static_assert(sizeof(UsbCdcFunctionalDescriptorType_e) == 1u);
#endif /* defined(__cplusplus) */

/***************************************************************************//**
 * \brief USB Communication Device Class (CDC) Descriptor Subtype
 * 
 * This template structure defines the Functional Descriptor Subtype values as per
 * Table #13 "bDescriptor SubType in Communications Class Functional Descriptors"
 * of the Universal Serial Bus Class Definitions for Communications Devices
 * Revision 1.2 (Errata 1) from Nov 3rd, 2010.
 ******************************************************************************/
enum UsbCdcFunctionalDescriptorSubtype_e : uint8_t {
    e_UsbDec_DescrSubtype_Header                = 0x00,
    e_UsbDec_DescrSubtype_CallMgmt              = 0x01,
    e_UsbDec_DescrSubtype_AbstractControlMgmt   = 0x02,
    e_UsbDec_DescrSubtype_Union                 = 0x06,
    /* TODO Add the remaining values from the spec. */
    e_UsbDec_DescrSubtype_Invalid               = 0xff,
};
#if defined(__cplusplus)
static_assert(sizeof(UsbCdcFunctionalDescriptorSubtype_e) == 1u);
#endif /* defined(__cplusplus) */

/***************************************************************************//**
 * \brief USB Communication Device Class (CDC) Functional Descriptor: Base Type
 * 
 * This template structure defines the Functional Descriptor as per Table #11
 * "Functional Descriptor General Format" of  the Universal Serial Bus Class
 * Definitions for Communications Devices Revision 1.2 (Errata 1) from Nov 3rd,
 * 2010.
 ******************************************************************************/
struct UsbCdc_FunctDescr_Base_s {
    uint8_t                             m_bFunctionLength;
    UsbCdcFunctionalDescriptorType_e    m_bDescriptorType;
    UsbCdcFunctionalDescriptorSubtype_e m_bDescriptorSubtype;
} __attribute__((packed));

#if defined(__cplusplus)
static_assert(sizeof(struct UsbCdc_FunctDescr_Base_s) == 3u);
#endif /* defined(__cplusplus) */


/***************************************************************************//**
 * \brief USB Communication Device Class (CDC) Functional Descriptor: Header
 * 
 * This template structure defines the Functional Descriptor as per Table #15
 * "Class-Specific Descriptor Header Format"
 * of the
 * "Universal Serial Bus Class Definitions for Communications Devices"
 * Revision 1.2 (Errata 1) from Nov 3rd, 2010.
 ******************************************************************************/
struct UsbCdc_FunctDescr_Header_s {
    struct UsbCdc_FunctDescr_Base_s m_cdcFncDescr;
    struct {
        uint8_t                     m_loByte;
        uint8_t                     m_hiByte;
    }                               m_bcdCDC;
} __attribute__((packed));

#if defined(__cplusplus)
static_assert(sizeof(struct UsbCdc_FunctDescr_Header_s) == 5u);
#endif /* defined(__cplusplus) */

/***************************************************************************//**
 * \brief USB Communication Device Class (CDC) Functional Descriptor: Union
 * 
 * This template structure defines the Functional Descriptor as per Table #16
 * "Union Interface Functional Descriptor"
 * of the
 * "Universal Serial Bus Class Definitions for Communications Devices"
 * Revision 1.2 (Errata 1) from Nov 3rd, 2010.
 ******************************************************************************/
template<unsigned nInterfaces>
struct UsbCdc_FunctDescr_UnionT {
    struct UsbCdc_FunctDescr_Base_s m_cdcFncDescr;
    uint8_t                         m_bControlInterface;
    uint8_t                         m_subordinateInterface[nInterfaces];
} __attribute__((packed));

#if defined(__cplusplus)
static_assert(sizeof(UsbCdc_FunctDescr_UnionT<0>) == 4u);
static_assert(sizeof(UsbCdc_FunctDescr_UnionT<1>) == 5u);
#endif /* defined(__cplusplus) */

/***************************************************************************//**
 * \brief USB Communication Device Class (CDC) Functional Descriptor: Call Management
 * 
 * This template structure defines the Functional Descriptor as per Table #3
 * "Call Management Functional Descriptor"
 * of the
 * "Universal Serial Bus Communications Class Subclass Specification for PSTN Devices"
 * Revision 1.2 from Feb 9, 2007
 ******************************************************************************/
struct UsbCdc_FunctDescr_CallMgmt_s {
    struct UsbCdc_FunctDescr_Base_s m_cdcFncDescr;
    /**
     * \brief Bitfield as per USB Specifictation.
     *
     * - D1:
     *  + 0 - Device sends/receives call management information only over the Communications Class interface.
     *  + 1 - Device can send/receive call management information over a Data Class interface.
     * 
     * - D0:
     *  + 0 - Device does not handle call management itself.
     *  + 1 - Device handles call management itself.
     */
    uint8_t                         m_bmCapabilities;
    uint8_t                         m_bDataInterface;
} __attribute__((packed));

#if defined(__cplusplus)
static_assert(sizeof(struct UsbCdc_FunctDescr_CallMgmt_s) == 5u);
#endif /* defined(__cplusplus) */

/***************************************************************************//**
 * \brief USB Communication Device Class (CDC) Functional Descriptor: Abstract Control Management (ACM)
 * 
 * This template structure defines the Functional Descriptor as per Table #4
 * "Abstract Control Management Functional Descriptor"
 * of the
 * "Universal Serial Bus Communications Class Subclass Specification for PSTN Devices"
 * Revision 1.2 from Feb 9, 2007
 ******************************************************************************/
struct UsbCdc_FunctDescr_ACM_s {
    struct UsbCdc_FunctDescr_Base_s m_cdcFncDescr;
    /**
     * \brief Bitfield as per USB Specifictation.
     *
     * The capabilities that this configuration supports. (A bit value of zero means that the request is not supported.)
     * - D7..D4: RESERVED (Reset to zero)
     * - D3
     *  + 1 - Device supports the notification Network_Connection.
     * - D2
     *  + 1 - Device supports the request Send_Break
     * - D1
     *  + 1 - Device supports the request combination of Set_Line_Coding, Set_Control_Line_State, Get_Line_Coding, and the notification Serial_State.
     * - D0
     *  + 1 - Device supports the request combination of Set_Comm_Feature, Clear_Comm_Feature, and Get_Comm_Feature.
     */
    uint8_t                         m_bmCapabilities;
} __attribute__((packed));

#if defined(__cplusplus)
static_assert(sizeof(struct UsbCdc_FunctDescr_ACM_s) == 4u);
#endif /* defined(__cplusplus) */


/***************************************************************************//**
 * \brief USB 2.0 Interface Association Descriptor
 *
 * This structure defines the Interface Association Descriptor as per Table #9-Z
 * "Standard Interface Association Descriptor"
 * of the USB Engineering Change Notice (ECN) titled
 * "Interface Association Descriptors"
 *****************************************************************************/
struct UsbInterfaceAssociationDescriptor_s {
    uint8_t m_bLength;          /** Size of this descriptor in bytes. */
    uint8_t m_bDescriptorType;
    uint8_t m_bFirstInterface;
    uint8_t m_bInterfaceCount;
    uint8_t m_bFunctionClass;
    uint8_t m_bFunctionSubClass;
    uint8_t m_bFunctionProtocol;
    uint8_t m_iFunction;
} __attribute__((packed));

#if defined(__cplusplus)
static_assert(sizeof(struct UsbInterfaceAssociationDescriptor_s) == 8u);
#endif /* defined(__cplusplus) */

/***************************************************************************//**
 * \brief "Get Status" Response for USB 2.0 Devices
 *
 * This structure defines the Response to a _Get Status_ Control Command Response
 * for USB Devices as per
 * Figure #9-4
 * "Information Returned by a GetStatus() Request to a Device"
 * of the
 * "Universal Serial Bus Specification"
 * Revision 2.0 from April 27, 2000
 *****************************************************************************/
union UsbDeviceStatus_u {
    uint16_t        m_status;
    struct {
        uint16_t    m_selfPowered   : 1;
        uint16_t    m_remoteWakeup  : 1;
        uint16_t    m_reserved      : 14;
    } m_bitfield;
} __attribute__((packed));

typedef union UsbDeviceStatus_u UsbDeviceStatus_t;

#if defined(__cplusplus)
static_assert(sizeof(UsbDeviceStatus_t) == 2u);
#endif /* defined(__cplusplus) */

/***************************************************************************//**
 * \brief USB HID 1.11 Subclass Codes
 *
 * This enum defines the HID Subclass Codes as per Section 4.2
 * "Subclass"
 * of the USB Specification titled
 * "Device Class Definition for Human Interface Devices (HID)"
 * from May 27th, 2001.
 *****************************************************************************/
enum UsbHid_SubclassCode_e : uint8_t {
    e_NoSubclass    = 0,
    e_BootInterface = 1
};

/***************************************************************************//**
 * \brief USB HID 1.11 Protocol Codes
 *
 * This enum defines the HID Protocol Codes as per Section 4.3
 * "Protocols"
 * of the USB Specification titled
 * "Device Class Definition for Human Interface Devices (HID)"
 * from May 27th, 2001.
 *****************************************************************************/
enum UsbHid_ProtocolCode_e : uint8_t {
    e_None      = 0,
    e_Keyboard  = 1,
    e_Mouse     = 2
};

/***************************************************************************//**
 * \brief USB HID 1.11 Country Code
 *
 * This enum defines the HID Country Code as per Section 6.2.1
 * "HID Descriptor"
 * of the USB Specification titled
 * "Device Class Definition for Human Interface Devices (HID)"
 * from May 27th, 2001.
 *****************************************************************************/
enum UsbHidCountryCode_e : uint8_t {
    e_NotSupported      = 0,
    e_Arabic            = 1,
    e_Belgian           = 2,
    e_CanadianBilingual = 3,
    e_CanadianFrench    = 4,
    e_CzechRepublic     = 5,
    e_Danish            = 6,
    e_Finnish           = 7,
    e_French            = 8,
    e_German            = 9,
    e_Greek             = 10,
    e_Hebrew            = 11,
    e_Hungary           = 12,
    e_International     = 13,
    e_Italian           = 14,
    e_Japan             = 15,
    e_Korean            = 16,
    e_LatinAmerican     = 17,
    e_NetherlandsDutch  = 18,
    e_Norwegian         = 19,
    e_Persian           = 20,
    e_Poland            = 21,
    e_Portuguese        = 22,
    e_Russia            = 23,
    e_Slovakia          = 24,
    e_Spanish           = 25,
    e_Swedish           = 26,
    e_SwissFrench       = 27,
    e_SwissGerman       = 28,
    e_Switzerland       = 29,
    e_Taiwan            = 30,
    e_TurkishQ          = 31,
    e_UK                = 32,
    e_US                = 33,
    e_Yugoslavia        = 34,
    e_TurkishF          = 35
};

#if defined(__cplusplus)
static_assert(sizeof(UsbHidCountryCode_e) == 1u);
#endif /* defined(__cplusplus) */

/***************************************************************************//**
 * \brief USB HID 1.11 Descriptor
 *
 * These values reflect the Class Descriptor Types as per Section 7.1
 * "Standard Requests"
 * of the USB Specification titled
 * "Device Class Definition for Human Interface Devices (HID)"
 * from May 27th, 2001.
 *****************************************************************************/
enum UsbHidDescriptorTypeId_e : uint8_t {
    e_Hid                   = 0x21,
    e_HidReport             = 0x22,
    e_HidPhysical           = 0x23,
};

#if defined(__cplusplus)
static_assert(sizeof(UsbHidDescriptorTypeId_e) == 1u);
#endif /* defined(__cplusplus) */

/***************************************************************************//**
 * \brief USB HID 1.11 Descriptor
 *
 * This structure defines the HID Descriptor as per Section 6.2.1
 * "HID Descriptor"
 * of the USB Specification titled
 * "Device Class Definition for Human Interface Devices (HID)"
 * from May 27th, 2001.
 *****************************************************************************/
struct UsbHidDescriptor {
    uint8_t m_bLength;                              /** @brief Size of this descriptor in bytes. */
    UsbHidDescriptorTypeId_e    m_bDescriptorType;  /** @brief Type of HID descriptor. */
    struct {
      uint8_t   m_loByte;
      uint8_t   m_hiByte;
    }  m_bcdHID;                        /** @brief HID Class Specification release. */
    UsbHidCountryCode_e m_bCountryCode; /** @brief Country code of the localized hardware. */
    uint8_t m_bNumDescriptors;          /** @brief Number of class descriptors, must be > 0. */
    uint8_t m_bDescriptorTypeName;      /** @brief Constant name identifying type of class descriptor. */
    struct {
      uint8_t   m_loByte;
      uint8_t   m_hiByte;
    } m_wDescriptorLength;          /** @brief Total Size fo the Descriptor, incl. Report Descriptor(s). */
} __attribute__((packed));

#if defined(__cplusplus)
static_assert(sizeof(struct UsbHidDescriptor) == 9u);
#endif /* defined(__cplusplus) */

/*******************************************************************************
 *
 ******************************************************************************/
} /* namespace usb */

#endif /* _USBTYPES_HPP_6F681E9D_BBA0_4F3F_A86C_C5464EF17DB6 */
