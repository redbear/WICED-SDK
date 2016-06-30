/*
 * Copyright 2015, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/** @file
 *
 * BLE Proximity Reporter Sample Application
 *
 * Features demonstrated
 *  - WICED BT GATT server APIs
 *
 * On startup this demo:
 *  - Initializes the GATT subsystem
 *  - Begins advertising
 *  - Waits for GATT clients to connect
 *
 * Application Instructions
 *   Connect a PC terminal to the serial port of the WICED Eval board,
 *   then build and download the application as described in the WICED
 *   Quick Start Guide
 *
 */

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

#include "ble_central_multilink.h"
#include "ble_peripheral_nano.h"

#include "mqtt_api.h"
#include "resources.h"
#include "gpio_button.h"
#include "aws_config.h"
#include "aws_common.h"
#include "cjson.h"
#include <stdlib.h>
#include <malloc.h>


/******************************************************
 *                      Macros
 ******************************************************/

#define SHADOW_INIT "Hello"

#define CLIENT_ID   "aws_mul_RGB"


/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/


/******************************************************
 *                 Type Definitions
 ******************************************************/


/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

static void                   ble_scan_init      			( void );
static wiced_bt_dev_status_t  ble_scan_management_callback	( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data );
static wiced_bt_gatt_status_t ble_central_gatt_cback		( wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data );
static void 				  ble_scan_callback				(wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data);

/******************************************************
 *               Variable Definitions
 ******************************************************/


/* timer event variable */
static wiced_timed_event_t  rgb_status_event;

static const uint8_t service_uuid[16] = { 0x66, 0x7E, 0x50, 0x17, 0x55, 0x5E, 0xE9, 0x9C, 0xE5, 0x11, 0xBC, 0xF0, 0xF8, 0x3B, 0x2D, 0x5A };

static aws_app_info_t  app_info =
{
    .mqtt_client_id = CLIENT_ID
};

static uint16_t conn_id = 0;
static uint8_t OpCode = 0;
static uint8_t ID = 0;
static uint8_t NUM = 0;
static uint8_t R = 0;
static uint8_t G = 0;
static uint8_t B = 0;
static uint8_t rgb_temp[3] = {0,0,0};
static uint8_t is_nano_ok = 0;
static uint8_t current_connecting_num = 0xFF;
static uint8_t current_disconnecting_num = 0xFF;
static uint8_t current_discovered_num = 0xFF;

static wiced_thread_t mqtt_event_thread;

//extern unsigned char _eheap[];
//extern unsigned char *sbrk_heap_top;

/******************************************************
 *               Static Function Definitions
 ******************************************************/
static void print_System_memory( void )
{
    volatile struct mallinfo mi = mallinfo( );

    WPRINT_APP_INFO( ("\r\n-------------------SYS DBG MEM HEAP DIAGNOSTIC-----------------------\r\n") );
    WPRINT_APP_INFO( ("malloc_info {\r\n"
                      "\tarena:  \t%5d;\t/* total space allocated from system */\r\n"
                      "\tordblks: \t%5d;\t/* number of non-inuse chunks */\r\n"
                      "\thblks:  \t%5d;\t/* number of mmapped regions */\r\n"
                      "\thblkhd:  \t%5d;\t/* total space in mmapped regions */\r\n"
                      "\tuordblks:\t%5d;\t/* total allocated space */\r\n"
                      "\tfordblks:\t%5d;\t/* total non-inuse space */\r\n"
                      "\tkeepcost:\t%5d;\t/* top-most, releasable (via malloc_trim) space */\r\n"
                      "};\r\n",
                      mi.arena,
                      mi.ordblks,
                      mi.hblks,
                      mi.hblkhd,
                      mi.usmblks,
                      mi.fordblks,
                      mi.keepcost ));
}


/*
 * Call back function to handle connection events.
 */
