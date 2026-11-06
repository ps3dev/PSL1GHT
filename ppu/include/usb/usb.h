/*! \file usb.h
 \brief USB low level management.
 */

#ifndef __CELL_USB_H__
#define __CELL_USB_H__

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
#define CELL_USB_REQTYPE_DIR_BITS                     0x80
#define CELL_USB_REQTYPE_DIR_TO_DEVICE                0x00
#define CELL_USB_REQTYPE_DIR_TO_HOST                  0x80

#define CELL_USB_REQTYPE_TYPE_STANDARD                0x00
#define CELL_USB_REQTYPE_TYPE_CLASS                   0x20
#define CELL_USB_REQTYPE_TYPE_VENDOR                  0x40
#define CELL_USB_REQTYPE_TYPE_RESERVED                0x60
#define CELL_USB_REQTYPE_TYPE_BITS                    0x60

#define CELL_USB_REQTYPE_RECIP_DEVICE                 0x00
#define CELL_USB_REQTYPE_RECIP_INTERFACE              0x01
#define CELL_USB_REQTYPE_RECIP_ENDPOINT               0x02
#define CELL_USB_REQTYPE_RECIP_OTHER                  0x03
#define CELL_USB_REQTYPE_RECIP_BITS                   0x1f

/* bRequest */
#define CELL_USB_REQUEST_GET_STATUS                   0x00
#define CELL_USB_REQUEST_CLEAR_FEATURE                0x01
#define CELL_USB_REQUEST_SET_FEATURE                  0x03
#define CELL_USB_REQUEST_SET_ADDRESS                  0x05
#define CELL_USB_REQUEST_GET_DESCRIPTOR               0x06
#define CELL_USB_REQUEST_SET_DESCRIPTOR               0x07
#define CELL_USB_REQUEST_GET_CONFIGURATION            0x08
#define CELL_USB_REQUEST_SET_CONFIGURATION            0x09
#define CELL_USB_REQUEST_GET_INTERFACE                0x0a
#define CELL_USB_REQUEST_SET_INTERFACE                0x0b
#define CELL_USB_REQUEST_SYNCH_FRAME                  0x0c

/* bDescriptorType */
#define CELL_USB_DESCRIPTOR_TYPE_DEVICE               0x01
#define CELL_USB_DESCRIPTOR_TYPE_CONFIG               0x02
#define CELL_USB_DESCRIPTOR_TYPE_STRING               0x03
#define CELL_USB_DESCRIPTOR_TYPE_INTERFACE            0x04
#define CELL_USB_DESCRIPTOR_TYPE_ENDPOINT             0x05
#define CELL_USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER     0x06
#define CELL_USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG   0x07
#define CELL_USB_DESCRIPTOR_TYPE_INTERFACE_POWER      0x08
#define CELL_USB_DESCRIPTOR_TYPE_OTG                  0x09
#define CELL_USB_DESCRIPTOR_TYPE_DEBUG                0x0a
#define CELL_USB_DESCTIPTOR_TYPE_INTERFACE_ASSOC      0x0b
#define CELL_USB_DESCRIPTOR_TYPE_HID                  0x21
#define CELL_USB_DESCRIPTOR_TYPE_REPORT               0x22
#define CELL_USB_DESCRIPTOR_TYPE_PHYSICAL             0x23
#define CELL_USB_DESCRIPTOR_TYPE_HUB                  0x29

/* features */
#define CELL_USB_FEATURE_ENDPOINT_HALT                0x00
#define CELL_USB_FEATURE_DEVICE_REMOTE_WAKEUP         0x01

/* device classes */
#define CELL_USB_CLASS_PER_INTERFACE                  0x00
#define CELL_USB_CLASS_AUDIO                          0x01
#define CELL_USB_CLASS_COMM                           0x02
#define CELL_USB_CLASS_HID                            0x03
#define CELL_USB_CLASS_MONITOR                        0x04
#define CELL_USB_CLASS_PHYSICAL                       0x05
#define CELL_USB_CLASS_PTP                            0x06
#define CELL_USB_CLASS_PRINTER                        0x07
#define CELL_USB_CLASS_STORAGE                        0x08
#define CELL_USB_CLASS_HUB                            0x09
#define CELL_USB_CLASS_DATA                           0x0a
#define CELL_USB_CLASS_CHIP                           0x0b
#define CELL_USB_CLASS_SECURITY                       0x0d
#define CELL_USB_CLASS_VIDEO                          0x0e
#define CELL_USB_CLASS_XBOX                           0x58
#define CELL_USB_CLASS_DIAG                           0xdc
#define CELL_USB_CLASS_WIRELESS                       0xe0
#define CELL_USB_CLASS_MISC                           0xef
#define CELL_USB_CLASS_APPLICATION                    0xfe
#define CELL_USB_CLASS_VENDOR_SPEC                    0xff

