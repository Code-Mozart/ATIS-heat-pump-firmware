#ifndef HEAT_PUMP_CONTROL_HPP
#define HEAT_PUMP_CONTROL_HPP

class heat_pump_control
{
public:
    heat_pump_control();
    ~heat_pump_control();

    void set_desired_temperature(float temperature);
    void set_desired_humidity(float humidity);
};

#endif // HEAT_PUMP_CONTROL_HPP
