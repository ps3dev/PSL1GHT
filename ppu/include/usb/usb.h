/*! \file usb.h
 \brief USB low level management.
 */

#ifndef __USB_H__
#define __USB_H__

#include <ppu-types.h>
#include <ppu-asm.h>

/*
 * variable prefix definitions
 *
 * b   = byte ( 8 bits )
 * w   = word ( 16 bits )
 * bm  = bitmap
 * bcd = binary-coded decimal
 * i   = index
 * id  = identifier
 *
 */

/*
 * constants
 */

/* bRequest types */
#define USB_REQTYPE_DIR_BITS                     0x80
#define USB_REQTYPE_DIR_TO_DEVICE                0x00
#define USB_REQTYPE_DIR_TO_HOST                  0x80

#define USB_REQTYPE_TYPE_STANDARD                0x00
#define USB_REQTYPE_TYPE_CLASS                   0x20
#define USB_REQTYPE_TYPE_VENDOR                  0x40
#define USB_REQTYPE_TYPE_RESERVED                0x60
#define USB_REQTYPE_TYPE_BITS                    0x60

#define USB_REQTYPE_RECIP_DEVICE                 0x00
#define USB_REQTYPE_RECIP_INTERFACE              0x01
#define USB_REQTYPE_RECIP_ENDPOINT               0x02
#define USB_REQTYPE_RECIP_OTHER                  0x03
#define USB_REQTYPE_RECIP_BITS                   0x1f

/* bRequest */
#define USB_REQUEST_GET_STATUS                   0x00
#define USB_REQUEST_CLEAR_FEATURE                0x01
#define USB_REQUEST_SET_FEATURE                  0x03
#define USB_REQUEST_SET_ADDRESS                  0x05
#define USB_REQUEST_GET_DESCRIPTOR               0x06
#define USB_REQUEST_SET_DESCRIPTOR               0x07
#define USB_REQUEST_GET_CONFIGURATION            0x08
#define USB_REQUEST_SET_CONFIGURATION            0x09
#define USB_REQUEST_GET_INTERFACE                0x0a
#define USB_REQUEST_SET_INTERFACE                0x0b
#define USB_REQUEST_SYNCH_FRAME                  0x0c

/* bDescriptorType */
#define USB_DESCRIPTOR_TYPE_DEVICE               0x01
#define USB_DESCRIPTOR_TYPE_CONFIG               0x02
#define USB_DESCRIPTOR_TYPE_STRING               0x03
#define USB_DESCRIPTOR_TYPE_INTERFACE            0x04
#define USB_DESCRIPTOR_TYPE_ENDPOINT             0x05
#define USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER     0x06
#define USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG   0x07
#define USB_DESCRIPTOR_TYPE_INTERFACE_POWER      0x08
#define USB_DESCRIPTOR_TYPE_OTG                  0x09
#define USB_DESCRIPTOR_TYPE_DEBUG                0x0a
#define USB_DESCTIPTOR_TYPE_INTERFACE_ASSOC      0x0b
#define USB_DESCRIPTOR_TYPE_HID                  0x21
#define USB_DESCRIPTOR_TYPE_REPORT               0x22
#define USB_DESCRIPTOR_TYPE_PHYSICAL             0x23
#define USB_DESCRIPTOR_TYPE_HUB                  0x29

/* features */
#define USB_FEATURE_ENDPOINT_HALT                0x00
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP         0x01