/* iConfiguration */
#define CELL_USB_CONFIGURATION_RESERVED_ZERO          0x1f
#define CELL_USB_CONFIGURATION_BUS_POWERED            0x00 /* (0<<6) */
#define CELL_USB_CONFIGURATION_REMOTE_WAKEUP          0x20 /* (1<<5) */
#define CELL_USB_CONFIGURATION_SELF_POWERED           0x40 /* (1<<6) */
#define CELL_USB_CONFIGURATION_RESERVED_ONE           0x80

/* bmAttribute */
#define CELL_USB_ENDPOINT_TRANSFER_TYPE_BITS          0x03
#define CELL_USB_ENDPOINT_TRANSFER_TYPE_SHIFT         0
#define CELL_USB_ENDPOINT_TRANSFER_TYPE_CONTROL       0x00
#define CELL_USB_ENDPOINT_TRANSFER_TYPE_ISOCHRONOUS   0x01
#define CELL_USB_ENDPOINT_TRANSFER_TYPE_BULK          0x02
#define CELL_USB_ENDPOINT_TRANSFER_TYPE_INTERRUPT     0x03

/* EndpointAddress */
#define CELL_USB_ENDPOINT_NUMBER_BITS                 0x1f
#define CELL_USB_ENDPOINT_NUMBER_SHIFT                0
#define CELL_USB_ENDPOINT_DIRECTION_BITS              0x80
#define CELL_USB_ENDPOINT_DIRECTION_SHIFT             7
#define CELL_USB_ENDPOINT_DIRECTION_OUT               0x00
#define CELL_USB_ENDPOINT_DIRECTION_IN                0x80

/* packet sizes */
#define CELL_USB_MAX_LS_CONTROL_PACKET_SIZE           8     /* low speed */
#define CELL_USB_MAX_LS_INTERRUPT_PACKET_SIZE         8     /* low speed */
#define CELL_USB_MAX_FS_CONTROL_PACKET_SIZE           64    /* full speed */
#define CELL_USB_MAX_FS_INTERRUPT_PACKET_SIZE         64    /* full speed */
#define CELL_USB_MAX_BULK_PACKET_SIZE                 64    /* full speed only */
#define CELL_USB_MAX_ISOCHRONOUS_PACKET_SIZE          1023  /* full speed only */
	
/* OHCI completion codes */
#define CELL_USB_HC_CC_NOERR                          0x0
#define CELL_USB_OHCI_CC_CRC                          0x1
#define CELL_USB_OHCI_CC_BFV                          0x2
#define CELL_USB_OHCI_CC_DTM                          0x3
#define CELL_USB_OHCI_CC_STALL                        0x4
#define CELL_USB_OHCI_CC_NOTRESP                      0x5
#define CELL_USB_OHCI_CC_PIDCF                        0x6
#define CELL_USB_OHCI_CC_UEPID                        0x7
#define CELL_USB_OHCI_CC_DOR                          0x8
#define CELL_USB_OHCI_CC_DUR                          0x9
#define CELL_USB_OHCI_CC_RSVDA                        0xa
#define CELL_USB_OHCI_CC_RSVDB                        0xb
#define CELL_USB_OHCI_CC_BOR                          0xc
#define CELL_USB_OHCI_CC_BUR                          0xd
#define CELL_USB_OHCI_CC_NOTACC1                      0xe
#define CELL_USB_OHCI_CC_NOTACC2                      0xf
#define CELL_USB_EHCI_CC_MISSMF                       0x10
#define CELL_USB_EHCI_CC_XACT                         0x20
#define CELL_USB_EHCI_CC_BABBLE                       0x30
#define CELL_USB_EHCI_CC_DATABUF                      0x40
#define CELL_USB_EHCI_CC_HALTED                       0x50

