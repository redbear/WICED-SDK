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

#ifndef BLE_DISCOVERED_SIMPLE_CONTRAL_H_
#define BLE_DISCOVERED_SIMPLE_CONTRAL_H_


#include <string.h>
#include <stdio.h>
#include "wiced.h"
#include "bt_target.h"
#include "wiced_bt_stack.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "sdpdefs.h"


#define NANO_NUM 8


typedef enum{

	NANO_DISCOVERY_IDLE,
	NANO_DISCOVERY_SERVICES,
	NANO_DISCOVERY_CHARS_OF_SERVICE1,
	NANO_DISCOVERY_DESCRIPTOR_OF_SERVICE1,

	NANO_DISCOVERY_FINISH,
}NanoDiscoveryState_t;

typedef enum{

	NANO_SERVICE1_IDLE,

	NANO_SERVICE1_WRITE,
	NANO_SERVICE1_NOTIFY,

}NanoDiscoveryChars_t;


void nano_init();
void nano_setConnectID(uint8_t num, uint16_t id);
uint16_t nano_getConnectID(uint8_t num);
void nano_setID(uint8_t ID);
uint8_t nano_getID(uint16_t ConnectID);
void nano_setPeerAddr(uint8_t num, wiced_bt_ble_scan_results_t *p_scan_result);
wiced_bool_t nano_connected(uint8_t num);
NanoDiscoveryState_t nano_getDiscoveryState(uint8_t num);
void nano_discoveryComplete(uint8_t num);
void nano_discoveryResult(uint8_t num, wiced_bt_gatt_event_data_t *p_event_data);
wiced_bt_gatt_status_t nano_discoveryService(uint8_t num);


void nano_startNotify(uint8_t num);
void nano_writeStatus(uint8_t num, uint8_t *buf);
void nano_readStatus(uint8_t num);

wiced_bt_gatt_status_t nano_discoveryService(uint8_t num);
uint8_t nano_checkUnconnected(void);
void nano_setDiscoveredState(uint8_t num, NanoDiscoveryState_t state);
uint8_t nano_getNumOfUndiscovered(void);

#endif


