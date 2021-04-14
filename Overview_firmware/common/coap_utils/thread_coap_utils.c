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

#include <openthread/ip6.h>
#include <openthread/link.h>
#include <openthread/thread.h>
#include <openthread/platform/alarm-milli.h>
#include "thread_coap_utils.h"
#include "app_timer.h"
#include "nrf_assert.h"
#include "nrf_log.h"
#include "sdk_config.h"
#include "thread_utils.h"
#include "SEGGER_RTT.h"

#define RESPONSE_POLL_PERIOD 100

APP_TIMER_DEF(m_provisioning_timer);
// APP_TIMER_DEF(m_led_timer);

// Function forward declaration
static void light_changed_default(thread_coap_utils_light_command_t light_command);
static uint32_t poll_period_response_set(otInstance * p_instance);
static uint32_t poll_period_restore(otInstance * p_instance);
static void thread_coap_utils_provisioning_request_handler(
        void                * p_context,
        otCoapHeader        * p_header,
        otMessage           * p_message,
        const otMessageInfo * p_message_info);
static void thread_coap_utils_cmd_request_handler(  
        void                * p_context,
        otCoapHeader        * p_header,
        otMessage           * p_message,
        const otMessageInfo * p_message_info);
static void thread_coap_utils_cmd_response_handler (
        void                * p_context,
        otCoapHeader        * p_header,
        otMessage           * p_message,
        const otMessageInfo * p_message_info,
        otError               result);

static void print_thread_msg(thread_msg_t* thread_msg);

static int fail_count_g;
static bool provisionning_enabled_g;
static bool has_received_g;

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

static void add_fail (void)
{
  fail_count_g +=1;
}

static void reset_fail (void)
{
  fail_count_g = 0;
}

int get_fail (void)
{
  return fail_count_g;
}

void enable_provisionning(bool val)
{
  if (val)
      provisionning_enabled_g = true;
  else    
      provisionning_enabled_g = false;
}

bool is_provisionning_enabled (void)
{
    return provisionning_enabled_g;
}

static void reset_has_received (void)
{ 
    has_received_g = false;
}

static void set_has_received (void)
{
   has_received_g = true;
}

bool coap_has_received (void)
{
    if (has_received_g)
    {
        has_received_g = false;
        return true;
    }
    else 
        return false;
}


//  .d8888b. 88888888888 8888888b.  888     888  .d8888b. 88888888888 
// d88P  Y88b    888     888   Y88b 888     888 d88P  Y88b    888     
// Y88b.         888     888    888 888     888 888    888    888     
//  "Y888b.      888     888   d88P 888     888 888           888     
//     "Y88b.    888     8888888P"  888     888 888           888     
//       "888    888     888 T88b   888     888 888    888    888     
// Y88b  d88P    888     888  T88b  Y88b. .d88P Y88b  d88P    888     
//  "Y8888P"     888     888   T88b  "Y88888P"   "Y8888P"     888  

static thread_msg_callback_t thread_msg_callback;

static uint32_t m_poll_period;

static const otIp6Address m_unspecified_ipv6 =
{
    .mFields =
    {
        .m8 = {0}
    }
};

static thread_coap_configuration_t m_thread_coap_configuration =
{
    .coap_server_enabled                = true,
    .coap_client_enabled                = true,
    .coap_cloud_enabled                 = false,
};

static thread_coap_utils_status_t m_coap_status =
{
    .provisioning_enabled = false,
    .provisioning_expiry  = 0,
    .led_blinking_is_on   = false,
    .peer_address         =
    {
        .mFields =
        {
            .m8 = {0}
        }
    },
    .multicast_light_on   = false,
};


static thread_coap_utils_resources_t m_coap_resources = {
    .provisioning_resource = {"provisioning", thread_coap_utils_provisioning_request_handler, NULL, NULL},
    .cmd_resource = {"cmd", thread_coap_utils_cmd_request_handler, NULL, NULL}
};

