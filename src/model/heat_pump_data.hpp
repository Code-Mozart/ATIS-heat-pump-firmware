#ifndef HEAT_PUMP_DATA_HPP
#define HEAT_PUMP_DATA_HPP

struct heat_pump_data
{
    bool is_on;

    float target_temperature;
    float actual_temperature;

    float target_pressure;
    float actual_pressure;
    
    float power_consumption;
};

#endif // HEAT_PUMP_DATA_HPP
