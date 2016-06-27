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
 * AWS Thing Shadow Demo Application
 *
 * This application demonstrates how you can publish state from WICED and get sync with AWS thing shadow.
 *
 * To demonstrate the app, work through the following steps.
 *  1. Modify the CLIENT_AP_SSID/CLIENT_AP_PASSPHRASE Wi-Fi credentials
 *     in the wifi_config_dct.h header file to match your Wi-Fi access point
 *  2. Plug the WICED eval board into your computer
 *  4. Open a terminal application and connect to the WICED eval board
 *  5. Build and download the application (to the WICED board)
 *
 * Once WICED device boots up it will publish current state of LIGHT (LED) that will be OFF on topic
 * $shadow/beta/state/DEMO. same WICED device subscribe to topic $shadow/beta/sync/DEMO to get the latest
 * state and sync with AWS.
 *
 * NOTE : DEMO is the thing name here created in AWS. You can create new thing and change here.
 *
 * After above steps from AWS CLI you can publish and change the state of LIGHT. You can refer below command as \
 * example.
 *
 * aws iot-data --endpoint-url https://data.iot.us-east-1.amazonaws.comupdate-thing-state update-thing-state --thing-name lightbulb
 * --payload "{ \"state\": {\"desired\": { \"status\": \"ON\" } } }" output.txt && output.txt
 *
 * After successful execution of above commands you can check that LED will be turned on in WICED device and publishes
 * same with reported state.
 *
 * You can also change status of LED manually with push button on WICED and WICED board publishes its new state with
 * $shadow/beta/state/DEMO. but AWS thing shadow gives back delta because reported state is overwritten with desired
 * state and it sends delta back. This is something needs to be debugged in AWS shadow part from Amazon.
 **/

#include "wiced.h"
#include "mqtt_api.h"
#include "resources.h"
#include "gpio_button.h"
#include "JSON.h"
#include "aws_config.h"
#include "aws_common.h"
#include <malloc.h>

/******************************************************
 *                      Macros
 ******************************************************/
#define SHADOW_PUBLISH_MESSAGE_STR_OFF      "{ \"state\": {\"reported\": { \"status\": \"OFF\" } } }"
#define SHADOW_PUBLISH_MESSAGE_STR_ON       "{ \"state\": {\"reported\": { \"status\": \"ON\" } } }"
#define SHADOW_PUBLISH_MESSAGE_STR_OFF_DESIRED_AND_REPORTED     "{ \"state\": {\"desired\": { \"status\": \"OFF\" } ,\"reported\": { \"status\": \"OFF\" } } }"
#define SHADOW_PUBLISH_MESSAGE_STR_ON_DESIRED_AND_REPORTED      "{ \"state\": {\"desired\": { \"status\": \"ON\" } ,\"reported\": { \"status\": \"ON\" } } }"

#define CLIENT_ID                           "wiced_shadow_aws"

/******************************************************
 *               Variable Definitions
 ******************************************************/
static aws_app_info_t  app_info =
{
    .mqtt_client_id = CLIENT_ID
};

static char* led_status = "OFF";
static char  req_led_status[8] = "OFF";

/******************************************************
 *               Static Function Definitions
 ******************************************************/
static wiced_result_t parse_json_shadow_status(wiced_json_object_t * json_object )
{
    if(strncmp(json_object->object_string, "status", sizeof("status")-1) == 0)
    {
        if(json_object->value_length > 0 && json_object->value_length < sizeof(req_led_status)-1)
        {
            memcpy(req_led_status, json_object->value, json_object->value_length);
            req_led_status[json_object->value_length] = '\0';
        }
    }

    return WICED_SUCCESS;
}

/*
 * Call back function to handle connection events.
 */