// 8888888b.  8888888888 8888888888     d8888 888     888 888    88888888888 
// 888  "Y88b 888        888           d88888 888     888 888        888     
// 888    888 888        888          d88P888 888     888 888        888     
// 888    888 8888888    8888888     d88P 888 888     888 888        888     
// 888    888 888        888        d88P  888 888     888 888        888     
// 888    888 888        888       d88P   888 888     888 888        888     
// 888  .d88P 888        888      d8888888888 Y88b. .d88P 888        888     
// 8888888P"  8888888888 888     d88P     888  "Y88888P"  88888888   888  

static void thread_coap_utils_handler_default( void * p_context, otCoapHeader * p_header,
                    otMessage * p_message, const otMessageInfo * p_message_info)
{
    (void)p_context;
    (void)p_header;
    (void)p_message;
    (void)p_message_info;

    NRF_LOG_INFO("Received CoAP message that does not match any request or resource\r\n");
}

void thread_msg_callback_init (thread_msg_callback_t callback) 
{
    thread_msg_callback = callback;
}

//  .d8888b.  888b     d888 8888888b.  
// d88P  Y88b 8888b   d8888 888  "Y88b 
// 888    888 88888b.d88888 888    888 
// 888        888Y88888P888 888    888 
// 888        888 Y888P 888 888    888 
// 888    888 888  Y8P  888 888    888 
// Y88b  d88P 888   "   888 888  .d88P 
//  "Y8888P"  888       888 8888888P"  

void thread_coap_utils_multicast_cmd_request_send( otInstance * p_instance, thread_msg_t* thread_msg)
{
    otError       error = OT_ERROR_NONE;
    otMessage   * p_message;
    otMessageInfo message_info;
    otCoapHeader  header;
    const char  * p_scope = NULL;
    do {
        otCoapHeaderInit(&header, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_PUT);
        UNUSED_VARIABLE(otCoapHeaderAppendUriPathOptions(&header, "cmd"));
        UNUSED_VARIABLE(otCoapHeaderSetPayloadMarker(&header));

        p_message = otCoapNewMessage(p_instance, &header);
        if (p_message == NULL) {
            NRF_LOG_INFO("Failed to allocate message for CoAP Request\r\n");
            break;
        }

        error = otMessageAppend(p_message, thread_msg, sizeof(thread_msg_t));
        if (error != OT_ERROR_NONE)
            break;
        
        p_scope = "FF03::1"; // Tcu_MULTICAST_REALM_LOCAL

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = OT_DEFAULT_COAP_PORT;
        UNUSED_VARIABLE(otIp6AddressFromString(p_scope, &message_info.mPeerAddr));

        error = otCoapSendRequest(p_instance, p_message, &message_info, NULL, NULL);
    } while (false);

    if (error != OT_ERROR_NONE && p_message != NULL) {
        NRF_LOG_INFO("Failed to send CoAP Request: %d\r\n", error);
        otMessageFree(p_message);
    }
    SEGGER_RTT_printf(0, "multicast sent \n\r");
}

void thread_coap_utils_unicast_cmd_request_send(otInstance * p_instance, thread_msg_t* thread_msg)
{
    otError       error = OT_ERROR_NONE;
    otMessage   * p_message;
    otMessageInfo message_info;
    otCoapHeader  header;

    do
    {
        if (otIp6IsAddressEqual(&m_coap_status.peer_address, &m_unspecified_ipv6))
        {
            NRF_LOG_INFO("Failed to send the CoAP Request to the Unspecified IPv6 Address\r\n");
            break;
        }

        otCoapHeaderInit(&header, OT_COAP_TYPE_CONFIRMABLE, OT_COAP_CODE_PUT);
        otCoapHeaderGenerateToken(&header, 2);
        UNUSED_VARIABLE(otCoapHeaderAppendUriPathOptions(&header, "cmd"));
        UNUSED_VARIABLE(otCoapHeaderSetPayloadMarker(&header));

        p_message = otCoapNewMessage(p_instance, &header);
        if (p_message == NULL)
        {
            NRF_LOG_INFO("Failed to allocate message for CoAP Request\r\n");
            break;
        }

        error = otMessageAppend(p_message, thread_msg, sizeof(thread_msg_t));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = OT_DEFAULT_COAP_PORT;
        memcpy(&message_info.mPeerAddr, &m_coap_status.peer_address, sizeof(message_info.mPeerAddr));

        error = otCoapSendRequest(p_instance,
                                  p_message,
                                  &message_info,
                                  thread_coap_utils_cmd_response_handler,
                                  p_instance);

        reset_has_received();                    
    } while (false);

    if (error != OT_ERROR_NONE && p_message != NULL)
    {
    SEGGER_RTT_printf(0, "Failed to send CoAP Request: %d\r\n", error);
        otMessageFree(p_message);
    }
}