/* PSW isochronous transfer completion codes */
#define CELL_USB_CC_NOERR                             0x0
#define CELL_USB_CC_MISSMF                            0x1
#define CELL_USB_CC_XACT                              0x2
#define CELL_USB_CC_BABBLE                            0x4
#define CELL_USB_CC_DATABUF                           0x8

/* return codes */
#define CELL_USB_PROBE_FAILED                         (-1)
#define CELL_USB_ATTACH_FAILED                        (-1)
#define CELL_USB_DETACH_FAILED                        (-1)
#define CELL_USB_PROBE_SUCCEEDED                      (0)
#define CELL_USB_ATTACH_SUCCEEDED                     (0)
#define CELL_USB_DETACH_SUCCEEDED                     (0)

#define CELL_USB_MAX_ISOCH_PACKETS                    (8)

/* device speed */
#define CELL_USB_DEVICE_SPEED_LS                      (0)
#define CELL_USB_DEVICE_SPEED_FS                      (1)
#define CELL_USB_DEVICE_SPEED_HS                      (2)

/* thread type */
#define CELL_USB_THREAD_TYPE_EVENT_HANDLER            (0)
#define CELL_USB_THREAD_TYPE_USBD_THREAD              (1)
#define CELL_USB_THREAD_TYPE_CALLBACK_THREAD          (2)

/* errors */
#define CELL_USB_OK                                   0x00
#define CELL_USB_ERR_NOT_INITIALIZED                  (0x80110001)
#define CELL_USB_ERR_ALREADY_INITIALIZED              (0x80110002)
#define CELL_USB_ERR_NO_MEMORY                        (0x80110003)
#define CELL_USB_ERR_INVALID_PARAM                    (0x80110004)
#define CELL_USB_ERR_INVALID_TRANSFER_TYPE            (0x80110005)
#define CELL_USB_ERR_LDD_ALREADY_REGISTERED           (0x80110006)
#define CELL_USB_ERR_LDD_NOT_ALLOCATED                (0x80110007)
#define CELL_USB_ERR_LDD_NOT_RELEASED                 (0x80110008)
#define CELL_USB_ERR_LDD_NOT_FOUND                    (0x80110009)
#define CELL_USB_ERR_DEVICE_NOT_FOUND                 (0x8011000a)
#define CELL_USB_ERR_PIPE_NOT_ALLOCATED               (0x8011000b)
#define CELL_USB_ERR_PIPE_NOT_RELEASED                (0x8011000c)
#define CELL_USB_ERR_PIPE_NOT_FOUND                   (0x8011000d)
#define CELL_USB_ERR_IOREQ_NOT_ALLOCATED              (0x8011000e)
#define CELL_USB_ERR_IOREQ_NOT_RELEASED               (0x8011000f)
#define CELL_USB_ERR_IOREQ_NOT_FOUND                  (0x80110010)
#define CELL_USB_ERR_CANNOT_GET_DESCRIPTOR            (0x80110011)
#define CELL_USB_ERR_FATAL                            (0x801100ff)


/*
 * enums
 */

enum cellUsbdDeviceClass
{
  CLASS_PER_INTERFACE =                          CELL_USB_CLASS_PER_INTERFACE,
  CLASS_AUDIO =                                  CELL_USB_CLASS_AUDIO,
  CLASS_COMM =                                   CELL_USB_CLASS_COMM,
  CLASS_HID =                                    CELL_USB_CLASS_HID,
  CLASS_MONITOR =                                CELL_USB_CLASS_MONITOR,
  CLASS_PHYSICAL =                               CELL_USB_CLASS_PHYSICAL,
  CLASS_PTP =                                    CELL_USB_CLASS_PTP,
  CLASS_PRINTER =                                CELL_USB_CLASS_PRINTER,
  CLASS_STORAGE =                                CELL_USB_CLASS_STORAGE,
  CLASS_HUB =                                    CELL_USB_CLASS_HUB,
  CLASS_DATA =                                   CELL_USB_CLASS_DATA,
  CLASS_CHIP =                                   CELL_USB_CLASS_CHIP,
  CLASS_SECURITY =                               CELL_USB_CLASS_SECURITY,
  CLASS_VIDEO =                                  CELL_USB_CLASS_VIDEO,
  CLASS_DIAG =                                   CELL_USB_CLASS_DIAG,
  CLASS_WIRELESS =                               CELL_USB_CLASS_WIRELESS,
  CLASS_MISC =                                   CELL_USB_CLASS_MISC,
  CLASS_APPLICATION =                            CELL_USB_CLASS_APPLICATION,
  CLASS_VENDOR_SPEC =                            CELL_USB_CLASS_VENDOR_SPEC
};