wiced_result_t mqtt_connection_event_cb( wiced_mqtt_object_t mqtt_object, wiced_mqtt_event_info_t *event )
{

    switch ( event->type )
    {
        case WICED_MQTT_EVENT_TYPE_CONNECT_REQ_STATUS:
        case WICED_MQTT_EVENT_TYPE_DISCONNECTED:
        case WICED_MQTT_EVENT_TYPE_PUBLISHED:
        case WICED_MQTT_EVENT_TYPE_SUBCRIBED:
        case WICED_MQTT_EVENT_TYPE_UNSUBSCRIBED:
        {
            app_info.expected_event = event->type;
            wiced_rtos_set_semaphore( &app_info.msg_semaphore );
            break;
        }

        case WICED_MQTT_EVENT_TYPE_PUBLISH_MSG_RECEIVED:
        {
        	cJSON *original, *child;
            wiced_mqtt_topic_msg_t msg = event->data.pub_recvd;
            original = cJSON_Parse( strchr( (char*) msg.data, '{' ) );
			child = original->child;
			while ( child != NULL )
			{
				WPRINT_APP_INFO(("child->string [%s]\n", child->string));
				if ( strcmp( child->string, "ID" ) == 0 )
				{
					// child = cJSON_GetArrayItem( child, 0 );
					ID = child->valueint;
				}
				if ( strcmp( child->string, "NUM" ) == 0 )
				{
					NUM = child->valueint;

				}
				 if ( strcmp( child->string, "OpCode" ) == 0 )
				{
					OpCode = child->valueint;

				}
				 if ( strcmp( child->string, "R" ) == 0 )
				{
					rgb_temp[0] = child->valueint;

				}
				 if ( strcmp( child->string, "G" ) == 0 )
				{
					rgb_temp[1]= child->valueint;

				}
				 if ( strcmp( child->string, "B" ) == 0 )
				{
					rgb_temp[2] = child->valueint;
				}
				child = child->next;
			}
			WPRINT_APP_INFO(("OpCode: %d  NUM: %d ID: %d R: %d G: %d B: %d \r\n", OpCode, NUM, ID, rgb_temp[0], rgb_temp[1], rgb_temp[2]));
			 cJSON_Delete( original );
			 if(OpCode == 1)
			 {
				nano_writeStatus(ID-1,rgb_temp);
			 }
            break;
        }

        default:
            break;
    }
    return WICED_SUCCESS;
}

/*
 * Handles key events
 */
static void setpoint_control_keypad_handler( void *arg )
{
    WPRINT_APP_INFO(("Button is pressed\n"));
    wiced_rtos_set_semaphore( &app_info.wake_semaphore );


}

/******************************************************
 *               Function Definitions
 ******************************************************/

void send_number(uint8_t flag) {
	uint8_t num;

	// If all peripherals are connected, notify the number
	if(flag) num = NANO_NUM;
	else num = 0;

	cJSON * pSubJson = NULL;
	pSubJson = cJSON_CreateObject();
	cJSON_AddNumberToObject(pSubJson, "ID", 0xF0);
	cJSON_AddNumberToObject(pSubJson, "OpCode", OpCode);
	cJSON_AddNumberToObject(pSubJson, "NUM", num);
	cJSON_AddNumberToObject(pSubJson, "R", 0);
	cJSON_AddNumberToObject(pSubJson, "G", 0);
	cJSON_AddNumberToObject(pSubJson, "B", 0);

	char * p = cJSON_Print(pSubJson);

	WPRINT_APP_INFO(("status data : \r\n%s\r\n", p));

	aws_mqtt_app_publish( app_info.mqtt_object, WICED_MQTT_QOS_DELIVER_AT_LEAST_ONCE, (uint8_t*)app_info.shadow_state_topic, (uint8_t*)p ,strlen(p) );

	cJSON_Delete(pSubJson);
	free(p);

	wiced_rtos_delay_milliseconds( 200 );
	WPRINT_APP_INFO(("\r\n"));

}

void send_status(uint8_t *buf) {
	cJSON * pSubJson = NULL;
	pSubJson = cJSON_CreateObject();
	cJSON_AddNumberToObject(pSubJson, "ID", buf[1]);
	cJSON_AddNumberToObject(pSubJson, "OpCode", buf[0]);
	cJSON_AddNumberToObject(pSubJson, "NUM", 0xFF);
	cJSON_AddNumberToObject(pSubJson, "R", buf[2]);
	cJSON_AddNumberToObject(pSubJson, "G", buf[3]);
	cJSON_AddNumberToObject(pSubJson, "B", buf[4]);

	char * p = cJSON_Print(pSubJson);

	WPRINT_APP_INFO(("status data : \r\n%s\r\n", p));

	aws_mqtt_app_publish( app_info.mqtt_object, WICED_MQTT_QOS_DELIVER_AT_LEAST_ONCE, (uint8_t*)app_info.shadow_state_topic, (uint8_t*)p ,strlen(p) );

	cJSON_Delete(pSubJson);
	free(p);

	wiced_rtos_delay_milliseconds( MQTT_DELAY_IN_MILLISECONDS * 2 );
	WPRINT_APP_INFO(("\r\n"));
}

