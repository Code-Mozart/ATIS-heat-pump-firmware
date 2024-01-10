#ifndef HEAT_PUMP_HPP
#define HEAT_PUMP_HPP

#include <string>

#include "customer.hpp"

typedef std::string string;

struct heatpump
{
    string ip;
    string mac;
    ::customer customer;

    float desired_temperature;
    float desired_humidity;
};

#endif // HEAT_PUMP_HPP
