include(FetchContent)

find_package(gtest QUIET)

if (NOT gtest_FOUND)
    FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )

    FetchContent_MakeAvailable(gtest)
endif()

if (TARGET gtest AND TARGET gtest_main)
    set(GTEST_LIBS gtest gtest_main)
else()
    find_package(GTest REQUIRED)
    set(GTEST_LIBS GTest::gtest GTest::gtest_main)
endif()