wiced_result_t rgb_changeBack( void* arg )
{
	wiced_gpio_output_low( WICED_LED1 );
	wiced_gpio_output_high( WICED_LED2 );
	wiced_gpio_output_low( WICED_LED3 );

	wiced_rtos_deregister_timed_event( &rgb_status_event );

	return WICED_TRUE;
}

void rgb_send_status()
{
	WPRINT_APP_INFO(("\r\n----Send data to client----\r\n"));

	cJSON * pSubJson = NULL;
	pSubJson = cJSON_CreateObject();
	WPRINT_APP_INFO(("\r\n connect handle %x \r\n",conn_id));
	ID = nano_getID(conn_id)+1;
	cJSON_AddNumberToObject(pSubJson, "ID", ID);
	cJSON_AddNumberToObject(pSubJson, "OpCode", OpCode);
	cJSON_AddNumberToObject(pSubJson, "NUM", NUM);
	cJSON_AddNumberToObject(pSubJson, "R", R);
	cJSON_AddNumberToObject(pSubJson, "G", G);
	cJSON_AddNumberToObject(pSubJson, "B", B);

	char * p = cJSON_Print(pSubJson);

	WPRINT_APP_INFO(("status data : \r\n%s\r\n", p));

	aws_mqtt_app_publish( app_info.mqtt_object, WICED_MQTT_QOS_DELIVER_AT_LEAST_ONCE, (uint8_t*)app_info.shadow_state_topic, (uint8_t*)p ,strlen(p) );

	cJSON_Delete(pSubJson);
	free(p);

	wiced_rtos_delay_milliseconds( MQTT_DELAY_IN_MILLISECONDS );
	WPRINT_APP_INFO(("\r\n"));
}

static uint8_t ble_advdata_parser(uint8_t type, uint8_t *p_advdata, uint8_t *len, uint8_t *p_field_data)
{
	uint8_t index=0;
	uint8_t field_length, field_type;

	while(index<31)
	{
		field_length = p_advdata[index];
		field_type	 = p_advdata[index+1];
		if(field_length == 0x00)
			return 1;
		if(field_type == type)
		{
			memcpy(p_field_data, &p_advdata[index+2], (field_length-1));
			*len = field_length - 1;
			return 0;
		}
		index += field_length + 1;
	}
	return 1;
}

void aws_setting()
{
	wiced_result_t   ret = WICED_SUCCESS;
	int              connection_retries = 0;
	ret = aws_app_init(&app_info);


	wiced_gpio_input_irq_enable( SETUP_BUTTON, IRQ_TRIGGER_RISING_EDGE, setpoint_control_keypad_handler, NULL );

	do
	{
		ret = aws_mqtt_conn_open( app_info.mqtt_object, mqtt_connection_event_cb );
		connection_retries++ ;
	} while ( ( ret != WICED_SUCCESS ) && ( connection_retries < WICED_MQTT_CONNECTION_NUMBER_OF_RETRIES ) );

	aws_mqtt_app_publish( app_info.mqtt_object, WICED_MQTT_QOS_DELIVER_AT_LEAST_ONCE, (uint8_t*)app_info.shadow_state_topic, (uint8_t*)SHADOW_INIT ,strlen(SHADOW_INIT) );

	wiced_rtos_delay_milliseconds( MQTT_DELAY_IN_MILLISECONDS * 2 );

	aws_mqtt_app_subscribe( app_info.mqtt_object, app_info.shadow_delta_topic , WICED_MQTT_QOS_DELIVER_AT_LEAST_ONCE );
}

void MQTT_thread(void)
{
	while(1)
	{
		/* Wait forever on wake semaphore until the wake button is pressed */
		wiced_rtos_get_semaphore( &app_info.wake_semaphore, WICED_NEVER_TIMEOUT );

		//WPRINT_APP_INFO( ("\nBefore JSON free heap: %d\r\n", (_eheap-sbrk_heap_top)) );
		malloc_trim(0);
		print_System_memory();

	}
}

/******************************************************
 *               Function Definitions
 ******************************************************/
