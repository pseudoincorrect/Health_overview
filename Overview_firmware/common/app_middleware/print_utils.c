#include "print_utils.h"

void print_thread_sensor_msg(thread_msg_t* thread_msg) 
{
    SEGGER_RTT_printf(0,"%02x%02x%02x%02x%02x%02x%02x%04x%04x%04x%04x%04x%04x%04x\n",
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

void print_pretty_sensors_thread_msg(thread_msg_t* thread_msg) 
{
    SEGGER_RTT_printf(0," \
        \rcommand     %c \n \
        \rnode Adress %02x%02x%02x%02x%02x%02x  \n \
        \rtemperature %02x  %02x \n \
        \rheart_rate   %d \n \
        \roxymetry    %d \n \
        \rhumidity    %d \n \
        \rdehydration %d \n \
        \rreserved1   %d \n \
        \rreserved2   %d \n \
        \n",
        thread_msg->type,
        thread_msg->content.sensors.addr.a[0], 
        thread_msg->content.sensors.addr.a[1], 
        thread_msg->content.sensors.addr.a[2],
        thread_msg->content.sensors.addr.a[3], 
        thread_msg->content.sensors.addr.a[4], 
        thread_msg->content.sensors.addr.a[5],
        // temperature: int_part.frac_part (8 bit each) we print only int_part
        (thread_msg->content.sensors.origin.node.temperature) >> 8, 
        (thread_msg->content.sensors.origin.node.temperature) & 0x00FF, 
        thread_msg->content.sensors.origin.node.heart_rate,
        thread_msg->content.sensors.origin.node.oxymetry,
        thread_msg->content.sensors.origin.node.humidity,
        thread_msg->content.sensors.origin.node.dehydration,
        thread_msg->content.sensors.origin.node.reserved1,
        thread_msg->content.sensors.origin.node.reserved2);
}

void print_pretty_nofif_thread_msg(thread_msg_t* thread_msg) 
{
    SEGGER_RTT_printf(0," \
        \rcommand     %c \n \
        \rnode Adress %02x%02x%02x%02x%02x%02x  \n \
        \rcontent     %c \n \
        \n",
        thread_msg->type,
        thread_msg->content.notif.addr.a[0], 
        thread_msg->content.notif.addr.a[1], 
        thread_msg->content.notif.addr.a[2],
        thread_msg->content.notif.addr.a[3], 
        thread_msg->content.notif.addr.a[4], 
        thread_msg->content.notif.addr.a[5],
        // temperature: int_part.frac_part (8 bit each) we print only int_part
        thread_msg->content.notif.data
    );
}

void print_addr(node_addr_t* node_addr) 
{
 SEGGER_RTT_printf(0,"node_addr is %02x%02x%02x%02x%02x%02x\n",
      node_addr->a[0], node_addr->a[1], node_addr->a[2],
      node_addr->a[3], node_addr->a[4], node_addr->a[5]);
}