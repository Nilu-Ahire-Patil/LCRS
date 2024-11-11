cmake_minimum_required(VERSION 3.10)

project(lcrs_all VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lcrs)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lcrsApp)