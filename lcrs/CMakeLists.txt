cmake_minimum_required(VERSION 3.10)


project(
    lcrs
        VERSION 1.0.0
        LANGUAGES CXX
)


set(FETCHCONTENT_QUIET OFF)
set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)


include(FetchContent)

FetchContent_Declare(
    Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.86.0
)

FetchContent_MakeAvailable(Boost)


file(GLOB
    HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
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
)


target_include_directories(
    ${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
            $<INSTALL_INTERFACE:include>
)

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
