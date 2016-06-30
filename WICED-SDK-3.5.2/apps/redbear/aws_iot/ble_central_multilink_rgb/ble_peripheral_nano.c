/*
 * Copyright 2015, RBL Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of RBL Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of RBL Corporation.
 */

/** @file
 *
 * BLE Discovered Service HRM Application
 *
 * On startup this demo:
 *  - Initializes the GATT subsystem
 *  - Begin scan
 *  - store Discovered information
 *
 * Application Instructions
 *   Connect a PC terminal to the serial port of the WICED Eval board,
 *   then build and download the application as described in the WICED
 *   Quick Start Guide
 *
 */

#include "ble_peripheral_nano.h"


/******************************************************
 *                 Type Definitions
 ******************************************************/

typedef struct{
	wiced_bt_gatt_char_properties_t	properties;
	uint16_t 						charHandle;
	uint16_t 						valueHandle;
	uint16_t						descHandle;
	uint16_t						cccdHandle;
	wiced_bt_uuid_t					uuid;
}Chars_t;

typedef struct{
	uint16_t 		start_handle;
	uint16_t 		end_handle;
	wiced_bt_uuid_t uuid;
}Service_t;


typedef struct{

	uint16_t 						connected_id;
	uint8_t 						ID;
	uint8_t                         ble_addr_type;
    wiced_bt_device_address_t       remote_bd_addr;

	struct {

    	Service_t	service;
    	Chars_t		chars[2];
    }Service;

    NanoDiscoveryState_t discoveryState;
    NanoDiscoveryChars_t discoveryDescriptorState;
}nano_t;

/******************************************************
 *               Variable Definitions
 ******************************************************/


static nano_t							        nano[NANO_NUM];

static uint8_t service_uuid[16]         ={0x5A, 0x2D, 0x3B, 0xF8, 0xF0, 0xBC, 0x11, 0xE5, 0x9C, 0xE9, 0x5E, 0x55, 0x17, 0x50, 0x7E, 0x66};
static uint8_t service_write_uuid[16]   ={0x5A, 0x2D, 0x40, 0xEE, 0xF0, 0xBC, 0x11, 0xE5, 0x9C, 0xE9, 0x5E, 0x55, 0x17, 0x50, 0x7E, 0x66};
static uint8_t service_notify_uuid[16]  ={0x5A, 0x2D, 0x42, 0x9C, 0xF0, 0xBC, 0x11, 0xE5, 0x9C, 0xE9, 0x5E, 0x55, 0x17, 0x50, 0x7E, 0x66};


/******************************************************
 *             GAP Function Definitions
 ******************************************************/
/******************************************************************************
**
** Function         nano_init
**
** Description      This function is called by an application to initialize
**					simpleContral device information.
**
** Returns          none.
**
******************************************************************************/
void nano_init()
{

	for(uint8_t num = 0;num<NANO_NUM;num++)
	{
		nano[num].connected_id	= GATT_INVALID_CONN_ID;
		nano[num].discoveryState = NANO_DISCOVERY_IDLE;
		nano[num].discoveryDescriptorState = NANO_SERVICE1_IDLE;
	}
}

/******************************************************************************
**
** Function         nano_setConnectID
**
** Description      This function is called by an application to set
**					chat device connect id.
**
** @param[in]		id : the device connect id.
**
** Returns          none.
**
******************************************************************************/
void nano_setConnectID(uint8_t num, uint16_t id)
{
	if(num >= NANO_NUM)
		return;
	nano[num].connected_id = id;
	nano_setID(num);
}
/******************************************************************************
**
** Function         nano_getConnectID
**
** Description      This function is called by an application to get
**					chat device connect id.
**
** Returns          chat device connect id.
**
******************************************************************************/
uint16_t nano_getConnectID(uint8_t num)
{
	if(num >= NANO_NUM)
		return GATT_INVALID_CONN_ID;

	return nano[num].connected_id;
}

/******************************************************************************
**
** Function         nano_setID
**
** Description      This function is called by an application to set
**					nano id.
**
** @param[in]		id : the nano id.
**
** Returns          none.
**
******************************************************************************/
void nano_setID(uint8_t ID)
{
	if(ID >= NANO_NUM)
		return;

	nano[ID].ID = ID+1;
}

