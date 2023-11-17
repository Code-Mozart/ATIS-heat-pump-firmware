#include "controller.hpp"

#include <iostream>
#include <chrono>
#include <thread>

#include "urls.hpp"

controller::controller(const string& mac, api_service& api, heat_pump_control& control)
    : mac(mac), api(api), control(control)
{
}

controller::~controller()
{
}

void controller::run()
{
    while (true)
    {
        try_update();
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

void controller::try_update()
{
    float desired_temperature;
    float desired_humidity;

    try
    {
        size_t len = GET_HEATPUMP_URL.length() + mac.length() + 1;
        string url(len, '\0');
        snprintf((char*)url.c_str(), len, GET_HEATPUMP_URL.c_str(), mac.c_str());

        json heatpump = api.GET(url);
        desired_temperature = heatpump["desired_temperature"];
        desired_humidity = heatpump["desired_humidity"];
    }
    catch(const std::exception& e)
    {
        std::cerr << "Failed to update heatpump: Network error" << std::endl;
        return;
    }

    try
    {
        control.set_desired_temperature(desired_temperature);
        control.set_desired_humidity(desired_humidity);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Failed to update heatpump: " << e.what() << std::endl;
    }
}
