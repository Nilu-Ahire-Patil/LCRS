cmake_minimum_required(VERSION 3.10)

PROJECT(Lcrs)

#FILE(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
#FILE(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

include(ExternalProject)

# Function to check and install jsoncpp
function(install_jsoncpp)
    ExternalProject_Add(
        jsoncpp
        GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
        GIT_TAG master  # or specify a version/tag
        BUILD_IN_SOURCE TRUE
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/jsoncpp_install
    )
endfunction()

# Find jsoncpp
find_package(JsonCpp QUIET)

if(NOT JsonCpp_FOUND)
    message(STATUS "jsoncpp not found, installing...")
    install_jsoncpp()
else()
    message(STATUS "Found jsoncpp")
endif()

add_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/lcrs.cpp)
#add_executable(${PROJECT_NAME}TestApp ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/jsoncpp_install/include
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/jsoncpp_install/lib/libjsoncpp.a)

#target_include_directories(${PROJECT_NAME}TestApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION bin)
install(TARGETS ${PROJECT_NAME} PUBLIC_HEADER DESTINATION include)
install(TARGETS ${PROJECT_NAME} PRIVATE_HEADER DESTINATION include/internal)

# install(TARGETS ${PROJECT_NAME}TestApp LIBRARY DESTINATION lib)
# install(TARGETS ${PROJECT_NAME}TestApp ARCHIVE DESTINATION bin)
# install(TARGETS ${PROJECT_NAME}TestApp PUBLIC_HEADER DESTINATION include)
# install(TARGETS ${PROJECT_NAME}TestApp PRIVATE_HEADER DESTINATION include/internal)