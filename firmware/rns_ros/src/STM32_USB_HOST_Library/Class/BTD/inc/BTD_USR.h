
#ifndef STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_BTD_USR_H_
#define STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_BTD_USR_H_

#include "STM32_USB_HOST_Library/Core/inc/usbh_core.h"
#include "STM32_USB_HOST_Library/Core/inc/usb_conf.h"
#include <stdio.h>
#include "STM32_USB_HOST_Library/Class/BTD/inc/usbh_btd_core.h"
#include "STM32_USB_HOST_Library/Class/BTD/inc/PS3BT.h"
#include "STM32_USB_HOST_Library/Class/BTD/inc/PS4BT.h"
#include "BIOS/bios.h"


extern  USBH_Usr_cb_TypeDef BTD_USR_cb;

void USBH_USR_Init(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceAttached(void);
void USBH_USR_ResetDevice(void);
void USBH_USR_DeviceDisconnected (void);
void USBH_USR_OverCurrentDetected (void);
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed);
void USBH_USR_Device_DescAvailable(void *);
void USBH_USR_DeviceAddressAssigned(void);
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc);
void USBH_USR_Manufacturer_String(void *);
void USBH_USR_Product_String(void *);
void USBH_USR_SerialNum_String(void *);
void USBH_USR_EnumerationDone(void);
USBH_USR_Status USBH_USR_UserInput(void);
void USBH_USR_DeviceNotSupported(void);
void USBH_USR_UnrecoveredError(void);
int USBH_USR_BTD_Application(void);



#endif /* STM32_USB_HOST_LIBRARY_CLASS_BTD_INC_BTD_USR_H_ */
