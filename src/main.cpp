#include <curl/curl.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <fstream>

#include "controller.hpp"
#include "api_service.hpp"
#include "heatpump_control.hpp"
#include "urls.hpp"

typedef std::string string;

struct program_args
{
    string mac;
    string api_key_path;
};

program_args validate_args(int argc, const char* argv[]) {
    if (argc != 3) {
        throw std::runtime_error("Invalid number of arguments.\nSyntax: ./heat-pump <heatpump mac> <api key file path>");
    }
    
    program_args args;
    args.mac = argv[1];
    args.api_key_path = argv[2];

    return args;
}

string read_file(const string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    return string(
        std::istreambuf_iterator<char>(file), 
        std::istreambuf_iterator<char>()
    );
}

int main(int argc, const char* argv[]) {
    try
    {
        program_args args = validate_args(argc, argv);
        string api_key = read_file(args.api_key_path);
        api_service api_service(api_key);
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