/* device classes */
#define USB_CLASS_PER_INTERFACE                  0x00
#define USB_CLASS_AUDIO                          0x01
#define USB_CLASS_COMM                           0x02
#define USB_CLASS_HID                            0x03
#define USB_CLASS_MONITOR                        0x04
#define USB_CLASS_PHYSICAL                       0x05
#define USB_CLASS_PTP                            0x06
#define USB_CLASS_PRINTER                        0x07
#define USB_CLASS_STORAGE                        0x08
#define USB_CLASS_HUB                            0x09
#define USB_CLASS_DATA                           0x0a
#define USB_CLASS_CHIP                           0x0b
#define USB_CLASS_SECURITY                       0x0d
#define USB_CLASS_VIDEO                          0x0e
#define USB_CLASS_XBOX                           0x58
#define USB_CLASS_DIAG                           0xdc
#define USB_CLASS_WIRELESS                       0xe0
#define USB_CLASS_MISC                           0xef
#define USB_CLASS_APPLICATION                    0xfe
#define USB_CLASS_VENDOR_SPEC                    0xff

/* iConfiguration */
#define USB_CONFIGURATION_RESERVED_ZERO          0x1f
#define USB_CONFIGURATION_BUS_POWERED            0x00 /* (0<<6) */
#define USB_CONFIGURATION_REMOTE_WAKEUP          0x20 /* (1<<5) */
#define USB_CONFIGURATION_SELF_POWERED           0x40 /* (1<<6) */
#define USB_CONFIGURATION_RESERVED_ONE           0x80

/* bmAttribute */
#define USB_ENDPOINT_TRANSFER_TYPE_BITS          0x03
#define USB_ENDPOINT_TRANSFER_TYPE_SHIFT         0
#define USB_ENDPOINT_TRANSFER_TYPE_CONTROL       0x00
#define USB_ENDPOINT_TRANSFER_TYPE_ISOCHRONOUS   0x01
#define USB_ENDPOINT_TRANSFER_TYPE_BULK          0x02
#define USB_ENDPOINT_TRANSFER_TYPE_INTERRUPT     0x03

/* EndpointAddress */
#define USB_ENDPOINT_NUMBER_BITS                 0x1f
#define USB_ENDPOINT_NUMBER_SHIFT                0
#define USB_ENDPOINT_DIRECTION_BITS              0x80
#define USB_ENDPOINT_DIRECTION_SHIFT             7
#define USB_ENDPOINT_DIRECTION_OUT               0x00
#define USB_ENDPOINT_DIRECTION_IN                0x80

/* packet sizes */
#define USB_MAX_LS_CONTROL_PACKET_SIZE           8     /* low speed */
#define USB_MAX_LS_INTERRUPT_PACKET_SIZE         8     /* low speed */
#define USB_MAX_FS_CONTROL_PACKET_SIZE           64    /* full speed */
#define USB_MAX_FS_INTERRUPT_PACKET_SIZE         64    /* full speed */
#define USB_MAX_BULK_PACKET_SIZE                 64    /* full speed only */
#define USB_MAX_ISOCHRONOUS_PACKET_SIZE          1023  /* full speed only */
	
/* OHCI completion codes */
#define USB_HC_CC_NOERR                          0x0
#define USB_OHCI_CC_CRC                          0x1
#define USB_OHCI_CC_BFV                          0x2
#define USB_OHCI_CC_DTM                          0x3
#define USB_OHCI_CC_STALL                        0x4
#define USB_OHCI_CC_NOTRESP                      0x5
#define USB_OHCI_CC_PIDCF                        0x6
#define USB_OHCI_CC_UEPID                        0x7
#define USB_OHCI_CC_DOR                          0x8
#define USB_OHCI_CC_DUR                          0x9
#define USB_OHCI_CC_RSVDA                        0xa
#define USB_OHCI_CC_RSVDB                        0xb
#define USB_OHCI_CC_BOR                          0xc
#define USB_OHCI_CC_BUR                          0xd
#define USB_OHCI_CC_NOTACC1                      0xe
#define USB_OHCI_CC_NOTACC2                      0xf
#define USB_EHCI_CC_MISSMF                       0x10
#define USB_EHCI_CC_XACT                         0x20
#define USB_EHCI_CC_BABBLE                       0x30
#define USB_EHCI_CC_DATABUF                      0x40
#define USB_EHCI_CC_HALTED                       0x50