void application_start( void )
{
	wiced_init( );

    wiced_gpio_output_low( WICED_LED1 );
    wiced_gpio_output_low( WICED_LED2 );
    wiced_gpio_output_low( WICED_LED3 );

	wiced_gpio_output_high( WICED_LED1 );
	wiced_gpio_output_low( WICED_LED2 );
	wiced_gpio_output_high( WICED_LED3 );

	//WPRINT_APP_INFO( ("\nInit free heap: %d\r\n", (_eheap-sbrk_heap_top)) );
	print_System_memory();

	aws_setting();
	//WPRINT_APP_INFO( ("\nAfter AWS up free heap: %d\r\n", (_eheap-sbrk_heap_top)) );
	print_System_memory();

	nano_init();
	/* Initialize Bluetooth controller and host stack */
	wiced_bt_stack_init( ble_scan_management_callback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools );
	//WPRINT_APP_INFO( ("\nAfter BLE up free heap: %d\r\n", (_eheap-sbrk_heap_top)) );
	print_System_memory();

	wiced_rtos_create_thread( &mqtt_event_thread, WICED_DEFAULT_LIBRARY_PRIORITY, "mqtt event", (wiced_thread_function_t)MQTT_thread, 512, NULL );

	//WPRINT_APP_INFO( ("\nAfter MQTT thread free heap: %d\r\n", (_eheap-sbrk_heap_top)) );
	print_System_memory();
}

/* Bluetooth management event handler */
static wiced_bt_dev_status_t ble_scan_management_callback( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data )
{
    wiced_bt_dev_status_t status = WICED_BT_SUCCESS;
    wiced_bt_device_address_t bda;

    WPRINT_BT_APP_INFO( ("ble_scan_management_callback \r\n") );
    switch ( event )
    {
        case BTM_ENABLED_EVT:
            /* Bluetooth controller and host stack enabled */
            WPRINT_BT_APP_INFO( ("Bluetooth enabled (%s)\n", ((p_event_data->enabled.status == WICED_BT_SUCCESS) ? "success":"failure")) );

            if ( p_event_data->enabled.status == WICED_BT_SUCCESS )
            {
                wiced_bt_dev_read_local_addr( bda );
                WPRINT_BT_APP_INFO( ("Local Bluetooth Address: [%02X:%02X:%02X:%02X:%02X:%02X]\n", bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]) );

                /* Enable simple_peripheral */
                ble_scan_init( );
            }
            break;

        case BTM_SECURITY_REQUEST_EVT:
            WPRINT_BT_APP_INFO( ("Security reqeust\n") );
            wiced_bt_ble_security_grant( p_event_data->security_request.bd_addr, WICED_BT_SUCCESS );
            break;

        case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT:
            WPRINT_BT_APP_INFO( ("Pairing IO Capabilities reqeust\n") );
            p_event_data->pairing_io_capabilities_ble_request.local_io_cap = BTM_IO_CAPABILIES_NONE; /* No IO capabilities on this platform */
            p_event_data->pairing_io_capabilities_ble_request.auth_req = BTM_LE_AUTH_REQ_BOND; /* Bonding required */
            break;

        case BTM_PAIRING_COMPLETE_EVT:
            WPRINT_BT_APP_INFO( ("Pairing complete %i.\n", p_event_data->pairing_complete.pairing_complete_info.ble.status) );
            break;

        case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
            /* adv state change callback */
            WPRINT_BT_APP_INFO( ("---->>> New ADV state: %d\n", p_event_data->ble_advert_state_changed) );
            break;

        case BTM_BLE_SCAN_STATE_CHANGED_EVT:
        	WPRINT_BT_APP_INFO( ("---->>> New Scan state: %d\n", p_event_data->ble_scan_state_changed) );
        	break;

        default:
            WPRINT_BT_APP_INFO( ("Unhandled Bluetooth Management Event: 0x%x\n", event) );
            break;
    }

    return ( status );
}

