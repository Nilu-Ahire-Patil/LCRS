#include <iostream>
#include <sstream>
#include <string>

#define SPDLOG_ACTIVE_LEVEL 0

#include "lcrs/logger.h"

// int main() {
//     listMacAddresses();
//     return 0;
// }

int main() {

    Logger::init("lcrsTest");
    Logger::setLevel(spdlog::level::trace);

    SPDLOG_TRACE("this is trace message");
    SPDLOG_ERROR("this is error message");

    SPDLOG_INFO("info");
   
    lcrsLog(INFO, "thsi is trace message {}", 3);
    lcrsLog(INFO, "thsi is trace message");

    return 0;
}