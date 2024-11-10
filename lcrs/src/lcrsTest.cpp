#include <iostream>
#include <sstream>
#include <string>

#include "lcrs/logger.h" // Always include first

#include "lcrs/system.h"

class Test : public System
{
public:
    static int system()
    {
        return System::getMacAddressMap().empty() ? EXIT_FAILURE : EXIT_SUCCESS;
    }

    static int logger()
    {
        SPDLOG_TRACE("this is trace message");
        SPDLOG_ERROR("this is error message");

        SPDLOG_INFO("info");

        lcrsLog(INFO, "thsi is trace message {}", 3);
        lcrsLog(INFO, "thsi is trace message");
        lcrsLog(TRACE, "thsi is trace message");

        return EXIT_SUCCESS;
    }
};

int
main()
{
    Logger::init("lcrsTest", spdlog::level::trace);
    // loggerTest();
    return Test::system();
}