/* GATT event handler */
static wiced_bt_gatt_status_t ble_central_gatt_cback( wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data )
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_SUCCESS;
    uint8_t *bda;
    uint8_t flag=0;
    switch ( event )
    {
        case GATT_CONNECTION_STATUS_EVT:
            /* GATT connection status change */
            bda = p_event_data->connection_status.bd_addr;
            WPRINT_BT_APP_INFO( ("GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X] %s.\n", bda[0], bda[1], bda[2], bda[3], bda[4], bda[5], (p_event_data->connection_status.connected ? "established" : "released")) );

            if ( p_event_data->connection_status.connected )
            {

            	if (current_connecting_num != 0xFF) {
            		WPRINT_BT_APP_INFO( ("Connect to PERIPHERAL_NANO :%x \r\n",current_connecting_num));
            		WPRINT_BT_APP_INFO( ("connect_handle :  %x \r\n",  p_event_data->connection_status.conn_id) );
            		// Save conn_handle.
            		nano_setConnectID(current_connecting_num, p_event_data->connection_status.conn_id);
					current_connecting_num = 0xFF;
            	}

            	// Check whether all nano have been connected.
            	if (0xFF == nano_checkUnconnected()) {   // All device is connected.
            		WPRINT_BT_APP_INFO( ("ALL NANO Connected.\r\n"));
					for(uint8_t num = 0;num<NANO_NUM;num++)
					{
						WPRINT_BT_APP_INFO( ("connect_handle :  %x \r\n",  nano_getConnectID(num)) );
					}

            		/*stopScan*/
            		wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_NONE, TRUE, NULL);

					// Start discover service.
					current_discovered_num = nano_getNumOfUndiscovered();

					if (0xFF != current_discovered_num) {
						WPRINT_BT_APP_INFO( ("Start discovered : %x \r\n",current_discovered_num));
						nano_discoveryService(current_discovered_num);
					}
            	}
            	else {
            			WPRINT_BT_APP_INFO( ("Restart scanning."));
            			/*restart Scan*/
            			wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, FALSE, ble_scan_callback);
            	      }

            }
            else
            {
            	//is_connecting_flag = 0;
				wiced_gpio_output_high( WICED_LED1 );
				wiced_gpio_output_low( WICED_LED2 );
				wiced_gpio_output_low( WICED_LED3 );
				if (is_nano_ok) {
					is_nano_ok = 0;
					send_number(is_nano_ok);
				  }
				// Get the number of nano according to the connect_handle.
				WPRINT_BT_APP_INFO( ("connect id : %2x \r\n", p_event_data->connection_status.conn_id) );
				WPRINT_BT_APP_INFO( ("disconnect reason  : %2x \r\n", p_event_data->connection_status.reason) );

				current_disconnecting_num = nano_getID(p_event_data->connection_status.conn_id);
				if (0xFF != current_disconnecting_num) {
				  WPRINT_BT_APP_INFO( ("PERIPHERAL_NANO disconnected."));
				//Disconnected, reset the relevant variables.
				nano_setDiscoveredState(current_disconnecting_num, NANO_DISCOVERY_IDLE);
				nano_setConnectID(current_disconnecting_num, GATT_INVALID_CONN_ID);
				}

				if ( /*(is_connecting_flag == 0) &&*/ (0xFF != nano_checkUnconnected()) ) {   // When duo is connecting to device, don't start scanning.
					WPRINT_BT_APP_INFO(("Restart scanning."));
					/* Begin scan */
					wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, FALSE, ble_scan_callback);
				  }

            }


            break;

        case GATT_DISCOVERY_RESULT_EVT:
        	//WPRINT_BT_APP_INFO( ("GATT_DISCOVERY_RESULT_EVT \r\n") );
        	//WPRINT_BT_APP_INFO( ("current discovered num : %x \r\n",current_discovered_num));
			nano_discoveryResult(current_discovered_num,p_event_data);

        	break;

        case GATT_DISCOVERY_CPLT_EVT:
        	//WPRINT_BT_APP_INFO( ("GATT_DISCOVERY_CPLT_EVT \r\n") );

        	current_discovered_num = nano_getNumOfUndiscovered();
			nano_discoveryComplete(current_discovered_num);
			if(NANO_DISCOVERY_FINISH == nano_getDiscoveryState(current_discovered_num))
			{
				WPRINT_BT_APP_INFO( ("NANO %x Discovery_Finished \r\n",current_discovered_num) );

				flag = 1;
				nano_startNotify(current_discovered_num);
				current_discovered_num = nano_getNumOfUndiscovered();
			}
			if(flag == 1){
				//
				WPRINT_BT_APP_INFO( ("Next device ID : %x \r\n" ,current_discovered_num));
				if (0xFF != current_discovered_num) {   // Start discover other device.

					nano_discoveryService(current_discovered_num);
				    }
				    else {
				      // All peripherals are discovered.
				      is_nano_ok = 1;
				      send_number(is_nano_ok);
				      // set LED to white.
				      wiced_gpio_output_high( WICED_LED1 );
					  wiced_gpio_output_high( WICED_LED2 );
					  wiced_gpio_output_high( WICED_LED3 );
				      WPRINT_BT_APP_INFO( ("ALL NANO Discovery Finished \r\n") );
				    }
			}

        	break;

        case GATT_OPERATION_CPLT_EVT:
