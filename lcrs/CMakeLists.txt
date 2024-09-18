cmake_minimum_required(VERSION 3.10)

project(Lcrs)

include(ExternalProject)

set(JSONCPP_INSTALL_DIR ${CMAKE_BINARY_DIR}/jsoncpp_source)

function(install_jsoncpp)
    ExternalProject_Add(
        jsoncpp
        GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
        GIT_TAG master  # specify a stable version if necessary
        SOURCE_DIR ${CMAKE_BINARY_DIR}/jsoncpp_source
        BINARY_DIR ${CMAKE_BINARY_DIR}/jsoncpp_build
        INSTALL_COMMAND ""  # Skip installation to prevent in-source issues
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${JSONCPP_INSTALL_DIR}
    )
endfunction()

find_package(JsonCpp QUIET)

if(NOT JsonCpp_FOUND)
    message(STATUS "jsoncpp not found, installing...")
    install_jsoncpp()
else()
    message(STATUS "Found jsoncpp")
endif()

add_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/lcrs.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${JSONCPP_INSTALL_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${JSONCPP_INSTALL_DIR}/lib/libjsoncpp.a
)

install(TARGETS ${PROJECT_NAME} 
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION bin
    PUBLIC_HEADER DESTINATION include
    PRIVATE_HEADER DESTINATION include/internal
)

# Add an executable target if needed (uncomment to use)
# add_executable(${PROJECT_NAME}TestApp ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
# target_link_libraries(${PROJECT_NAME}TestApp PRIVATE ${PROJECT_NAME})
# target_include_directories(${PROJECT_NAME}TestApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Install rules for the test application (uncomment to use)
# install(TARGETS ${PROJECT_NAME}TestApp 
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION bin
#     PUBLIC_HEADER DESTINATION include
#     PRIVATE_HEADER DESTINATION include/internal
# )
