#include "logger.h"
#include <iostream>
#include <sys/time.h>

#define TIMESTAMP_PREFIX_FORMAT "%u.%03u: "
#define TIMESTAMP_PREFIX_MAX_CHARS 20

void logprintf(const char* file, const char* func, int line, const std::string& t_text)
{

    struct timeval t;
    gettimeofday(&t, NULL);
    
    std::string printStr;

    printStr += std::to_string((unsigned int)t.tv_sec);
    printStr += ".";
    printStr += std::to_string((unsigned int)t.tv_usec / 1000);

    printStr += std::string(file);

    printStr += std::string(func);

    printStr += std::to_string(line);

    printStr += t_text;

    std::cerr << printStr << std::endl; 

}