enum cellUsbdDescriptorType
{
  DT_DEVICE =                                    CELL_USB_DESCRIPTOR_TYPE_DEVICE,
  DT_CONFIG =                                    CELL_USB_DESCRIPTOR_TYPE_CONFIG,
  DT_STRING =                                    CELL_USB_DESCRIPTOR_TYPE_STRING,
  DT_INTERFACE =                                 CELL_USB_DESCRIPTOR_TYPE_INTERFACE,
  DT_ENDPOINT =                                  CELL_USB_DESCRIPTOR_TYPE_ENDPOINT,
  DT_HID =                                       CELL_USB_DESCRIPTOR_TYPE_HID,
  DT_REPORT =                                    CELL_USB_DESCRIPTOR_TYPE_REPORT,
  DT_PHYSICAL =                                  CELL_USB_DESCRIPTOR_TYPE_PHYSICAL,
  DT_HUB =                                       CELL_USB_DESCRIPTOR_TYPE_HUB
};

enum cellUsbdEndpointDirection
{
  ENDPOINT_IN =                                  CELL_USB_ENDPOINT_DIRECTION_IN,
  ENDPOINT_OUT =                                 CELL_USB_ENDPOINT_DIRECTION_OUT
};

enum cellUsbdTransferType
{
  TRANSFER_TYPE_CONTROL =                        CELL_USB_ENDPOINT_TRANSFER_TYPE_CONTROL, 
  TRANSFER_TYPE_ISOCHRONOUS =                    CELL_USB_ENDPOINT_TRANSFER_TYPE_ISOCHRONOUS,
  TRANSFER_TYPE_BULK =                           CELL_USB_ENDPOINT_TRANSFER_TYPE_BULK,
  TRANSFER_TYPE_INTERRUPT =                      CELL_USB_ENDPOINT_TRANSFER_TYPE_INTERRUPT,
};

enum cellUsbdStandardRequest
{
  REQUEST_GET_STATUS =                           CELL_USB_REQUEST_GET_STATUS,
  REQUEST_CLEAR_FEATURE =                        CELL_USB_REQUEST_CLEAR_FEATURE,
  REQUEST_SET_FEATURE =                          CELL_USB_REQUEST_SET_FEATURE,
  REQUEST_SET_ADDRESS =                          CELL_USB_REQUEST_SET_ADDRESS,
  REQUEST_GET_DESCRIPTOR =                       CELL_USB_REQUEST_GET_DESCRIPTOR,
  REQUEST_SET_DESCRIPTOR =                       CELL_USB_REQUEST_SET_DESCRIPTOR,
  REQUEST_GET_CONFIGURATION =                    CELL_USB_REQUEST_GET_CONFIGURATION,
  REQUEST_SET_CONFIGURATION =                    CELL_USB_REQUEST_SET_CONFIGURATION,
  REQUEST_GET_INTERFACE =                        CELL_USB_REQUEST_GET_INTERFACE,
  REQUEST_SET_INTERFACE =                        CELL_USB_REQUEST_SET_INTERFACE,
  REQUEST_SYNCH_FRAME =                          CELL_USB_REQUEST_SYNCH_FRAME
};

enum cellUsbdRequestType
{
  REQUEST_TYPE_STANDARD =                        CELL_USB_REQTYPE_TYPE_STANDARD,
  REQUEST_TYPE_CLASS =                           CELL_USB_REQTYPE_TYPE_CLASS,
  REQUEST_TYPE_VENDOR =                          CELL_USB_REQTYPE_TYPE_VENDOR,
  REQUEST_TYPE_BITS =                            CELL_USB_REQTYPE_TYPE_BITS
};

