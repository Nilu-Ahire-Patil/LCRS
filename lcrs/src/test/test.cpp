#include "lcrs/test/test.h"
#include "gtest/gtest.h"

int Test::all()
{
    int result = system();
    result &= logger();
    return result;
}

int Test::system()
{
    return System::getMacAddressMap().empty() ? EXIT_FAILURE : EXIT_SUCCESS;
}

int Test::logger()
{
    SPDLOG_TRACE("this is trace message");
    SPDLOG_ERROR("this is error message");

    SPDLOG_INFO("info");

    lcrsLog(INFO, "thsi is trace message {}", 3);
    lcrsLog(INFO, "thsi is trace message");
    lcrsLog(TRACE, "thsi is trace message");

    return EXIT_SUCCESS;
}

int main(int argc, char** argv){
    Logger::init("lcrsTest", spdlog::level::trace);
    return Test::all();
}


TEST(System, macAddressMap)
{
    int available_address_count = System::getMacAddressMap().size();
    EXPECT_GT(available_address_count, 0);
}