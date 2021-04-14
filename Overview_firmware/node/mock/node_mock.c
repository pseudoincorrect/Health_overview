#include "node_mock.h"

void init_mock_node_sensors(n_sensors_t* n_sensors)
{
    n_sensors->temperature = 0;
    n_sensors->heart_rate  = 0;
    n_sensors->oxymetry    = 0;
    n_sensors->humidity    = 0;
    n_sensors->dehydration = 0;
    n_sensors->reserved1   = 0;
    n_sensors->reserved2   = 0;
}

void update_mock_node_sensors(n_sensors_t* n_sensors)
{
    n_sensors->temperature = n_sensors->temperature + 1;
    n_sensors->heart_rate  = n_sensors->heart_rate + 2;
    n_sensors->oxymetry    = n_sensors->oxymetry + 3;
    n_sensors->humidity    = n_sensors->humidity + 4;
    n_sensors->dehydration = n_sensors->dehydration + 5;
    n_sensors->reserved1   = n_sensors->reserved1 + 6;
    n_sensors->reserved2   = n_sensors->reserved2 + 7;
}



void copy_mock_node_sensors(n_sensors_t* n_sensors, n_sensors_t* n_sensors_mock)
{
    n_sensors->temperature = n_sensors_mock->temperature;
    n_sensors->heart_rate  = n_sensors_mock->heart_rate;
    n_sensors->oxymetry    = n_sensors_mock->oxymetry;
    n_sensors->humidity    = n_sensors_mock->humidity;
    n_sensors->dehydration = n_sensors_mock->dehydration;
    n_sensors->reserved1   = n_sensors_mock->reserved1;
    n_sensors->reserved2   = n_sensors_mock->reserved2;
}