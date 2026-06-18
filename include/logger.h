#pragma once

#include <string>

void logprintf(const char* file, const char* func, int line, const std::string& t_text);

#define JUBJUB_LOG(text) \
do { \
    logprintf(__FILE__, __FUNCTION__, __LINE__, text); \
}while(0)

