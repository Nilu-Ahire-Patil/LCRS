include(FetchContent)

find_package(spdlog QUIET)

if (NOT spdlog_FOUND)
    option(SPDLOG_BUILD_SHARED "build spdlog as shared library" ${BUILD_SHARED_LIBS})

    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.0
    )

    FetchContent_MakeAvailable(spdlog)
endif()