/* PSW isochronous transfer completion codes */
#define USB_CC_NOERR                             0x0
#define USB_CC_MISSMF                            0x1
#define USB_CC_XACT                              0x2
#define USB_CC_BABBLE                            0x4
#define USB_CC_DATABUF                           0x8

/* return codes */
#define USB_PROBE_FAILED                         (-1)
#define USB_ATTACH_FAILED                        (-1)
#define USB_DETACH_FAILED                        (-1)
#define USB_PROBE_SUCCEEDED                      (0)
#define USB_ATTACH_SUCCEEDED                     (0)
#define USB_DETACH_SUCCEEDED                     (0)

#define USB_MAX_ISOCH_PACKETS                    (8)

/* device speed */
#define USB_DEVICE_SPEED_LS                      (0)
#define USB_DEVICE_SPEED_FS                      (1)
#define USB_DEVICE_SPEED_HS                      (2)

/* thread type */
#define USB_THREAD_TYPE_EVENT_HANDLER            (0)
#define USB_THREAD_TYPE_USBD_THREAD              (1)
#define USB_THREAD_TYPE_CALLBACK_THREAD          (2)

/* errors */
#define USB_OK                                   0x00
#define USB_ERR_NOT_INITIALIZED                  (0x80110001)
#define USB_ERR_ALREADY_INITIALIZED              (0x80110002)
#define USB_ERR_NO_MEMORY                        (0x80110003)
#define USB_ERR_INVALID_PARAM                    (0x80110004)
#define USB_ERR_INVALID_TRANSFER_TYPE            (0x80110005)
#define USB_ERR_LDD_ALREADY_REGISTERED           (0x80110006)
#define USB_ERR_LDD_NOT_ALLOCATED                (0x80110007)
#define USB_ERR_LDD_NOT_RELEASED                 (0x80110008)
#define USB_ERR_LDD_NOT_FOUND                    (0x80110009)
#define USB_ERR_DEVICE_NOT_FOUND                 (0x8011000a)
#define USB_ERR_PIPE_NOT_ALLOCATED               (0x8011000b)
#define USB_ERR_PIPE_NOT_RELEASED                (0x8011000c)
#define USB_ERR_PIPE_NOT_FOUND                   (0x8011000d)
#define USB_ERR_IOREQ_NOT_ALLOCATED              (0x8011000e)
#define USB_ERR_IOREQ_NOT_RELEASED               (0x8011000f)
#define USB_ERR_IOREQ_NOT_FOUND                  (0x80110010)
#define USB_ERR_CANNOT_GET_DESCRIPTOR            (0x80110011)
#define USB_ERR_FATAL                            (0x801100ff)


/*
 * enums
 */

enum usbDeviceClass
{
  CLASS_PER_INTERFACE =                          USB_CLASS_PER_INTERFACE,
  CLASS_AUDIO =                                  USB_CLASS_AUDIO,
  CLASS_COMM =                                   USB_CLASS_COMM,
  CLASS_HID =                                    USB_CLASS_HID,
  CLASS_MONITOR =                                USB_CLASS_MONITOR,
  CLASS_PHYSICAL =                               USB_CLASS_PHYSICAL,
  CLASS_PTP =                                    USB_CLASS_PTP,
  CLASS_PRINTER =                                USB_CLASS_PRINTER,
  CLASS_STORAGE =                                USB_CLASS_STORAGE,
  CLASS_HUB =                                    USB_CLASS_HUB,
  CLASS_DATA =                                   USB_CLASS_DATA,
  CLASS_CHIP =                                   USB_CLASS_CHIP,
  CLASS_SECURITY =                               USB_CLASS_SECURITY,
  CLASS_VIDEO =                                  USB_CLASS_VIDEO,
  CLASS_DIAG =                                   USB_CLASS_DIAG,
  CLASS_WIRELESS =                               USB_CLASS_WIRELESS,
  CLASS_MISC =                                   USB_CLASS_MISC,
  CLASS_APPLICATION =                            USB_CLASS_APPLICATION,
  CLASS_VENDOR_SPEC =                            USB_CLASS_VENDOR_SPEC
};

