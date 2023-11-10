#ifndef URLS_HPP
#define URLS_HPP

#include <string>

typedef std::string string;

static const string BASE_URL = "https://tryhackme.com/room/manage/hackingheatpumps";

// GET /heat-pumps/:uuid
static const string GET_HEATPUMP_URL = BASE_URL + "heat-pumps/%s";

// POST /heat-pumps/:uuid/data
static const string POST_HEATPUMP_DATA_URL = BASE_URL + "heat-pumps/%s/data";

// GET /heat-pumps/:uuid/rules
static const string GET_HEATPUMP_RULES_URL = BASE_URL + "heat-pumps/%s/rules";

#endif // URLS_HPP