enum cellUsbdRequestRecipient
{
  RECIPIENT_DEVICE =                             CELL_USB_REQTYPE_RECIP_DEVICE,
  RECIPIENT_INTERFACE =                          CELL_USB_REQTYPE_RECIP_INTERFACE,
  RECIPIENT_ENDPOINT =                           CELL_USB_REQTYPE_RECIP_ENDPOINT,
  RECIPIENT_OTHER =                              CELL_USB_REQTYPE_RECIP_OTHER,
  RECIPIENT_BITS =                               CELL_USB_REQTYPE_RECIP_BITS
};

enum cellUsbdBCD
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
typedef struct _cellUsbd_control_setup
{
  u8 bmRequestType;
  u8 bRequest;
  u16 wValue;
  u16 wIndex;
  u16 wLength;
} CellUsbdControlSetup;

/* 0x01 device descriptor */
typedef struct _cellUsbd_device_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant CELL_USB_DESCRIPTOR_TYPE_DEVICE */
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
} CellUsbdDeviceDescriptor;

/* 0x02 configuration descriptor */
typedef struct _cellUsbd_config_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant CELL_USB_DESCRIPTOR_TYPE_CONFIG */
  u16 wTotalLength;        /* number of bytes in descriptor and all children */
  u8 bNumInterfaces;       /* number of interfaces in config */
  u8 bConfigurationValue;  /* identifier for GET_CONFIGURATION / SET_CONFIGURATION */
  u8 iConfiguration;       /* index of string descriptor for the config */
  u8 bmAttributes;         /* self/bus power and remote wakeup settings */
  u8 MaxPower;             /* bus powered required as (max milliamps/2) */
} CellUsbdConfigDescriptor;

/* 0x03 string descriptor */
typedef struct _cellUsbd_string_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant CELL_USB_DESCRIPTOR_TYPE_STRING */
  u8 bString[0];           /* unicode string */
} CellUsbdStringDescriptor;

/* 0x04 interface descriptor */
typedef struct _cellUsbd_interface_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant CELL_USB_DESCRIPTOR_TYPE_INTERFACE */
  u8 bInterfaceNumber;     /* number identifying this interface */
  u8 bAlternateSetting;    /* value used to select alternate setting */
  u8 bNumEndpoints;        /* number of endpoints supported (excluding 0) */
  u8 bInterfaceClass;      /* class code */
  u8 bInterfaceSubClass;   /* subclass code */
  u8 bInterfaceProtocol;   /* protocol code */
  u8 iInterface;           /* index of string descriptor for the interface */
} CellUsbdInterfaceDescriptor;

/* 0x05 endpoint descriptor */
typedef struct _cellUsbd_endpoint_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant CELL_USB_DESCRIPTOR_TYPE_ENDPOINT */
  u8 bEndpointAddress;     /* endpoint number and direction */
  u8 bmAttributes;         /* transfer type supported */
  u16 wMaxPacketSize;      /* maximum packet size supported */
  u8 bInterval;            /* maximum latency/polling interval/NAK rate */
} CellUsbdEndpointDescriptor;

/* 0x06 device qualifier descriptor */
typedef struct _cellUsbd_device_qualifier_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant CELL_USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER */
  u16 bcdUSB;              /* USB spec release compliance number */
  u8 bDeviceClass;         /* class code */
  u8 bDeviceSubClass;      /* subclass code */
  u8 bDeviceProtocol;      /* protocol code */
  u8 bMaxPacketSize0;      /* max packet size for endpoint 0 */
  u8 bNumConfigurations;   /* number of configurations */
  u8 bReserved;            /* reserved for future use */
} CellUsbdDeviceQualifierDescriptor;

/* 0x07 other_speed_configuration descriptor */
typedef struct _cellUsbd_other_speed_configuration_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant CELL_USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG */
  u16 wTotalLength;        /* number of bytes in descriptor and all children */
  u8 bNumInterfaces;       /* number of interfaces in config */
  u8 bConfigurationValue;  /* identifier for GET_CONFIGURATION / SET_CONFIGURATION */
  u8 iConfiguration;       /* index of string descriptor for the config */
  u8 bmAttributes;         /* self/bus power and remote wakeup settings */
  u8 MaxPower;             /* bus powered required as (max milliamps/2) */
} CellUsbdOtherSpeedConfigurationDescriptor;

