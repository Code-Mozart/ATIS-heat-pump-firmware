#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "api_service.hpp"
#include "heatpump_control.hpp"

class controller
{
public:
    controller(const string& mac, api_service& api, heat_pump_control& control);
    ~controller();

    void run();
private:
    void try_update();
private:
    const string mac;
    api_service api;
    heat_pump_control control;
};

#endif // CONTROLLER_HPP