static void thread_coap_utils_cmd_request_handler(  
        void                * p_context,
        otCoapHeader        * p_header,
        otMessage           * p_message,
        const otMessageInfo * p_message_info)
{
    thread_msg_t thread_msg;
    do {
        if (otCoapHeaderGetType(p_header) != OT_COAP_TYPE_CONFIRMABLE &&
            otCoapHeaderGetType(p_header) != OT_COAP_TYPE_NON_CONFIRMABLE &&
            otCoapHeaderGetCode(p_header) != OT_COAP_CODE_PUT )
        {
            break;
        }

        if (otMessageRead(p_message, otMessageGetOffset(p_message), &thread_msg, sizeof(thread_msg_t)) != sizeof(thread_msg_t))
        {
            NRF_LOG_INFO("cmd handler - wrong reception\r\n");
        }

        if (otCoapHeaderGetType(p_header) == OT_COAP_TYPE_CONFIRMABLE)
        {
            thread_coap_utils_cmd_response_send(p_context, p_header, p_message_info);
        }

        thread_msg_callback(&thread_msg);
    } while (false);
}

void thread_coap_utils_cmd_response_send(   
        void                * p_context,
        otCoapHeader        * p_request_header,
        const otMessageInfo * p_message_info)
{
    otError      error = OT_ERROR_NO_BUFS;
    otCoapHeader header;
    otMessage  * p_response;
    do
    {
    otCoapHeaderInit(&header, OT_COAP_TYPE_ACKNOWLEDGMENT, OT_COAP_CODE_CHANGED);
    otCoapHeaderSetMessageId(&header, otCoapHeaderGetMessageId(p_request_header));
    otCoapHeaderSetToken(&header,
                            otCoapHeaderGetToken(p_request_header),
                            otCoapHeaderGetTokenLength(p_request_header));

    p_response = otCoapNewMessage(p_context, &header);
    if (p_response == NULL)
        break;

    error = otCoapSendResponse(p_context, p_response, p_message_info);

    } while (false);

    if ((error != OT_ERROR_NONE) && (p_response != NULL)) {
        otMessageFree(p_response);
    }
}

static void thread_coap_utils_cmd_response_handler (
                                          void                * p_context,
                                          otCoapHeader        * p_header,
                                          otMessage           * p_message,
                                          const otMessageInfo * p_message_info,
                                          otError               result)
{
    (void)p_context;
    (void)p_header;

    // restore the polling period back to initial slow value
    UNUSED_VARIABLE(poll_period_restore(thread_ot_instance_get()));

    if (result == OT_ERROR_NONE)
    {
       SEGGER_RTT_printf(0,"thread msg sent SUCCESS \n");
        set_has_received();             
        UNUSED_VARIABLE(otMessageRead(p_message,
                                      otMessageGetOffset(p_message),
                                      &m_coap_status.peer_address,
                                      sizeof(m_coap_status.peer_address))
        );
    }
    else
    {
        SEGGER_RTT_printf(0,"thread_coap_utils_cmd_response_handler FAIL %d \r\n", result);
        reset_has_received();   
    }
}

// 8888888b.  8888888b.   .d88888b.  888     888 
// 888   Y88b 888   Y88b d88P" "Y88b 888     888 
// 888    888 888    888 888     888 888     888 
// 888   d88P 888   d88P 888     888 Y88b   d88P 
// 8888888P"  8888888P"  888     888  Y88b d88P  
// 888        888 T88b   888     888   Y88o88P   
// 888        888  T88b  Y88b. .d88P    Y888P    
// 888        888   T88b  "Y88888P"      Y8P     
                                              