enum usbDescriptorType
{
  DT_DEVICE =                                    USB_DESCRIPTOR_TYPE_DEVICE,
  DT_CONFIG =                                    USB_DESCRIPTOR_TYPE_CONFIG,
  DT_STRING =                                    USB_DESCRIPTOR_TYPE_STRING,
  DT_INTERFACE =                                 USB_DESCRIPTOR_TYPE_INTERFACE,
  DT_ENDPOINT =                                  USB_DESCRIPTOR_TYPE_ENDPOINT,
  DT_HID =                                       USB_DESCRIPTOR_TYPE_HID,
  DT_REPORT =                                    USB_DESCRIPTOR_TYPE_REPORT,
  DT_PHYSICAL =                                  USB_DESCRIPTOR_TYPE_PHYSICAL,
  DT_HUB =                                       USB_DESCRIPTOR_TYPE_HUB
};

enum usbEndpointDirection
{
  ENDPOINT_IN =                                  USB_ENDPOINT_DIRECTION_IN,
  ENDPOINT_OUT =                                 USB_ENDPOINT_DIRECTION_OUT
};

enum usbTransferType
{
  TRANSFER_TYPE_CONTROL =                        USB_ENDPOINT_TRANSFER_TYPE_CONTROL, 
  TRANSFER_TYPE_ISOCHRONOUS =                    USB_ENDPOINT_TRANSFER_TYPE_ISOCHRONOUS,
  TRANSFER_TYPE_BULK =                           USB_ENDPOINT_TRANSFER_TYPE_BULK,
  TRANSFER_TYPE_INTERRUPT =                      USB_ENDPOINT_TRANSFER_TYPE_INTERRUPT,
};

enum usbStandardRequest
{
  REQUEST_GET_STATUS =                           USB_REQUEST_GET_STATUS,
  REQUEST_CLEAR_FEATURE =                        USB_REQUEST_CLEAR_FEATURE,
  REQUEST_SET_FEATURE =                          USB_REQUEST_SET_FEATURE,
  REQUEST_SET_ADDRESS =                          USB_REQUEST_SET_ADDRESS,
  REQUEST_GET_DESCRIPTOR =                       USB_REQUEST_GET_DESCRIPTOR,
  REQUEST_SET_DESCRIPTOR =                       USB_REQUEST_SET_DESCRIPTOR,
  REQUEST_GET_CONFIGURATION =                    USB_REQUEST_GET_CONFIGURATION,
  REQUEST_SET_CONFIGURATION =                    USB_REQUEST_SET_CONFIGURATION,
  REQUEST_GET_INTERFACE =                        USB_REQUEST_GET_INTERFACE,
  REQUEST_SET_INTERFACE =                        USB_REQUEST_SET_INTERFACE,
  REQUEST_SYNCH_FRAME =                          USB_REQUEST_SYNCH_FRAME
};

enum usbRequestType
{
  REQUEST_TYPE_STANDARD =                        USB_REQTYPE_TYPE_STANDARD,
  REQUEST_TYPE_CLASS =                           USB_REQTYPE_TYPE_CLASS,
  REQUEST_TYPE_VENDOR =                          USB_REQTYPE_TYPE_VENDOR,
  REQUEST_TYPE_BITS =                            USB_REQTYPE_TYPE_BITS
};

enum usbRequestRecipient
{
  RECIPIENT_DEVICE =                             USB_REQTYPE_RECIP_DEVICE,
  RECIPIENT_INTERFACE =                          USB_REQTYPE_RECIP_INTERFACE,
  RECIPIENT_ENDPOINT =                           USB_REQTYPE_RECIP_ENDPOINT,
  RECIPIENT_OTHER =                              USB_REQTYPE_RECIP_OTHER,
  RECIPIENT_BITS =                               USB_REQTYPE_RECIP_BITS
};