/******************************************************************************
**
** Function         nano_setID
**
** Description      This function is called by an application to set
**					nano id.
**
** @param[in]		id : the nano id.
**
** Returns          none.
**
******************************************************************************/
uint8_t nano_getID(uint16_t ConnectID)
{
	for(uint8_t ID = 0;ID<NANO_NUM;ID++)
	{

		if(nano_getConnectID(ID) == ConnectID)
		{
			//WPRINT_APP_INFO( ("nano_get_conn_ID: %x \r\n",nano_getConnectID(ID)));
			return ID;
		}
	}
	return 0xFF;
}
/******************************************************************************
**
** Function         nano_setPeerAddr
**
** Description      This function is called by an application to set
**					chat device connect id.
**
** @param[in]		id : the device connect id.
**
** Returns          none.
**
******************************************************************************/
void nano_setPeerAddr(uint8_t num, wiced_bt_ble_scan_results_t *p_scan_result)
{
	if(num >= NANO_NUM)
		return;

	nano[num].ble_addr_type  = p_scan_result->ble_addr_type;
	memcpy( nano[num].remote_bd_addr, p_scan_result->remote_bd_addr, 0x06);
}

/******************************************************************************
**
** Function         nano_connected
**
** Description      This function is called by an application to connect to
**					chat device.
**
** @param[in]		*p_scan_result : discovery service result.
**
** Returns          none.
**
******************************************************************************/
wiced_bool_t nano_connected(uint8_t num)
{
	if(num >=NANO_NUM)
		return WICED_FALSE;

	return wiced_bt_gatt_le_connect (nano[num].remote_bd_addr,
									 BLE_ADDR_RANDOM,
									 BLE_CONN_MODE_HIGH_DUTY,
									 WICED_TRUE);
}

/******************************************************************************
**
** Function         nano_getDiscoveryState
**
** Description      This function is called by an application to get
**					current discovery state of device.
**
** @param[in]		*p_scan_result : discovery service result.
**
** Returns          none.
**
******************************************************************************/
NanoDiscoveryState_t nano_getDiscoveryState(uint8_t num)
{
	if(num > NANO_NUM)
		return 0xFF;

	return nano[num].discoveryState;
}


/******************************************************
 *              GATT Function Definitions
 ******************************************************/
/******************************************************************************
**
** Function         nano_startNotify
**
** Description      This function is called by an application to start notify.
**
** Returns          none.
**
******************************************************************************/
void nano_startNotify(uint8_t num)
{
	uint8_t buf[2] = {0x01, 0x00};
    wiced_bt_gatt_value_t write_value;

    write_value.handle      = nano[num].Service.chars[1].cccdHandle;
    write_value.len         = 2;
    memcpy(write_value.value, buf, 2);
    write_value.auth_req    = GATT_AUTH_REQ_NONE;
    wiced_bt_gatt_send_write (nano[num].connected_id, GATT_WRITE_NO_RSP, &write_value);
}

void nano_writeStatus(uint8_t num, uint8_t *buf)
{
	wiced_bt_gatt_value_t write_value;

    write_value.handle      = nano[num].Service.chars[0].valueHandle;
    write_value.len         = 3;
    memcpy(write_value.value, buf, 3);
    write_value.auth_req    = GATT_AUTH_REQ_NONE;
    wiced_bt_gatt_send_write (nano[num].connected_id, GATT_WRITE_NO_RSP, &write_value);
}

void nano_readStatus(uint8_t num)
{
	wiced_bt_gatt_read_param_t read_param;

	read_param.by_handle.handle = nano[num].Service.chars[1].valueHandle;
	read_param.by_handle.auth_req = GATT_AUTH_REQ_NONE;
    wiced_bt_gatt_send_read(nano[num].connected_id, GATT_READ_BY_HANDLE, &read_param);
}
/******************************************************************************
**
** Function         nano_discoveryComplete
**
** Description      This function is called by an application to handle the
** 					discovery complete.
**
** Returns          none.
**
******************************************************************************/
void nano_discoveryComplete(uint8_t num)
{
	wiced_bt_gatt_discovery_param_t discovery_param;

	if(nano[num].discoveryState == NANO_DISCOVERY_SERVICES)
	{
		if(nano[num].Service.service.start_handle)
		{
			nano[num].discoveryState = NANO_DISCOVERY_CHARS_OF_SERVICE1;

			memset( &discovery_param.uuid, 0, sizeof( discovery_param.uuid ) );
			discovery_param.s_handle = nano[num].Service.service.start_handle;
			discovery_param.e_handle = nano[num].Service.service.end_handle;
			wiced_bt_gatt_send_discover(nano[num].connected_id, GATT_DISCOVER_CHARACTERISTICS, &discovery_param);
		}
	}
	else if(nano[num].discoveryState == NANO_DISCOVERY_CHARS_OF_SERVICE1)
	{
		nano[num].discoveryState = NANO_DISCOVERY_DESCRIPTOR_OF_SERVICE1;

		memset( &discovery_param.uuid, 0, sizeof( discovery_param.uuid ) );
		discovery_param.s_handle = nano[num].Service.service.start_handle;
		discovery_param.e_handle = nano[num].Service.service.end_handle;
		wiced_bt_gatt_send_discover(nano[num].connected_id, GATT_DISCOVER_CHARACTERISTIC_DESCRIPTORS, &discovery_param);
	}
	else if(nano[num].discoveryState == NANO_DISCOVERY_DESCRIPTOR_OF_SERVICE1)
	{
//		WPRINT_BT_APP_INFO( ("--------- NANO_DISCOVERY_DESCRIPTOR_OF_SERVICE1 ----------\r\n") );
//		for(uint8_t index=0; index<2; index++)
//		{
//			WPRINT_BT_APP_INFO( ("--------- characteristice %d-------- \r\n", index) );
//			WPRINT_BT_APP_INFO( ("UUID              : "));
//			for(uint8_t n=0; n<16; n++)
//			{
//				WPRINT_BT_APP_INFO( ("%x ", nano[num].Service.chars[index].uuid.uu.uuid128[n]) );
//			}
//			WPRINT_BT_APP_INFO( ("\r\n"));
//			WPRINT_BT_APP_INFO( ("charHandle        : %2x\r\n", nano[num].Service.chars[index].charHandle) );
//			WPRINT_BT_APP_INFO( ("valueHandle       : %2x\r\n", nano[num].Service.chars[index].valueHandle) );
//			WPRINT_BT_APP_INFO( ("descHandle        : %2x\r\n", nano[num].Service.chars[index].descHandle) );
//			WPRINT_BT_APP_INFO( ("CCCDHandle        : %2x\r\n", nano[num].Service.chars[index].cccdHandle) );
//			WPRINT_BT_APP_INFO( ("properties        : %x\r\n",  nano[num].Service.chars[index].properties) );
//		}

		nano[num].discoveryState = NANO_DISCOVERY_FINISH;
	}
}

