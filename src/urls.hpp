#ifndef URLS_HPP
#define URLS_HPP

#include <string>

typedef std::string string;

static const string BASE_URL = "https://tryhackme.com/room/manage/hackingheatpumps/api/";

// GET /heatpumps/{mac}
static const string GET_HEATPUMP_URL = BASE_URL + "heatpumps/%s";

#endif // URLS_HPP
