cmake_minimum_required(VERSION 3.10)


project(
    lcrs
        VERSION 1.0.0
        LANGUAGES CXX
)

cmake_policy(SET CMP0077 NEW)

set(FETCHCONTENT_QUIET OFF)
set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)


include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/spdlog.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/boost.cmake)


file(GLOB
    HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

file(GLOB 
    SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)


add_library(
    ${PROJECT_NAME}
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

add_library(
    ${PROJECT_NAME}::${PROJECT_NAME}
        ALIAS ${PROJECT_NAME}
)

target_link_libraries(
    ${PROJECT_NAME}
        PRIVATE
            Boost::asio 
            Boost::system
            spdlog::spdlog
)

target_include_directories(
    ${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
            $<INSTALL_INTERFACE:include>
)

option(ENABLE_TESTS "build test" ON)

if(ENABLE_TESTS)

    include(CTest)
    enable_testing()
    add_compile_definitions(TEST_MODE)

    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/gtest.cmake)

    add_executable(
        ${PROJECT_NAME}Test
        src/test/test.cpp
    )

    target_include_directories(${PROJECT_NAME}Test
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(${PROJECT_NAME}Test
            PRIVATE
                spdlog::spdlog
                lcrs::lcrs
                GTest::gtest
                GTest::gtest_main
        )

    add_test(NAME ${PROJECT_NAME}Test COMMAND ${PROJECT_NAME}Test)

endif()

# set_target_properties(
#     ${PROJECT_NAME}
#         PROPERTIES
#             VERSION ${PROJECT_VERSION}
#             SOVERSION 1
#             EXPORT_NAME ${PROJECT_NAME}
# )


# install(TARGETS
#     ${PROJECT_NAME}
#         EXPORT ${PROJECT_NAME}Targets
#             LIBRARY DESTINATION lib
#             ARCHIVE DESTINATION lib
#             RUNTIME DESTINATION bin
#             INCLUDES DESTINATION include
# )


# install(
#     DIRECTORY include/
#     DESTINATION include
# )


# include(CMakePackageConfigHelpers)


# write_basic_package_version_file(
#     "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY AnyNewerVersion
# )


# configure_file(
#     cmake/${PROJECT_NAME}Config.cmake.in
#     ${PROJECT_NAME}Config.cmake @ONLY
# )


# install(FILES
#     "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#     "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#     DESTINATION lib/cmake/${PROJECT_NAME}
# )


# # Correct export command
# # install(EXPORT ${PROJECT_NAME}Targets
# #     FILE ${PROJECT_NAME}Targets.cmake
# #     NAMESPACE ${PROJECT_NAME}::
# #     DESTINATION lib/cmake/${PROJECT_NAME}
# # )