/* 0x0b interface association descriptor */
typedef struct _cellUsbd_interface_association_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant CELL_USB_DESCRIPTOR_TYPE_INTERFACE_ASSOC */
  u8 bFirstInterface;      /* first associated interface with function */
  u8 bInterfaceCount;      /* number of contiguous interfaces associated with function */
  u8 bFunctionClass;       /* class code */
  u8 bFunctionSubClass;    /* subclass code */
  u8 bFunctionProtocol;    /* protocol code */
  u8 iFunction;            /* index of string descriptor for the function */
} CellUsbdInterfaceAssociationDescriptor;

/* HID descriptor info (class specific descriptor) */
typedef struct _cellUsbd_hid_sub_descriptor_info
{   
  u8 bDescriptorType; 
  u8 wDescriptorLength0;
  u8 wDescriptorLength1;
} CellUsbdHidSubDescriptorInfo;

/* 0x21 HID descriptor */
typedef struct _cellUsbd_hid_descriptor
{
  u8 bLength;              /* descriptor size in bytes */
  u8 bDescriptorType;      /* constant CELL_USB_DESCRIPTOR_TYPE_HID */
  u8 bcdHID0;
  u8 bcdHID1;
  u8 bCountryCode;
  u8 bNumDescriptors;      /* number of subdescriptor */
  CellUsbdHidSubDescriptorInfo subDescriptorInfo[0];
} CellUsbdHidDescriptor;

/*
typedef struct _cellUsbd_interface
{
  const struct CellUsbdInterfaceDescriptor *altsetting ATTRIBUTE_PRXPTR;
  s32 num_altsetting;
} CellUsbdInterface;
*/

/* */
typedef struct _cellUsbd_isoch_psw_len
{
  u16 len:11;
  u16 reserved:1;
  u16 PSW:4;
} CellUsbdIsochPswLen;

/* */
typedef struct _cellUsbd_hs_isoch_psw_len
{
  u16 len:12;
  u16 PSW:4;
} CellUsbdHSIsochPswLen;

/* */
typedef struct _cellUsbd_hs_isoch_request
{
  void *buffer_base ATTRIBUTE_PRXPTR;
  s32 relative_start_frame;
  s32 num_packets;
  CellUsbdHSIsochPswLen packets[CELL_USB_MAX_ISOCH_PACKETS];
} CellUsbdHSIsochRequest;

/* */
typedef struct _cellUsbd_isoch_request
{
  void *buffer_base ATTRIBUTE_PRXPTR;
  s32 relative_start_frame;
  s32 num_packets;
  CellUsbdIsochPswLen packets[CELL_USB_MAX_ISOCH_PACKETS];
} CellUsbdIsochRequest;


/*
 * struct aliases
 */

typedef CellUsbdControlSetup CellUsbdDeviceRequest;
typedef CellUsbdConfigDescriptor CellUsbdConfigurationDescriptor;


/*
 * callbacks
 */

/* Logical Device Driver (ldd) operations */
typedef struct _cellUsbd_ldd_ops
{
  const char *name;
  s32 (*probe)(s32 dev_id);
  s32 (*attach)(s32 dev_id);
  s32 (*detach)(s32 dev_id);
} CellUsbdLddOps;

/* transfer done */
typedef void (*CellUsbdDoneCallback)(s32 result, s32 count, void* arg);

/* isochronous transfer done */
typedef void (*CellUsbdIsochDoneCallback)(s32 result, CellUsbdIsochRequest *req, void *arg);

/* HS isochronous transfer done */
typedef void (*CellUsbdHSIsochDoneCallback)(s32 result, CellUsbdHSIsochRequest *req, void *arg);


/*
 * functions
 */ 

s32 cellUsbdInit(void);
s32 cellUsbdEnd(void);

s32 cellUsbdOpenPipe(s32 device_id, CellUsbdEndpointDescriptor *end_desc);
s32 cellUsbdClosePipe(s32 pipe_id);

s32 cellUsbdAllocateMemory(void **ptr, size_t size);
s32 cellUsbdFreeMemory(void *ptr);