static void thread_coap_utils_provisioning_request_handler(void                * p_context,
                                                           otCoapHeader        * p_header,
                                                           otMessage           * p_message,
                                                           const otMessageInfo * p_message_info)
{
    (void)p_message;
    otMessageInfo message_info;
 
  
    SEGGER_RTT_printf(0,"thread_coap_utils_provisioning_request_handler \n");

    if (!is_provisionning_enabled())
        return;

//    if (!thread_coap_utils_provisioning_is_enabled())
//    {
//        return;
//    }

    if ((otCoapHeaderGetType(p_header) == OT_COAP_TYPE_NON_CONFIRMABLE) &&
        (otCoapHeaderGetCode(p_header) == OT_COAP_CODE_GET))
    {
        message_info = *p_message_info;
        memset(&message_info.mSockAddr, 0, sizeof(message_info.mSockAddr));
        if (thread_coap_utils_provisioning_response_send(
                p_context, p_header, &message_info) == OT_ERROR_NONE)
        {
                thread_coap_utils_provisioning_enable(false);
        }
    }
}

static void provisioning_response_handler(void                * p_context,
                                          otCoapHeader        * p_header,
                                          otMessage           * p_message,
                                          const otMessageInfo * p_message_info,
                                          otError               result)
{
    (void)p_context;
    (void)p_header;

//    SEGGER_RTT_printf(0,"provisioning_response_handler \n");

    // restore the polling period back to initial slow value
    UNUSED_VARIABLE(poll_period_restore(thread_ot_instance_get()));

    if (result == OT_ERROR_NONE)
    {
        // SEGGER_RTT_printf(0,"provisioning_response_handler SUCCESS\n");
        UNUSED_VARIABLE(otMessageRead(p_message,
                                      otMessageGetOffset(p_message),
                                      &m_coap_status.peer_address,
                                      sizeof(m_coap_status.peer_address))
        );
    }
    else
    {
        SEGGER_RTT_printf(0,"provisioning_response_handler FAIL %d\r\n", result);
    }
}

