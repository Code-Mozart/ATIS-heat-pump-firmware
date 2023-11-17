#include <string>
#include <iostream>
#include <curl/curl.h>

#include "api_service.hpp"

typedef std::string string;

api_service::api_service(const string& api_token) :
    api_token(api_token)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

api_service::~api_service()
{
    curl_global_cleanup();
}

const json api_service::GET(const string& url)
{
    CURL* curl = curl_easy_init();
    if (curl)
    {
        string response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, api_service::write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + api_token).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        return json::parse(response);
    }
    else
    {
        std::cerr << "Network error" << std::endl;
        return json::object();
    }
}

size_t api_service::write_callback(char* contents, size_t size, size_t nmemb, void* userdata)
{
    ((string*)userdata)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