enum usbBCD
{
  BCD_10 =                                       0x0100,
  BCD_11 =                                       0x0110,
  BCD_20 =                                       0x0200,
  BCD_30 =                                       0x0300
};


/*
 * structs
 */

/* control setup */
typedef struct _usb_control_setup
{
  u8 bmRequestType;
  u8 bRequest;
  u16 wValue;
  u16 wIndex;
  u16 wLength;
} usbControlSetup;

/* 0x01 device descriptor */
typedef struct _usb_device_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant USB_DESCRIPTOR_TYPE_DEVICE */
  u16 bcdUSB;              /* USB spec release compliance number */
  u8 bDeviceClass;         /* class code */
  u8 bDeviceSubClass;      /* subclass code */
  u8 bDeviceProtocol;      /* protocol code */
  u8 bMaxPacketSize0;      /* max packet size for endpoint 0 */
  u16 idVendor;            /* USB-IF Vendor ID (VID) */
  u16 idProduct;           /* Product ID (PID) */
  u16 bcdDevice;           /* device release number */
  u8 iManufacturer;        /* manufacturer string descriptor index */
  u8 iProduct;             /* product string desccriptor index */
  u8 iSerialNumber;        /* serial number string descriptor index */
  u8 bNumConfigurations;   /* number of configurations */
} usbDeviceDescriptor;

/* 0x02 configuration descriptor */
typedef struct _usb_config_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant USB_DESCRIPTOR_TYPE_CONFIG */
  u16 wTotalLength;        /* number of bytes in descriptor and all children */
  u8 bNumInterfaces;       /* number of interfaces in config */
  u8 bConfigurationValue;  /* identifier for GET_CONFIGURATION / SET_CONFIGURATION */
  u8 iConfiguration;       /* index of string descriptor for the config */
  u8 bmAttributes;         /* self/bus power and remote wakeup settings */
  u8 MaxPower;             /* bus powered required as (max milliamps/2) */
} usbConfigDescriptor;

/* 0x03 string descriptor */
typedef struct _usb_string_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant USB_DESCRIPTOR_TYPE_STRING */
  u8 bString[0];           /* unicode string */
} usbStringDescriptor;

/* 0x04 interface descriptor */
typedef struct _usb_interface_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant USB_DESCRIPTOR_TYPE_INTERFACE */
  u8 bInterfaceNumber;     /* number identifying this interface */
  u8 bAlternateSetting;    /* value used to select alternate setting */
  u8 bNumEndpoints;        /* number of endpoints supported (excluding 0) */
  u8 bInterfaceClass;      /* class code */
  u8 bInterfaceSubClass;   /* subclass code */
  u8 bInterfaceProtocol;   /* protocol code */
  u8 iInterface;           /* index of string descriptor for the interface */
} usbInterfaceDescriptor;

/* 0x05 endpoint descriptor */
typedef struct _usb_endpoint_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant USB_DESCRIPTOR_TYPE_ENDPOINT */
  u8 bEndpointAddress;     /* endpoint number and direction */
  u8 bmAttributes;         /* transfer type supported */
  u16 wMaxPacketSize;      /* maximum packet size supported */
  u8 bInterval;            /* maximum latency/polling interval/NAK rate */
} usbEndpointDescriptor;

/* 0x06 device qualifier descriptor */
typedef struct _usb_device_qualifier_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER */
  u16 bcdUSB;              /* USB spec release compliance number */
  u8 bDeviceClass;         /* class code */
  u8 bDeviceSubClass;      /* subclass code */
  u8 bDeviceProtocol;      /* protocol code */
  u8 bMaxPacketSize0;      /* max packet size for endpoint 0 */
  u8 bNumConfigurations;   /* number of configurations */
  u8 bReserved;            /* reserved for future use */
} usbDeviceQualifierDescriptor;