wiced_result_t mqtt_connection_event_cb( wiced_mqtt_object_t mqtt_object, wiced_mqtt_event_info_t *event )
{
    wiced_result_t ret = WICED_SUCCESS;

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
            wiced_mqtt_topic_msg_t msg = event->data.pub_recvd;
            WPRINT_APP_INFO(("Received %.*s  for TOPIC : %.*s\n\n", (int) msg.data_len, msg.data, (int) msg.topic_len, msg.topic));

            ret = wiced_JSON_parser( (const char*)msg.data , msg.data_len );
            if(ret == WICED_SUCCESS)
            {
                WPRINT_APP_INFO(("Requested LED State[%s] Current LED State [%s]\n", req_led_status, led_status));
                if ( strcasecmp( req_led_status, led_status ) == 0 )
                {
                    break;
                }
                else
                {
                    wiced_rtos_set_semaphore( &app_info.wake_semaphore );
                }
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
void application_start( void )
{
    wiced_result_t   ret = WICED_SUCCESS;
    int              connection_retries = 0;

    ret = aws_app_init(&app_info);

    wiced_JSON_parser_register_callback(parse_json_shadow_status);

    wiced_gpio_input_irq_enable( SETUP_BUTTON, IRQ_TRIGGER_RISING_EDGE, setpoint_control_keypad_handler, NULL );

    do
    {
        ret = aws_mqtt_conn_open( app_info.mqtt_object, mqtt_connection_event_cb );
        connection_retries++ ;
    } while ( ( ret != WICED_SUCCESS ) && ( connection_retries < WICED_MQTT_CONNECTION_NUMBER_OF_RETRIES ) );

    aws_mqtt_app_publish( app_info.mqtt_object, WICED_MQTT_QOS_DELIVER_AT_LEAST_ONCE, (uint8_t*)app_info.shadow_state_topic, (uint8_t*)SHADOW_PUBLISH_MESSAGE_STR_OFF ,sizeof(SHADOW_PUBLISH_MESSAGE_STR_OFF) );

    wiced_rtos_delay_milliseconds( MQTT_DELAY_IN_MILLISECONDS * 2 );

    aws_mqtt_app_subscribe( app_info.mqtt_object, app_info.shadow_delta_topic , WICED_MQTT_QOS_DELIVER_AT_LEAST_ONCE );
    //print_System_memory();
    wiced_gpio_output_high( RGB_R );
    wiced_gpio_output_high( RGB_G );
    wiced_gpio_output_high( RGB_B );
    while ( 1 )
    {
        /* Wait forever on wake semaphore until the wake button is pressed */
        wiced_rtos_get_semaphore( &app_info.wake_semaphore, WICED_NEVER_TIMEOUT );

        /* Toggle the LED */
        if ( strcasecmp( led_status, "ON" ) == 0 )
        {
        	wiced_gpio_output_high( RGB_B );
            led_status = "OFF";
            strcpy(req_led_status, led_status);
            aws_mqtt_app_publish( app_info.mqtt_object, WICED_MQTT_QOS_DELIVER_AT_LEAST_ONCE, (uint8_t*)app_info.shadow_state_topic, (uint8_t*)SHADOW_PUBLISH_MESSAGE_STR_OFF_DESIRED_AND_REPORTED ,sizeof(SHADOW_PUBLISH_MESSAGE_STR_OFF_DESIRED_AND_REPORTED) );
        }
        else
        {
        	wiced_gpio_output_low( RGB_B );
            led_status = "ON";
            strcpy(req_led_status, led_status);
            aws_mqtt_app_publish( app_info.mqtt_object, WICED_MQTT_QOS_DELIVER_AT_LEAST_ONCE, (uint8_t*)app_info.shadow_state_topic, (uint8_t*)SHADOW_PUBLISH_MESSAGE_STR_ON_DESIRED_AND_REPORTED ,sizeof(SHADOW_PUBLISH_MESSAGE_STR_ON_DESIRED_AND_REPORTED) );
        }
    }

    aws_mqtt_app_unsubscribe( app_info.mqtt_object, app_info.shadow_delta_topic );

    aws_mqtt_conn_close( app_info.mqtt_object );

    wiced_rtos_deinit_semaphore( &app_info.msg_semaphore );
    ret = wiced_mqtt_deinit( app_info.mqtt_object );
    free( app_info.mqtt_object );
    app_info.mqtt_object = NULL;

    return;
}
