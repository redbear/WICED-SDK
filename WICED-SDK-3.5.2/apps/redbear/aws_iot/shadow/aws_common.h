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
 */

#include "wiced.h"
#include "mqtt_api.h"
#include "resources.h"
#include "gpio_button.h"
#include "aws_config.h"

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************
 *                      Macros
 ******************************************************/

#define AWS_IOT_HOST_NAME                   "data.iot.ap-southeast-1.amazonaws.com"//"data.iot.us-east-1.amazonaws.com"
#define MQTT_REQUEST_TIMEOUT                (5000)
#define MQTT_DELAY_IN_MILLISECONDS          (1000)
#define MQTT_MAX_RESOURCE_SIZE              (0x7fffffff)
#define THING_STATE_TOPIC_STR_BUILDER       "$aws/things/%s/shadow/update"
#define THING_DELTA_TOPIC_STR_BUILDER       "$aws/things/%s/shadow/update/delta"

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
 *                 Global Variables
 ******************************************************/

typedef struct aws_app_info_s
{
    wiced_mqtt_event_type_t   expected_event;
    wiced_semaphore_t         msg_semaphore;
    wiced_semaphore_t         wake_semaphore;
    char                      thing_name[32];
    char                      shadow_state_topic[64];
    char                      shadow_delta_topic[64];
    char                      mqtt_client_id[64];
    wiced_mqtt_object_t       mqtt_object;
} aws_app_info_t;

/******************************************************
 *               Function Declarations
 ******************************************************/
wiced_result_t aws_app_init( aws_app_info_t *app_info );

/*
 * Publish (send) WICED_MESSAGE_STR to WICED_TOPIC and wait for 5 seconds to receive a PUBCOMP (as it is QoS=2).
 */
wiced_result_t aws_mqtt_app_publish( wiced_mqtt_object_t mqtt_obj, uint8_t qos, uint8_t *topic, uint8_t *data, uint32_t data_len );

/*
 * Open a connection and wait for MQTT_REQUEST_TIMEOUT period to receive a connection open OK event
 */
wiced_result_t aws_mqtt_conn_open( wiced_mqtt_object_t mqtt_obj, wiced_mqtt_callback_t callback );

/*
 * Close a connection and wait for 5 seconds to receive a connection close OK event
 */
wiced_result_t aws_mqtt_conn_close( wiced_mqtt_object_t mqtt_obj );

/*
 * Subscribe to WICED_TOPIC and wait for 5 seconds to receive an ACM.
 */
wiced_result_t aws_mqtt_app_subscribe( wiced_mqtt_object_t mqtt_obj, char *topic, uint8_t qos );

/*
 * Unsubscribe from WICED_TOPIC and wait for 10 seconds to receive an ACM.
 */
wiced_result_t aws_mqtt_app_unsubscribe( wiced_mqtt_object_t mqtt_obj, char *topic );

wiced_result_t mqtt_connection_event_cb( wiced_mqtt_object_t mqtt_object, wiced_mqtt_event_info_t *event );

void print_System_memory( void );
#ifdef __cplusplus
} /* extern "C" */
#endif