/* 0x07 other_speed_configuration descriptor */
typedef struct _usb_other_speed_configuration_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG */
  u16 wTotalLength;        /* number of bytes in descriptor and all children */
  u8 bNumInterfaces;       /* number of interfaces in config */
  u8 bConfigurationValue;  /* identifier for GET_CONFIGURATION / SET_CONFIGURATION */
  u8 iConfiguration;       /* index of string descriptor for the config */
  u8 bmAttributes;         /* self/bus power and remote wakeup settings */
  u8 MaxPower;             /* bus powered required as (max milliamps/2) */
} usbOtherSpeedConfigurationDescriptor;

/* 0x0b interface association descriptor */
typedef struct _usb_interface_association_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant USB_DESCRIPTOR_TYPE_INTERFACE_ASSOC */
  u8 bFirstInterface;      /* first associated interface with function */
  u8 bInterfaceCount;      /* number of contiguous interfaces associated with function */
  u8 bFunctionClass;       /* class code */
  u8 bFunctionSubClass;    /* subclass code */
  u8 bFunctionProtocol;    /* protocol code */
  u8 iFunction;            /* index of string descriptor for the function */
} usbInterfaceAssociationDescriptor;

/* HID descriptor info (class specific descriptor) */
typedef struct _usb_hid_sub_descriptor_info
{   
  u8 bDescriptorType; 
  u8 wDescriptorLength0;
  u8 wDescriptorLength1;
} usbHidSubDescriptorInfo;

/* 0x21 HID descriptor */
typedef struct _usb_hid_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant USB_DESCRIPTOR_TYPE_HID */
  u8 bcdHID0;
  u8 bcdHID1;
  u8 bCountryCode;
  u8 bNumDescriptors;      /* number of subdescriptor */
  usbHidSubDescriptorInfo subDescriptorInfo[0];
} usbHidDescriptor;

/*
typedef struct _usb_interface
{
  const struct usbInterfaceDescriptor *altsetting ATTRIBUTE_PRXPTR;
  s32 num_altsetting;
} usbInterface;
*/

/* */
typedef struct _usb_isoch_psw_len
{
  u16 len:11;
  u16 reserved:1;
  u16 PSW:4;
} usbIsochPswLen;

/* */
typedef struct _usb_hs_isoch_psw_len
{
  u16 len:12;
  u16 PSW:4;
} usbHSIsochPswLen;

/* */
typedef struct _usb_hs_isoch_request
{
  void *buffer_base ATTRIBUTE_PRXPTR;
  s32 relative_start_frame;
  s32 num_packets;
  usbHSIsochPswLen packets[USB_MAX_ISOCH_PACKETS];
} usbHSIsochRequest;

/* */
typedef struct _usb_isoch_request
{
  void *buffer_base ATTRIBUTE_PRXPTR;
  s32 relative_start_frame;
  s32 num_packets;
  usbIsochPswLen packets[USB_MAX_ISOCH_PACKETS];
} usbIsochRequest;


/*
 * struct aliases
 */

typedef usbControlSetup usbDeviceRequest;
typedef usbConfigDescriptor usbConfigurationDescriptor;


/*
 * callbacks
 */

/* Logical Device Driver (ldd) operations */
typedef struct _usb_ldd_ops
{
  const char *name;
  s32 (*probe)(s32 dev_id);
  s32 (*attach)(s32 dev_id);
  s32 (*detach)(s32 dev_id);
} usbLddOps;

/* transfer done */
typedef void (*usbDoneCallback)(s32 result, s32 count, void* arg);

/* isochronous transfer done */
typedef void (*usbIsochDoneCallback)(s32 result, usbIsochRequest *req, void *arg);

