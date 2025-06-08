cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)  # Set C++20 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Ensure the compiler uses the set standard

##################
# Project
get_filename_component(PROJECT_NAME_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" PROJECT_MAIN ${PROJECT_NAME_DIR})

set(PROJECT_ID ${PROJECT_MAIN})
project(${PROJECT_ID})
message(STATUS "PROJECT_ID is: " ${PROJECT_ID})

add_subdirectory(src)
add_subdirectory(tests)

####################
# Packages & libs
find_package(Threads REQUIRED)

####################
# Main app
add_executable(${PROJECT_MAIN} main.cpp)
target_link_libraries(${PROJECT_MAIN} PRIVATE ${PROJECT_LIB} ${CMAKE_THREAD_LIBS_INIT})
