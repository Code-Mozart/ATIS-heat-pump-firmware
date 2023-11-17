#include <string>
#include <iostream>

#include "heatpump_control.hpp"

typedef std::string string;

heat_pump_control::heat_pump_control()
{
    // initialize heatpump control
}

heat_pump_control::~heat_pump_control()
{
    // deinitialize heatpump control
}

void heat_pump_control::set_desired_temperature(float temperature)
{
    std::cout << "Setting desired temperature to " << temperature << std::endl;
    // set desired temperature
}

void heat_pump_control::set_desired_humidity(float humidity)
{
    std::cout << "Setting desired humidity to " << humidity << std::endl;
    // set desired humidity
}
