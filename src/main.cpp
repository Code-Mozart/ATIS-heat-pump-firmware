#include <curl/curl.h>
#include <iostream>
#include <string>

#include "urls.hpp"

typedef std::string string;

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string get_response(const string& url) {
    CURL *curl;
    CURLcode res;
    string body;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    return body;
}

void print_response(const string& response) {
    std::cout << "Website responded (" << response.size() << " bytes)" << std::endl;
    if (response.size() > 100) {
        std::cout << response.substr(0, 100) << "..." << std::endl;
    } else {
        std::cout << response << std::endl;
    }
}

int main(const char* argv[], int argc) {
    // TODO: Check syntax
    // Syntax: ./heat-pump <heatpump uuid> <api key>

    string url = BASE_URL + "";
    string response = get_response(url);

    print_response(response);
    return 0;
}
