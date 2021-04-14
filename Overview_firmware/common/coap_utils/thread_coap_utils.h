/**
 * Copyright (c) 2017 - 2018, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef _thread_coap_utils_H_
#define _thread_coap_utils_H_

#include <stdbool.h>
#include <openthread/coap.h>

#include "thread_utils.h"
#include "command_def.h"
#include "SEGGER_RTT.h"

#define PROVISIONING_EXPIRY_TIME 20000
#define LED_INTERVAL             100

typedef void (*thread_msg_callback_t)(thread_msg_t * threadMsg);


// Enumeration describing light commands.
typedef enum
{
    LIGHT_OFF = '0',
    LIGHT_ON,
    LIGHT_TOGGLE
} thread_coap_utils_light_command_t;


// Type definition of the function used to handle light resource change.
typedef void (*light_changed_handler_t)(thread_coap_utils_light_command_t light_state);

typedef struct
{
    bool coap_server_enabled;
    bool coap_client_enabled;
    bool coap_cloud_enabled;
} thread_coap_configuration_t;

// Structure holding CoAP cloud server information.
typedef struct
{
    const char * p_cloud_hostname;
    const char * p_cloud_uri_path;
    const char * p_cloud_thing_resource;
    uint8_t      cloud_coap_content_format;
} thread_coap_cloud_information_t;

// Structure holding CoAP status information.
typedef struct
{
    bool         provisioning_enabled; // Information if provisioning is enabled.
    uint32_t     provisioning_expiry;  // Provisioning timeout time.
    bool         led_blinking_is_on;   // Information if leds are blinking

    otIp6Address peer_address;       // An address of a related server node.
    bool         multicast_light_on; // Information which multicast command should be sent next.
} thread_coap_utils_status_t;

// Structure holding CoAP resources.
typedef struct
{
    otCoapResource provisioning_resource; // CoAP provisioning resource.
    otCoapResource cmd_resource;        // CoAP cmd resource.
} thread_coap_utils_resources_t;

// Scope of multicast request.
typedef enum
{
    thread_coap_utils_MULTICAST_LINK_LOCAL,
    thread_coap_utils_MULTICAST_REALM_LOCAL,
} thread_coap_utils_multicast_scope_t;




void enable_provisionning(bool val);
bool coap_has_received (void);

void thread_msg_callback_init (thread_msg_callback_t callback);


// Function that starts the CoAP with specified resources.
// @details The thread_init() function needs to be executed before calling this function.
// @param[in] p_thread_coap_configuration A pointer to the Thread CoAP configuration structure.
void thread_coap_utils_init(const thread_coap_configuration_t * p_thread_coap_configuration);

// Function that stops the CoAP.
void thread_coap_utils_deinit(void);

// Function that initializes the led timer.
void thread_coap_utils_led_timer_init(void);

// Function that returns Thread CoAP configuration structure instance pointer.
// @return A pointer to the Thread CoAP configuration structure.

thread_coap_configuration_t * thread_coap_configuration_get(void);

// Function that initializes the provisioning timer.
void thread_coap_utils_provisioning_timer_init(void);

// Function that indicates if device enabled provisioning mode.
// @return True if provisioning mode is enabled, false otherwise.
bool thread_coap_utils_provisioning_is_enabled(void);

// Function that sets the information if provisioning has been enabled.
// @param value A provisioning enabled value to be set.
void thread_coap_utils_provisioning_enable(bool value);

// Function that sends the provisioning response.
// @param[in] p_context        A pointer to the application-specific context.
// @param[in] p_request_header A pointer to the OpenThread Coap Header structure.
// @param[in] p_message_info   A pointer to the constant OpenThread Message Info structure.
// @return                     An OpenThread error value. OT_ERROR_NONE in case of success.
otError thread_coap_utils_provisioning_response_send(void                * p_context,
                                                     otCoapHeader        * p_request_header,
                                                     const otMessageInfo * p_message_info);

// Function that sets the CoAP peer IPv6 address.
// @param[in] p_peer_addr A const pointer to the otIp6Address.
void thread_coap_utils_peer_addr_set(const otIp6Address * p_peer_addr);

// Function that returns the current CoAP peer IPv6 address.
// @return A pointer to the otIp6Address structure.
otIp6Address * thread_coap_utils_peer_addr_get(void);

// Function that removes the peer address.
void thread_coap_utils_peer_addr_clear(void);

// Function that checks if peer address is checked.
// @return false in case the peer address is unspecified (::), true otherwise.
bool thread_coap_utils_peer_addr_is_set(void);

// Function that sends the provisioning request to the CoAP server.
// @param[in] otInstance OpenThread instance structure pointer.
void thread_coap_utils_provisioning_request_send(otInstance * p_instance);

// send a threadMsg to all
void thread_coap_utils_multicast_cmd_request_send(otInstance * p_instance, thread_msg_t* threadMsg);

// send a threadMsg to server
void thread_coap_utils_unicast_cmd_request_send(otInstance * p_instance, thread_msg_t* threadMsg);

//section CoAP Cloud function proptypes


// Function that sends the new data to the cloud CoAP server.
// @param[in] p_thread_coap_cloud_information : 
//       A thread_coap_cloud_information_t instance structure pointer.
// @param[in] data : A uint16_t value to be sent to the server.
void thread_coap_utils_cloud_data_update(
                                const thread_coap_cloud_information_t * p_thread_coap_cloud_information,
                                uint16_t                                data);
void thread_coap_utils_cmd_response_send(   
        void                * p_context,
        otCoapHeader        * p_request_header,
        const otMessageInfo * p_message_info);

#endif /* thread_coap_utils_H */