//        	WPRINT_BT_APP_INFO( (" \r\n----GATT_OPERATION_CPLT_EVT---- \r\n") );
//        	WPRINT_BT_APP_INFO( ("connect_handle :  %x \r\n",  p_event_data->operation_complete.conn_id) );
//        	WPRINT_BT_APP_INFO( ("op_type        :  %x \r\n",  p_event_data->operation_complete.op) );
//        	WPRINT_BT_APP_INFO( ("the len        :  %d \r\n",  p_event_data->operation_complete.response_data.att_value.len) );
//        	uint8_t index;
//        	WPRINT_BT_APP_INFO( ("the data       : ") );
//        	for(index=0; index<p_event_data->operation_complete.response_data.att_value.len; index++)
//        	{
//        		WPRINT_BT_APP_INFO( ("%x ", p_event_data->operation_complete.response_data.att_value.p_data[index] ) );
//        	}
//        	WPRINT_BT_APP_INFO( (" \r\n") );
        	if(p_event_data->operation_complete.op == GATTC_OPTYPE_NOTIFICATION)
			{

        		conn_id = p_event_data->operation_complete.conn_id;   //connection ID
//				WPRINT_BT_APP_INFO( ("connection ID :%x ",p_event_data->operation_complete.conn_id));
//				WPRINT_BT_APP_INFO( ("conn_id  :%x ",conn_id));
				OpCode = 0x00;                                                                 //OpCode
				NUM = 0xFF;   //NUM
				R = p_event_data->operation_complete.response_data.att_value.p_data[0];   //R
				G = p_event_data->operation_complete.response_data.att_value.p_data[1];   //G
				B = p_event_data->operation_complete.response_data.att_value.p_data[2];   //B
				rgb_send_status();
				wiced_gpio_output_high( WICED_LED1 );
				wiced_gpio_output_low( WICED_LED2 );
				wiced_gpio_output_low( WICED_LED3 );
				/* Start a notify timer task */
				wiced_rtos_register_timed_event( &rgb_status_event, WICED_HARDWARE_IO_WORKER_THREAD, &rgb_changeBack, 300, 0 );

			}
			else if(p_event_data->operation_complete.op == GATTC_OPTYPE_READ)
			{

			}

        	break;

        default:
        	WPRINT_BT_APP_INFO( ("Unhandled Bluetooth Gatt Event: 0x%x\n", event) );
            break;
    }

    return ( status );
}

static void ble_scan_callback(wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data)
{
	uint8_t len;
	uint8_t buf[31];

	WPRINT_BT_APP_INFO( ("\r\n--------ble_scan_callback-------- \r\n") );

	if(0x00 == ble_advdata_parser(BTM_BLE_ADVERT_TYPE_128SRV_COMPLETE, p_adv_data, &len, buf)) {
		WPRINT_BT_APP_INFO( ("The service uuid : ") );
		for(uint8_t index=0; index<16; index++){
			WPRINT_BT_APP_INFO( ("%x ",buf[index]) );
		}
		WPRINT_BT_APP_INFO( ("\r\n ") );
	}
	if(0x00 == memcmp(service_uuid, buf, 16)){
		// Get a number of unconnected nano in queue.
		current_connecting_num = nano_checkUnconnected();
		WPRINT_BT_APP_INFO( ("Find nano..\r\n ") );
		WPRINT_BT_APP_INFO( ("Current unconnected nano : %x \r\n",current_connecting_num));
		if (/*(is_connecting_flag == 0)&&*/  (0xFF != current_connecting_num)) {
			WPRINT_BT_APP_INFO( ("Found target Demo_Nano %d \r\n",current_connecting_num) );
			wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_NONE, TRUE, NULL);

			nano_setPeerAddr(current_connecting_num,p_scan_result);
			nano_connected(current_connecting_num);
			// Make sure no other connect operation when connecting to a device.
			//is_connecting_flag = 1;
		}
		else {
			WPRINT_BT_APP_INFO( ("no unconnected nano\r\n"));
			wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_NONE, TRUE, NULL);
	    }
	}
}


/* Initialize simple_peripheral */
static void ble_scan_init( void )
{
    /* Enable privacy */
    wiced_bt_ble_enable_privacy( TRUE );

    /* Register for gatt event notifications */
    wiced_bt_gatt_register( &ble_central_gatt_cback );

    /* Begin scan */
    wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, FALSE, ble_scan_callback);

    WPRINT_BT_APP_INFO( ("ble scan start \r\n") );
}

