#if !defined(__MSG_TYPE_H__)
#define __MSG_TYPE_H__

#include "stdint.h"
#include "stdbool.h"

#define ADDR_LEN        6

typedef uint8_t notif_t;
typedef uint8_t msg_type_t;

enum msg_type {
    G_N_NOTIF = 1, 
    N_G_NOTIF = 2, 
    N_G_SENSORS = 3, 
    R_G_SENSORS = 4, 
    G_R_SETUP = 5, 
    R_G_SETUP = 6,
};

enum notif_type {
    NODE_SETUP = 1
};

typedef struct {
    uint8_t a[ADDR_LEN]; 
} node_addr_t;

typedef struct {
    uint16_t temperature;
    uint16_t heart_rate;
    uint16_t oxymetry;
    uint16_t humidity;
    uint16_t dehydration;
    uint16_t reserved1;
    uint16_t reserved2;
} n_sensors_t;

typedef struct {
    uint32_t data;
} r_sensors_t;

typedef union {
    n_sensors_t node;
    r_sensors_t router;
} origin_t;

typedef struct {
    node_addr_t addr;
    origin_t origin;
} sensors_t;

typedef struct {
    node_addr_t addr;
    notif_t data;
} notif_msg_t;

// unify msg size
typedef union {
    sensors_t sensors;
    notif_msg_t notif;
} msg_data_t;

typedef struct {
    msg_type_t type;
    msg_data_t content;
} thread_msg_t;

#endif // __MSG_TYPE_H__
