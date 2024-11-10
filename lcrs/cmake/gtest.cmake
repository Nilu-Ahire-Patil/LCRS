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