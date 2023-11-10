#include <curl/curl.h>
#include <iostream>
#include <string>

typedef std::string string;

// static const string BASE_URL = "https://tryhackme.com/room/professorkurzsheatpump/api/";
static const string BASE_URL = "https://tryhackme.com/room/professorkurzsheatpump/";

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL *curl;
    CURLcode res;
    // string url = BASE_URL + "invoices";
    string url = BASE_URL + "";

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
    
    std::cout << "Website responded (" << body.size() << " bytes)" << std::endl;
    if (body.size() > 100) {
        std::cout << body.substr(0, 100) << "..." << std::endl;
    } else {
        std::cout << body << std::endl;
    }

    return 0;
}