/******************************************************************************
**
** Function         nano_discoveryResult
**
** Description      This function is called by an application to handle the
** 					discovery resutl.
**
** @param[in]		*p_event_data : the discovery respond data.
**
** Returns          none.
**
******************************************************************************/
void nano_discoveryResult(uint8_t num, wiced_bt_gatt_event_data_t *p_event_data)
{
	wiced_bt_uuid_t uuid;
	//WPRINT_BT_APP_INFO( ("SimpleContral_discoveryResult \r\n"));
	if(nano[num].discoveryState == NANO_DISCOVERY_SERVICES)
	{   //WPRINT_BT_APP_INFO( ("NANO_DISCOVERY_SERVICES \r\n"));
		if(GATT_DISCOVERY_RESULT_SERVICE_UUID_LEN(p_event_data) == LEN_UUID_128)
		{
			uuid.len = LEN_UUID_128;
			for(uint8_t index=0; index<16; index++)
			{
				uuid.uu.uuid128[16-index-1] = (GATT_DISCOVERY_RESULT_SERVICE_UUID128(p_event_data))[index];
			}

			if(0x00 == memcmp(uuid.uu.uuid128, service_uuid, 16))
			{
				//WPRINT_BT_APP_INFO( ("Find service \r\n"));
				nano[num].Service.service.uuid = uuid;
				nano[num].Service.service.start_handle = GATT_DISCOVERY_RESULT_SERVICE_START_HANDLE(p_event_data);
				nano[num].Service.service.end_handle   = GATT_DISCOVERY_RESULT_SERVICE_END_HANDLE(p_event_data);
			}
			else
			{
				//WPRINT_BT_APP_INFO( ("others service \r\n") );
			}
		}
		else if(GATT_DISCOVERY_RESULT_SERVICE_UUID_LEN(p_event_data) == LEN_UUID_16)
		{
			//WPRINT_BT_APP_INFO( ("others service 16bit  \r\n") );
		}
	}
	else if(nano[num].discoveryState == NANO_DISCOVERY_CHARS_OF_SERVICE1)
	{
		if(GATT_DISCOVERY_RESULT_CHARACTERISTIC_UUID_LEN(p_event_data) == LEN_UUID_128)
		{
			uuid.len = LEN_UUID_128;
			for(uint8_t index=0; index<16; index++)
			{
				uuid.uu.uuid128[16-index-1] = (GATT_DISCOVERY_RESULT_CHARACTERISTIC_UUID128(p_event_data))[index];
			}
			if(0x00 == memcmp(uuid.uu.uuid128, service_write_uuid, 16))
			{
				//WPRINT_BT_APP_INFO( ("char1 \r\n") );
				nano[num].Service.chars[0].uuid = uuid;
				nano[num].Service.chars[0].charHandle = p_event_data->discovery_result.discovery_data.characteristic_declaration.handle;
				nano[num].Service.chars[0].valueHandle = GATT_DISCOVERY_RESULT_CHARACTERISTIC_VALUE_HANDLE(p_event_data);
				nano[num].Service.chars[0].properties  = p_event_data->discovery_result.discovery_data.characteristic_declaration.characteristic_properties;
			}
			else if(0x00 == memcmp(uuid.uu.uuid128, service_notify_uuid, 16))
			{
				//WPRINT_BT_APP_INFO( ("char2  \r\n") );
				nano[num].Service.chars[1].uuid = uuid;
				nano[num].Service.chars[1].charHandle = p_event_data->discovery_result.discovery_data.characteristic_declaration.handle;
				nano[num].Service.chars[1].valueHandle = GATT_DISCOVERY_RESULT_CHARACTERISTIC_VALUE_HANDLE(p_event_data);
				nano[num].Service.chars[1].properties  = p_event_data->discovery_result.discovery_data.characteristic_declaration.characteristic_properties;
			}
			else
			{
				//WPRINT_BT_APP_INFO( ("other chars \r\n") );
			}
		}
		else if(GATT_DISCOVERY_RESULT_CHARACTERISTIC_UUID_LEN(p_event_data) == LEN_UUID_16)
		{
			//WPRINT_BT_APP_INFO( ("other chars 16bit \r\n") );
		}
	}
	else if(nano[num].discoveryState == NANO_DISCOVERY_DESCRIPTOR_OF_SERVICE1)
	{
		if(GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID_LEN(p_event_data) == LEN_UUID_128)
		{
			uuid.len = LEN_UUID_128;
			for(uint8_t index=0; index<16; index++)
			{
				uuid.uu.uuid128[16-index-1] = (GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID128(p_event_data))[index];
			}
			if(0x00 == memcmp(uuid.uu.uuid128, service_write_uuid, 16))
			{
				nano[num].discoveryDescriptorState = NANO_SERVICE1_WRITE;
				//simpleContral.Service1.chars[0].uuid = service_uuid;
				//simpleContral.Service1.chars[0].valueHandle = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
			}
			else if(0x00 == memcmp(uuid.uu.uuid128, service_notify_uuid, 16))
			{
				nano[num].discoveryDescriptorState = NANO_SERVICE1_NOTIFY;
				//simpleContral.Service1.chars[1].uuid = service_uuid;
				//simpleContral.Service1.chars[1].valueHandle = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
			}
		}
		else if(GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID_LEN(p_event_data) == LEN_UUID_16)
		{
			if(GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID16(p_event_data) == 0x2901)
			{
				if(nano[num].discoveryDescriptorState == NANO_SERVICE1_WRITE)
					nano[num].Service.chars[0].descHandle = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
				else if(nano[num].discoveryDescriptorState == NANO_SERVICE1_NOTIFY)
					nano[num].Service.chars[1].descHandle = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
			}
			else if(GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID16(p_event_data) == 0x2902)
			{
				if(nano[num].discoveryDescriptorState == NANO_SERVICE1_WRITE)
					nano[num].Service.chars[0].cccdHandle = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
				else if(nano[num].discoveryDescriptorState == NANO_SERVICE1_NOTIFY)
					nano[num].Service.chars[1].cccdHandle = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
			}
		}
	}
}