/* HS isochronous transfer done */
typedef void (*usbHSIsochDoneCallback)(s32 result, usbHSIsochRequest *req, void *arg);


/*
 * functions
 */ 

s32 usbInit(void);
s32 usbEnd(void);

s32 usbOpenPipe(s32 device_id, usbEndpointDescriptor *end_desc);
s32 usbClosePipe(s32 pipe_id);

s32 usbAllocateMemory(void **ptr, size_t size);
s32 usbFreeMemory(void *ptr);

void *usbScanStaticDescriptor(s32 device_id, void *ptr, unsigned char type);

s32 usbGetDeviceLocation(s32 device_id, unsigned char *location);
s32 usbGetDeviceSpeed(s32 device_id, u8 *speed);

void *usbGetPrivateData(s32 device_id);
s32 usbSetPrivateData(s32 device_id, void *priv);

s32 usbGetThreadPriority(s32 thread_type);
/* s32 usbSetThreadPriority(UNKNOWN); */
s32 usbSetThreadPriority2(s32 event_prio, s32 usb_prio, s32 callback_priority);

s32 usbRegisterLdd(usbLddOps *lddops);
s32 usbUnregisterLdd(usbLddOps *lddops);
s32 usbRegisterExtraLdd(usbLddOps *lddops, u16 vendor_id, u16 product_id);
s32 usbRegisterExtraLdd2(usbLddOps *lddops, u16 vendor_id, u16 product_id_min, u16 product_id_max);
s32 usbUnregisterExtraLdd(usbLddOps *lddops);

s32 usbBulkTransfer(s32 pipe_id, void *buf, s32 len, usbDoneCallback cb, void* arg);
s32 usbControlTransfer(s32 pipe_id, usbDeviceRequest *req, void *buf, usbDoneCallback cb, void *arg);
s32 usbInterruptTransfer(s32 pipe_id, void *buf, s32 len, usbDoneCallback cb, void *arg);
s32 usbIsochronousTransfer(s32 pipe_id, usbIsochRequest *req, usbIsochDoneCallback cb, void *arg);
s32 usbHSIsochronousTransfer(s32 pipe_id, usbHSIsochRequest *req, usbHSIsochDoneCallback cb, void *arg);


/*
 * standard control xfer macros
 */