void thread_coap_utils_provisioning_request_send(otInstance * p_instance)
{
    otError       error = OT_ERROR_NO_BUFS;
    otCoapHeader  header;
    otMessage   * p_request;
    otMessageInfo message_info;

    SEGGER_RTT_printf(0,"thread_coap_utils_provisioning_request_send \n");

    do
    {
        otCoapHeaderInit(&header, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_GET);
        otCoapHeaderGenerateToken(&header, 2);
        UNUSED_VARIABLE(otCoapHeaderAppendUriPathOptions(&header, "provisioning"));

        p_request = otCoapNewMessage(p_instance, &header);
        if (p_request == NULL)
        {
            break;
        }

        // decrease the polling period for higher responsiveness
        uint32_t err_code = poll_period_response_set(p_instance);
        if (err_code == NRF_ERROR_BUSY)
        {
            break;
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort    = OT_DEFAULT_COAP_PORT;
        UNUSED_VARIABLE(otIp6AddressFromString("FF03::1", &message_info.mPeerAddr));

        error = otCoapSendRequest(
            p_instance, p_request, &message_info, provisioning_response_handler, p_instance);

    } while (false);

    if (error != OT_ERROR_NONE && p_request != NULL)
    {
        otMessageFree(p_request);
    }

}

// 8888888b.   .d88888b.  888      888      
// 888   Y88b d88P" "Y88b 888      888      
// 888    888 888     888 888      888      
// 888   d88P 888     888 888      888      
// 8888888P"  888     888 888      888      
// 888        888     888 888      888      
// 888        Y88b. .d88P 888      888      
// 888         "Y88888P"  88888888 88888888 

static uint32_t poll_period_response_set(otInstance * p_instance)
{
    uint32_t error;

    do
    {
        if (otThreadGetLinkMode(p_instance).mRxOnWhenIdle)
        {
            error = NRF_ERROR_INVALID_STATE;
            break;
        }

        if (!m_poll_period)
        {
            m_poll_period = otLinkGetPollPeriod(p_instance);
            NRF_LOG_INFO("Poll Period: %dms set\r\n", RESPONSE_POLL_PERIOD);
            otLinkSetPollPeriod(p_instance, RESPONSE_POLL_PERIOD);

            error =  NRF_SUCCESS;
        }
        else
        {
            error = NRF_ERROR_BUSY;
        }
    } while (false);

    return error;
}


static uint32_t poll_period_restore(otInstance * p_instance)
{
    uint32_t error;

    do
    {
        if (otThreadGetLinkMode(p_instance).mRxOnWhenIdle)
        {
            error = NRF_ERROR_INVALID_STATE;
            break;
        }

        if (m_poll_period)
        {
            otLinkSetPollPeriod(p_instance, m_poll_period);
            NRF_LOG_INFO("Poll Period: %dms restored\r\n", m_poll_period);
            m_poll_period = 0;

            error =  NRF_SUCCESS;
        }
        else
        {
            error = NRF_ERROR_BUSY;
        }
    } while (false);

    return error;
}

// 8888888 888b    888 8888888 88888888888 
//   888   8888b   888   888       888     
//   888   88888b  888   888       888     
//   888   888Y88b 888   888       888     
//   888   888 Y88b888   888       888     
//   888   888  Y88888   888       888     
//   888   888   Y8888   888       888     
// 8888888 888    Y888 8888888     888     

thread_coap_configuration_t * thread_coap_configuration_get()
{
    return &m_thread_coap_configuration;
}

static void thread_coap_utils_configuration_set(const thread_coap_configuration_t * p_thread_coap_configuration)
{
    m_thread_coap_configuration = *p_thread_coap_configuration;
}

void thread_coap_utils_init(const thread_coap_configuration_t * p_thread_coap_configuration)
{
    otError error = otCoapStart(thread_ot_instance_get(), OT_DEFAULT_COAP_PORT);
    ASSERT(error == OT_ERROR_NONE);

    otCoapSetDefaultHandler(thread_ot_instance_get(), thread_coap_utils_handler_default, NULL);

    thread_coap_utils_configuration_set(p_thread_coap_configuration);

    if (m_thread_coap_configuration.coap_server_enabled)
    {
        m_coap_resources.provisioning_resource.mContext = thread_ot_instance_get();
        m_coap_resources.cmd_resource.mContext = thread_ot_instance_get();

        error = otCoapAddResource(thread_ot_instance_get(), &m_coap_resources.cmd_resource);
        ASSERT(error == OT_ERROR_NONE);

        error = otCoapAddResource(thread_ot_instance_get(), &m_coap_resources.provisioning_resource);
        ASSERT(error == OT_ERROR_NONE);
    }
}

void thread_coap_utils_deinit(void)
{
    otError error = otCoapStop(thread_ot_instance_get());
    ASSERT(error == OT_ERROR_NONE);

    otCoapSetDefaultHandler(thread_ot_instance_get(), thread_coap_utils_handler_default, NULL);

    if (m_thread_coap_configuration.coap_server_enabled)
    {
        m_coap_resources.cmd_resource.mContext = NULL;
        m_coap_resources.provisioning_resource.mContext = NULL;

        otCoapRemoveResource(thread_ot_instance_get(), &m_coap_resources.cmd_resource);
        otCoapRemoveResource(thread_ot_instance_get(), &m_coap_resources.provisioning_resource);
    }
}

//        d8888 8888888b.  8888888b.  8888888b.  
//       d88888 888  "Y88b 888  "Y88b 888   Y88b 
//      d88P888 888    888 888    888 888    888 
//     d88P 888 888    888 888    888 888   d88P 
//    d88P  888 888    888 888    888 8888888P"  
//   d88P   888 888    888 888    888 888 T88b   
//  d8888888888 888  .d88P 888  .d88P 888  T88b  
// d88P     888 8888888P"  8888888P"  888   T88b 
                                              
void thread_coap_utils_peer_addr_set(const otIp6Address * p_peer_addr)
{
    m_coap_status.peer_address = *p_peer_addr;
}

otIp6Address * thread_coap_utils_peer_addr_get(void)
{
    return &m_coap_status.peer_address;
}

void thread_coap_utils_peer_addr_clear(void)
{
    m_coap_status.peer_address = m_unspecified_ipv6;
}

bool thread_coap_utils_peer_addr_is_set(void)
{
    return (!otIp6IsAddressEqual(&m_coap_status.peer_address, &m_unspecified_ipv6));
}

//  .d8888b.  888       .d88888b.  888     888 8888888b.  
// d88P  Y88b 888      d88P" "Y88b 888     888 888  "Y88b 
// 888    888 888      888     888 888     888 888    888 
// 888        888      888     888 888     888 888    888 
// 888        888      888     888 888     888 888    888 
// 888    888 888      888     888 888     888 888    888 
// Y88b  d88P 888      Y88b. .d88P Y88b. .d88P 888  .d88P 
//  "Y8888P"  88888888  "Y88888P"   "Y88888P"  8888888P"  

static void cloud_data_send(otInstance                            * p_instance,
                            const thread_coap_cloud_information_t * p_thread_coap_cloud_information,
                            char                                  * p_payload)
{
    otError       error;
    otCoapHeader  header;
    otCoapOption  content_format_option;
    otMessage   * p_request;
    otMessageInfo message_info;
    uint8_t content_format = p_thread_coap_cloud_information->cloud_coap_content_format;
    do
    {
        content_format_option.mNumber = OT_COAP_OPTION_CONTENT_FORMAT;
        content_format_option.mLength = 1;
        content_format_option.mValue  = &content_format;

        otCoapHeaderInit(&header, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_POST);
        UNUSED_VARIABLE(otCoapHeaderAppendUriPathOptions(&header, p_thread_coap_cloud_information->p_cloud_uri_path));
        UNUSED_VARIABLE(otCoapHeaderAppendOption(&header, &content_format_option));
        UNUSED_VARIABLE(otCoapHeaderSetPayloadMarker(&header));

        p_request = otCoapNewMessage(p_instance, &header);
        if (p_request == NULL)
        {
            NRF_LOG_INFO("Failed to allocate message for CoAP Request\r\n");
            break;
        }

        error = otMessageAppend(p_request, p_payload, strlen(p_payload));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        memset(&message_info, 0, sizeof(message_info));
        message_info.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        message_info.mPeerPort = OT_DEFAULT_COAP_PORT;
        message_info.mPeerAddr = m_coap_status.peer_address;

        error = otCoapSendRequest(p_instance, p_request, &message_info, NULL, NULL);

    } while (false);

    if (error != OT_ERROR_NONE && p_request != NULL)
    {
        NRF_LOG_INFO("Failed to send CoAP Request: %d\r\n", error);
        otMessageFree(p_request);
    }
}

void thread_coap_utils_cloud_data_update(
                        const thread_coap_cloud_information_t * p_thread_coap_cloud_information,
                        uint16_t                                data)
{
    char payload_buffer[64];

    sprintf(payload_buffer,
            "{\"values\":[{\"key\":\"%s\",\"value\":\"%d\"}]}",
            p_thread_coap_cloud_information->p_cloud_thing_resource, data);

    cloud_data_send(thread_ot_instance_get(), p_thread_coap_cloud_information, payload_buffer);
}

bool thread_coap_utils_provisioning_is_enabled(void)
{
    return m_coap_status.provisioning_enabled;
}

void thread_coap_utils_provisioning_enable(bool value)
{
    uint32_t error;

    m_coap_status.provisioning_enabled = value;

    if (value)
    {
        m_coap_status.provisioning_expiry = otPlatAlarmMilliGetNow() + PROVISIONING_EXPIRY_TIME;
        error = app_timer_start(m_provisioning_timer,
                                APP_TIMER_TICKS(PROVISIONING_EXPIRY_TIME),
                                NULL);
        ASSERT(error == NRF_SUCCESS);

        // error = app_timer_start(m_led_timer, APP_TIMER_TICKS(LED_INTERVAL), NULL);
        // ASSERT(error == NRF_SUCCESS);

        NRF_LOG_INFO("Provisionning enabled");
    }
    else
    {
        m_coap_status.provisioning_expiry = 0;

        error = app_timer_stop(m_provisioning_timer);
        ASSERT(error == NRF_SUCCESS);

//        error = app_timer_stop(m_led_timer);
//        ASSERT(error == NRF_SUCCESS);

         NRF_LOG_INFO("Provisionning disabled");
    }
}

// 88888888888 8888888 888b     d888 8888888888 8888888b.  
//     888       888   8888b   d8888 888        888   Y88b 
//     888       888   88888b.d88888 888        888    888 
//     888       888   888Y88888P888 8888888    888   d88P 
//     888       888   888 Y888P 888 888        8888888P"  
//     888       888   888  Y8P  888 888        888 T88b   
//     888       888   888   "   888 888        888  T88b  
//     888     8888888 888       888 8888888888 888   T88b 

static void thread_coap_utils_provisioning_timer_handler(void * p_context)
{
    thread_coap_utils_provisioning_enable(false);
}

void thread_coap_utils_provisioning_timer_init(void)
{
    uint32_t error = app_timer_create(&m_provisioning_timer,
                                      APP_TIMER_MODE_SINGLE_SHOT,
                                      thread_coap_utils_provisioning_timer_handler);
    ASSERT(error == NRF_SUCCESS);
}

otError thread_coap_utils_provisioning_response_send(void                * p_context,
                                                     otCoapHeader        * p_request_header,
                                                     const otMessageInfo * p_message_info)
{
    otError      error = OT_ERROR_NO_BUFS;
    otCoapHeader header;
    otMessage  * p_response;
    do
    {
        otCoapHeaderInit(&header, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_CONTENT);
        otCoapHeaderSetToken(&header,
                             otCoapHeaderGetToken(p_request_header),
                             otCoapHeaderGetTokenLength(p_request_header));
        UNUSED_VARIABLE(otCoapHeaderSetPayloadMarker(&header));

        p_response = otCoapNewMessage(thread_ot_instance_get(), &header);
        if (p_response == NULL)
        {
            break;
        }

        error = otMessageAppend(
            p_response, otThreadGetMeshLocalEid(thread_ot_instance_get()), sizeof(otIp6Address));
        if (error != OT_ERROR_NONE)
        {
            break;
        }

        error = otCoapSendResponse(thread_ot_instance_get(), p_response, p_message_info);

    } while (false);

    if (error != OT_ERROR_NONE && p_response != NULL)
    {
        otMessageFree(p_response);
    }

    return error;
}

//  888b     d888 8888888  .d8888b.   .d8888b.  
//  8888b   d8888   888   d88P  Y88b d88P  Y88b 
//  88888b.d88888   888   Y88b.      888    888 
//  888Y88888P888   888    "Y888b.   888        
//  888 Y888P 888   888       "Y88b. 888        
//  888  Y8P  888   888         "888 888    888 
//  888   "   888   888   Y88b  d88P Y88b  d88P 
//  888       888 8888888  "Y8888P"   "Y8888P"  

static void print_thread_msg(thread_msg_t* thread_msg) {
    SEGGER_RTT_printf(0,"%02x%02x%02x%02x%02x%02x%02x%04x%04x%04x%04x%04x%04x%04x\n\r",
        thread_msg->type,
        thread_msg->content.sensors.addr.a[0], 
        thread_msg->content.sensors.addr.a[1], 
        thread_msg->content.sensors.addr.a[2],
        thread_msg->content.sensors.addr.a[3], 
        thread_msg->content.sensors.addr.a[4], 
        thread_msg->content.sensors.addr.a[5],
        thread_msg->content.sensors.origin.node.temperature,
        thread_msg->content.sensors.origin.node.heart_rate,
        thread_msg->content.sensors.origin.node.oxymetry,
        thread_msg->content.sensors.origin.node.humidity,
        thread_msg->content.sensors.origin.node.dehydration,
        thread_msg->content.sensors.origin.node.reserved1,
        thread_msg->content.sensors.origin.node.reserved2);
}