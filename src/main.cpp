#include <curl/curl.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <thread>
#include <chrono>

#include "controller.hpp"
#include "api_service.hpp"
#include "heatpump_control.hpp"
#include "urls.hpp"

typedef std::string string;

struct program_args
{
    string mac;
    string api_key;
};

program_args validate_args(int argc, const char* argv[]) {
    if (argc != 3) {
        throw std::runtime_error("Invalid number of arguments.\nSyntax: ./heat-pump <heatpump mac> <api key>");
    }
    
    program_args args;
    args.mac = argv[1];
    args.api_key = argv[2];

    return args;
}

int main(int argc, const char* argv[]) {
    try
    {
        program_args args = validate_args(argc, argv);
        api_service api_service(args.api_key);
        heat_pump_control heat_pump_control;

        controller controller(args.mac, api_service, heat_pump_control);
        controller.run();

        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    catch(...)
    {
        std::cerr << "Unknown error" << std::endl;
        return 1;
    }
}
