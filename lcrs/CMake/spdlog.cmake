include(FetchContent)

find_package(spdlog QUIET)

add_compile_definitions(SPDLOG_ACTIVE_LEVEL=0)

if (NOT spdlog_FOUND)
    option(SPDLOG_BUILD_SHARED "build spdlog as shared library" ${BUILD_SHARED_LIBS})

    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )

    FetchContent_MakeAvailable(spdlog)
endif()