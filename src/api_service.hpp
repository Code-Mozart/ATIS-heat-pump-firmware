#ifndef API_SERVICE_HPP
#define API_SERVICE_HPP

#include <json.hpp>

#include "model/heatpump.hpp"

typedef std::string string;
using json = nlohmann::json;

class api_service
{
public:
    api_service(const string& api_token);
    ~api_service();

    const json GET(const string& url);
private:
    static size_t write_callback(char* contents, size_t size, size_t nmemb, void* userdata);
private:
    const string api_token;
};

#endif // API_SERVICE_HPP
