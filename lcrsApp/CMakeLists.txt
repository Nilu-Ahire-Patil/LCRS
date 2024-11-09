cmake_minimum_required(VERSION 3.10)


project(
    lcrsApp
        VERSION 1.0.0
        LANGUAGES CXX
)


add_executable(
    ${PROJECT_NAME}
        main.cpp
)


target_link_libraries(
    ${PROJECT_NAME}
        PRIVATE
            lcrs::lcrs
)