void *cellUsbdScanStaticDescriptor(s32 device_id, void *ptr, unsigned char type);

s32 cellUsbdGetDeviceLocation(s32 device_id, unsigned char *location);
s32 cellUsbdGetDeviceSpeed(s32 device_id, u8 *speed);

void *cellUsbdGetPrivateData(s32 device_id);
s32 cellUsbdSetPrivateData(s32 device_id, void *priv);

s32 cellUsbdGetThreadPriority(s32 thread_type);
//s32 cellUsbdSetThreadPriority(UNKNOWN);
s32 cellUsbdSetThreadPriority2(s32 event_prio, s32 cellUsbd_prio, s32 callback_priority);

s32 cellUsbdRegisterLdd(CellUsbdLddOps *lddops);
s32 cellUsbdUnregisterLdd(CellUsbdLddOps *lddops);
s32 cellUsbdRegisterExtraLdd(CellUsbdLddOps *lddops, u16 vendor_id, u16 product_id);
s32 cellUsbdRegisterExtraLdd2(CellUsbdLddOps *lddops, u16 vendor_id, u16 product_id_min, u16 product_id_max);
s32 cellUsbdUnregisterExtraLdd(CellUsbdLddOps *lddops);

s32 cellUsbdBulkTransfer(s32 pipe_id, void *buf, s32 len, CellUsbdDoneCallback cb, void* arg);
s32 cellUsbdControlTransfer(s32 pipe_id, CellUsbdDeviceRequest *req, void *buf, CellUsbdDoneCallback cb, void *arg);
s32 cellUsbdInterruptTransfer(s32 pipe_id, void *buf, s32 len, CellUsbdDoneCallback cb, void *arg);
s32 cellUsbdIsochronousTransfer(s32 pipe_id, CellUsbdIsochRequest *req, CellUsbdIsochDoneCallback cb, void *arg);
s32 cellUsbdHSIsochronousTransfer(s32 pipe_id, CellUsbdHSIsochRequest *req, CellUsbdHSIsochDoneCallback cb, void *arg);


/*
 * standard control xfer macros
 */