#define usbClearDeviceFeature(pipe_id, fs, done_cb, arg) ({  \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x00; \
  device_request.bRequest = USB_REQUEST_CLEAR_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = 0; \
  device_request.wLength = 0; \
  usbControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define usbClearInterfaceFeature(pipe_id, fs, iInterface, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x01; \
  device_request.bRequest = USB_REQUEST_CLEAR_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = (iInterface); \
  device_request.wLength = 0; \
  usbControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define usbClearEndpointFeature(pipe_id, fs, endpoint, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x02; \
  device_request.bRequest = USB_REQUEST_CLEAR_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = (endpoint); \
  device_request.wLength = 0; \
  usbControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define usbGetConfiguration(pipe_id, ptr, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x80; \
  device_request.bRequest = USB_REQUEST_GET_CONFIGURATION; \
  device_request.wValue = 0; \
  device_request.wIndex = 0; \
  device_request.wLength = 1; \
  usbControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define usbGetDescriptor(pipe_id, type, wIndex, lang_id, ptr, len, done_cb, arg) \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x80; \
  device_request.bRequest = USB_REQUEST_GET_DESCRIPTOR; \
  device_request.wValue = ((type) << 8) | (wIndex); \
  device_request.wIndex = (lang_id); \
  device_request.wLength = (len); \
  usbControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg));

#define usbGetInterface(pipe_id, iInterface, ptr, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x81; \
  device_request.bRequest = USB_REQUEST_GET_INTERFACE; \
  device_request.wValue = 0; \
  device_request.wIndex = (iInterface); \
  device_request.wLength = 1; \
  usbControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define usbGetDeviceStatus(pipe_id, ptr, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x80; \
  device_request.bRequest = USB_REQUEST_GET_STATUS; \
  device_request.wValue = 0; \
  device_request.wIndex = 0; \
  device_request.wLength = 2; \
  usbControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define usbGetInterfaceStatus(pipe_id, iInterface, ptr, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x81; \
  device_request.bRequest = USB_REQUEST_GET_STATUS; \
  device_request.wValue = 0; \
  device_request.wIndex = (iInterface); \
  device_request.wLength = 2; \
  usbControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define usbGetEndpointStatus(pipe_id, endpoint, ptr, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x82; \
  device_request.bRequest = USB_REQUEST_GET_STATUS; \
  device_request.wValue = 0; \
  device_request.wIndex = (endpoint); \
  device_request.wLength = 2; \
  usbControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define usbSetAddress(pipe_id, address, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x00; \
  device_request.bRequest = USB_REQUEST_SET_ADDRESS; \
  device_request.wValue = (address); \
  device_request.wIndex = 0; \
  device_request.wLength = 0; \
  usbControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define usbSetConfiguration(pipe_id, config, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x00; \
  device_request.bRequest = USB_REQUEST_SET_CONFIGURATION; \
  device_request.wValue = (config); \
  device_request.wIndex = 0; \
  device_request.wLength = 0; \
  usbControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define usbSetDeviceDescriptor(pipe_id, type, wIndex, lang_id, ptr, len, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x00; \
  device_request.bRequest = USB_REQUEST_SET_DESCRIPTOR; \
  device_request.wValue = ((type) << 8) | (wIndex); \
  device_request.wIndex = (lang_id); \
  device_request.wLength = (len); \
  usbControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define usbSetInterfaceDescriptor(pipe_id, type, wIndex, lang_id, ptr, len, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x01; \
  device_request.bRequest = USB_REQUEST_SET_DESCRIPTOR; \
  device_request.wValue = ((type) << 8) | (wIndex); \
  device_request.wIndex = (lang_id); \
  device_request.wLength = (len); \
  usbControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define usbSetEndpointDescriptor(pipe_id, type, wIndex, lang_id, ptr, len, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x02; \
  device_request.bRequest = USB_REQUEST_SET_DESCRIPTOR; \
  device_request.wValue = ((type) << 8) | (wIndex); \
  device_request.wIndex = (lang_id); \
  device_request.wLength = (len); \
  usbControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define usbSetDeviceFeature(pipe_id, fs, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x00; \
  device_request.bRequest = USB_REQUEST_SET_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = 0; \
  device_request.wLength = 0; \
  usbControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define usbSetInterfaceFeature(pipe_id, fs, iInterface, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x01; \
  device_request.bRequest = USB_REQUEST_SET_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = (iInterface); \
  device_request.wLength = 0; \
  usbControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define usbSetEndpointFeature(pipe_id, fs, endpoint, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x02; \
  device_request.bRequest = USB_REQUEST_SET_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = (endpoint); \
  device_request.wLength = 0; \
  usbControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define usbSetInterface(pipe_id, iInterface, alt_setting, done_cb, arg) ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x01; \
  device_request.bRequest = USB_REQUEST_SET_INTERFACE; \
  device_request.wValue = (alt_setting); \
  device_request.wIndex = (iInterface); \
  device_request.wLength = 0; \
  usbControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define usbSynchFrame(pipe_id, endpoint, buf, done_cb, arg)  ({ \
  usbDeviceRequest device_request; \
  device_request.bmRequestType = 0x82; \
  device_request.bRequest = USB_REQUEST_SYNCH_FRAME; \
  device_request.wValue = 0; \
  device_request.wIndex = (endpoint); \
  device_request.wLength = 2; \
  usbControlTransfer((pipe_id), (&device_request), buf, (done_cb), (arg)); })


#endif /* __USB_H__ */

