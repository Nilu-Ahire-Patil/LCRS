include(FetchContent)

find_package(boost::boost QUIET)

if (NOT spdlog_FOUND)
    FetchContent_Declare(
        boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.86.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )

    FetchContent_MakeAvailable(boost)
endif()