#define cellUsbdClearDeviceFeature(pipe_id, fs, done_cb, arg) ({  \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x00; \
  device_request.bRequest = CELL_USB_REQUEST_CLEAR_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = 0; \
  device_request.wLength = 0; \
  cellUsbdControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define cellUsbdClearInterfaceFeature(pipe_id, fs, iInterface, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x01; \
  device_request.bRequest = CELL_USB_REQUEST_CLEAR_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = (iInterface); \
  device_request.wLength = 0; \
  cellUsbdControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define cellUsbdClearEndpointFeature(pipe_id, fs, endpoint, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x02; \
  device_request.bRequest = CELL_USB_REQUEST_CLEAR_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = (endpoint); \
  device_request.wLength = 0; \
  cellUsbdControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define cellUsbdGetConfiguration(pipe_id, ptr, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x80; \
  device_request.bRequest = CELL_USB_REQUEST_GET_CONFIGURATION; \
  device_request.wValue = 0; \
  device_request.wIndex = 0; \
  device_request.wLength = 1; \
  cellUsbdControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define cellUsbdGetDescriptor(pipe_id, type, wIndex, lang_id, ptr, len, done_cb, arg) \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x80; \
  device_request.bRequest = CELL_USB_REQUEST_GET_DESCRIPTOR; \
  device_request.wValue = ((type) << 8) | (wIndex); \
  device_request.wIndex = (lang_id); \
  device_request.wLength = (len); \
  cellUsbdControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg));

#define cellUsbdGetInterface(pipe_id, iInterface, ptr, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x81; \
  device_request.bRequest = CELL_USB_REQUEST_GET_INTERFACE; \
  device_request.wValue = 0; \
  device_request.wIndex = (iInterface); \
  device_request.wLength = 1; \
  cellUsbdControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define cellUsbdGetDeviceStatus(pipe_id, ptr, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x80; \
  device_request.bRequest = CELL_USB_REQUEST_GET_STATUS; \
  device_request.wValue = 0; \
  device_request.wIndex = 0; \
  device_request.wLength = 2; \
  cellUsbdControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define cellUsbdGetInterfaceStatus(pipe_id, iInterface, ptr, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x81; \
  device_request.bRequest = CELL_USB_REQUEST_GET_STATUS; \
  device_request.wValue = 0; \
  device_request.wIndex = (iInterface); \
  device_request.wLength = 2; \
  cellUsbdControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define cellUsbdGetEndpointStatus(pipe_id, endpoint, ptr, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x82; \
  device_request.bRequest = CELL_USB_REQUEST_GET_STATUS; \
  device_request.wValue = 0; \
  device_request.wIndex = (endpoint); \
  device_request.wLength = 2; \
  cellUsbdControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define cellUsbdSetAddress(pipe_id, address, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x00; \
  device_request.bRequest = CELL_USB_REQUEST_SET_ADDRESS; \
  device_request.wValue = (address); \
  device_request.wIndex = 0; \
  device_request.wLength = 0; \
  cellUsbdControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define cellUsbdSetConfiguration(pipe_id, config, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x00; \
  device_request.bRequest = CELL_USB_REQUEST_SET_CONFIGURATION; \
  device_request.wValue = (config); \
  device_request.wIndex = 0; \
  device_request.wLength = 0; \
  cellUsbdControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define cellUsbdSetDeviceDescriptor(pipe_id, type, wIndex, lang_id, ptr, len, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x00; \
  device_request.bRequest = CELL_USB_REQUEST_SET_DESCRIPTOR; \
  device_request.wValue = ((type) << 8) | (wIndex); \
  device_request.wIndex = (lang_id); \
  device_request.wLength = (len); \
  cellUsbdControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define cellUsbdSetInterfaceDescriptor(pipe_id, type, wIndex, lang_id, ptr, len, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x01; \
  device_request.bRequest = CELL_USB_REQUEST_SET_DESCRIPTOR; \
  device_request.wValue = ((type) << 8) | (wIndex); \
  device_request.wIndex = (lang_id); \
  device_request.wLength = (len); \
  cellUsbdControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define cellUsbdSetEndpointDescriptor(pipe_id, type, wIndex, lang_id, ptr, len, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x02; \
  device_request.bRequest = CELL_USB_REQUEST_SET_DESCRIPTOR; \
  device_request.wValue = ((type) << 8) | (wIndex); \
  device_request.wIndex = (lang_id); \
  device_request.wLength = (len); \
  cellUsbdControlTransfer((pipe_id), (&device_request), (ptr), (done_cb), (arg)); })

#define cellUsbdSetDeviceFeature(pipe_id, fs, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x00; \
  device_request.bRequest = CELL_USB_REQUEST_SET_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = 0; \
  device_request.wLength = 0; \
  cellUsbdControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define cellUsbdSetInterfaceFeature(pipe_id, fs, iInterface, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x01; \
  device_request.bRequest = CELL_USB_REQUEST_SET_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = (iInterface); \
  device_request.wLength = 0; \
  cellUsbdControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define cellUsbdSetEndpointFeature(pipe_id, fs, endpoint, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x02; \
  device_request.bRequest = CELL_USB_REQUEST_SET_FEATURE; \
  device_request.wValue = (fs); \
  device_request.wIndex = (endpoint); \
  device_request.wLength = 0; \
  cellUsbdControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define cellUsbdSetInterface(pipe_id, iInterface, alt_setting, done_cb, arg) ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x01; \
  device_request.bRequest = CELL_USB_REQUEST_SET_INTERFACE; \
  device_request.wValue = (alt_setting); \
  device_request.wIndex = (iInterface); \
  device_request.wLength = 0; \
  cellUsbdControlTransfer((pipe_id), (&device_request), NULL, (done_cb), (arg)); })

#define cellUsbdSynchFrame(pipe_id, endpoint, buf, done_cb, arg)  ({ \
  CellUsbdDeviceRequest device_request; \
  device_request.bmRequestType = 0x82; \
  device_request.bRequest = CELL_USB_REQUEST_SYNCH_FRAME; \
  device_request.wValue = 0; \
  device_request.wIndex = (endpoint); \
  device_request.wLength = 2; \
  cellUsbdControlTransfer((pipe_id), (&device_request), buf, (done_cb), (arg)); })


#endif /* __CELL_USB_H__ */