/******************************************************************************
**
** Function         nano_discoveryService
**
** Description      This function is called by an application to start discovery
**					chat device information.
**
** Returns          wiced_bt_gatt_status_t.
**
******************************************************************************/
wiced_bt_gatt_status_t nano_discoveryService(uint8_t num)
{
    wiced_bt_gatt_discovery_param_t discovery_param;

    memset(&discovery_param, 0, sizeof(wiced_bt_gatt_discovery_param_t));

    discovery_param.uuid.len 		= LEN_UUID_16;
    discovery_param.s_handle 		= 0x0001;
    discovery_param.e_handle 		= 0xFFFF;

    nano[num].discoveryState = NANO_DISCOVERY_SERVICES;

    return wiced_bt_gatt_send_discover(nano[num].connected_id, GATT_DISCOVER_SERVICES_ALL, &discovery_param);
}


uint8_t nano_checkUnconnected(void) {
  for (uint8_t index = 0; index < NANO_NUM; index++) {
    if(nano[index].connected_id == GATT_INVALID_CONN_ID) return index;
  }
  return 0xFF;
}

void nano_setDiscoveredState(uint8_t num, NanoDiscoveryState_t state) {
  if (num >= NANO_NUM) return;

  nano[num].discoveryState = state;
}

uint8_t nano_getNumOfUndiscovered(void) {
  for (uint8_t index = 0; index < NANO_NUM; index++) {
    if (nano[index].discoveryState != NANO_DISCOVERY_FINISH) return index;
  }
  return 0xFF;
}
