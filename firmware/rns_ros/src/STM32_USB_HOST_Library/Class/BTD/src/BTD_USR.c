#include "STM32_USB_HOST_Library/Class/BTD/inc/BTD_USR.h"



USBH_Usr_cb_TypeDef BTD_USR_cb =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_BTD_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError

};

const uint8_t BTD_HOST_INIT[]        = "> Host Library Initialized";
const uint8_t BTD_DEV_ATTACHED[]     = "> Device Attached";
const uint8_t BTD_DEV_DISCONNECTED[] = "> Device Disconnected";
const uint8_t BTD_DEV_ENUMERATED[]   = "> Enumeration completed";
const uint8_t BTD_DEV_HIGHSPEED[]    = "> High speed device detected";
const uint8_t BTD_DEV_FULLSPEED[]    = "> Full speed device detected";
const uint8_t BTD_DEV_LOWSPEED[]     = "> Low speed device detected";
const uint8_t BTD_DEV_ERROR[]        = "> Device fault";
const uint8_t BTD_UNREC_ERROR[]      = "> UNRECOVERED ERROR STATE";


/**
* @brief  USBH_USR_Init
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
	sprintf(btduartbuff,"%s\r\n", &BTD_HOST_INIT);
	UARTPrintString(PRINTING_USART,btduartbuff);
}

/**
* @brief  USBH_USR_DeviceAttached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
	sprintf(btduartbuff,"%s\r\n", &BTD_DEV_ATTACHED);
	UARTPrintString(PRINTING_USART,btduartbuff);
}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{
	sprintf(btduartbuff,"%s\r\n", &BTD_UNREC_ERROR);
    UARTPrintString(PRINTING_USART,btduartbuff);
}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Status
*/
void USBH_USR_DeviceDisconnected (void)
{
	sprintf(btduartbuff,"%s \r\n", &BTD_DEV_DISCONNECTED);
	UARTPrintString(USART1,btduartbuff);
}

/**
* @brief  USBH_USR_ResetUSBDevice
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* callback for USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected
*         Displays the message on LCD for device speed
* @param  Device speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{

  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
	  sprintf(btduartbuff,"%s \r\n", &BTD_DEV_HIGHSPEED);
	  UARTPrintString(PRINTING_USART,btduartbuff);
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
	  sprintf(btduartbuff,"%s \r\n", &BTD_DEV_FULLSPEED);
	  UARTPrintString(PRINTING_USART,btduartbuff);
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
	  sprintf(btduartbuff,"%s \r\n", &BTD_DEV_LOWSPEED);
	  UARTPrintString(PRINTING_USART,btduartbuff);
  }
  else
  {
	  sprintf(btduartbuff,"%s \r\n", &BTD_DEV_ERROR);
	  UARTPrintString(PRINTING_USART,btduartbuff);
  }
}

/**
* @brief  USBH_USR_Device_DescAvailable
*         Displays the message on LCD for device descriptor
* @param  device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;

  sprintf(btduartbuff,"VID : %x\r\n" , (uint32_t)(*hs).idVendor);
  UARTPrintString(PRINTING_USART,btduartbuff);
  sprintf(btduartbuff,"PID : %x\r\n" , (uint32_t)(*hs).idProduct);
  UARTPrintString(PRINTING_USART,btduartbuff);
  sprintf(btduartbuff,"Device Class: %X\r\n" , (uint32_t)(*hs).bDeviceClass);
  UARTPrintString(PRINTING_USART,btduartbuff);
  sprintf(btduartbuff,"Device Protocol: %X\r\n" , (uint32_t)(*hs).bDeviceProtocol);
  UARTPrintString(PRINTING_USART,btduartbuff);
  sprintf(btduartbuff,"Device Subclass: %X\r\n" , (uint32_t)(*hs).bDeviceSubClass);
  UARTPrintString(PRINTING_USART,btduartbuff);
  sprintf(btduartbuff,"Device Subclass: %X\r\n" , (uint32_t)(*hs).bDeviceSubClass);
  UARTPrintString(PRINTING_USART,btduartbuff);

}

/**
* @brief  USBH_USR_DeviceAddressAssigned
*         USB device is successfully assigned the Address
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{

}


/**
* @brief  USBH_USR_Conf_Desc
*         Displays the message on LCD for configuration descriptor
* @param  Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
  USBH_InterfaceDesc_TypeDef *id;
  USBH_CfgDesc_TypeDef * id2;
  USBH_EpDesc_TypeDef * ep0;
  id = itfDesc;
  id2 = cfgDesc;
  ep0 = epDesc;

  sprintf(btduartbuff,"Interface Class %x\r\n",(*id).bInterfaceClass);
  UARTPrintString(PRINTING_USART, btduartbuff);

  sprintf(btduartbuff,"Interface Sub class %x\r\n",(*id).bInterfaceSubClass);
   UARTPrintString(PRINTING_USART, btduartbuff);

   sprintf(btduartbuff,"Interface Protocol %x\r\n",(*id).bInterfaceProtocol);
   UARTPrintString(PRINTING_USART, btduartbuff);

   sprintf(btduartbuff,"Number of Interface %x\r\n",(*id2).bConfigurationValue);
   UARTPrintString(PRINTING_USART, btduartbuff);

   sprintf(btduartbuff,"EP0 %d %d \r\n",(*ep0).bEndpointAddress,(*ep0).bmAttributes );
   UARTPrintString(PRINTING_USART, btduartbuff);



}

/**
* @brief  USBH_USR_Manufacturer_String
*         Displays the message on LCD for Manufacturer String
* @param  Manufacturer String
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
	sprintf(btduartbuff,"Manufacturer : %s \r\n", (char *)ManufacturerString);
    UARTPrintString(PRINTING_USART,btduartbuff);

}

/**
* @brief  USBH_USR_Product_String
*         Displays the message on LCD for Product String
* @param  Product String
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{

	sprintf(btduartbuff,"Product : %s \r\n", (char *)ProductString);
	UARTPrintString(PRINTING_USART,btduartbuff);
}

/**
* @brief  USBH_USR_SerialNum_String
*         Displays the message on LCD for SerialNum_String
* @param  SerialNum_String
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{

	sprintf(btduartbuff,"Serial Number : %s \r\n", (char *)SerialNumString);
	UARTPrintString(PRINTING_USART,btduartbuff);
}



/**
* @brief  EnumerationDone
*         User response request is displayed to ask application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{
	sprintf(btduartbuff,"%s \r\n", &BTD_DEV_ENUMERATED);
    UARTPrintString(PRINTING_USART,btduartbuff);
}


/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
	sprintf(btduartbuff,"No registered class for this device. \r\n");
    UARTPrintString(PRINTING_USART,btduartbuff);
}


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{
  USBH_USR_Status usbh_usr_status;
  usbh_usr_status = USBH_USR_RESP_OK;
  return usbh_usr_status;
}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Over Current Detected on VBUS
* @param  None
* @retval Status
*/
void USBH_USR_OverCurrentDetected (void)
{
	sprintf(btduartbuff,"Overcurrent detected. \r\n");
    UARTPrintString(PRINTING_USART,btduartbuff);
}


/**
* @brief  USBH_USR_MSC_Application
*         Demo application for mass storage
* @param  None
* @retval Status
*/
int USBH_USR_BTD_Application(void){

  return 1;
}




/**
* @brief  USBH_USR_DeInit
*         Deinit User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{

}


