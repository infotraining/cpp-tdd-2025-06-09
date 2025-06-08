##################
# Project
get_filename_component(PROJECT_NAME_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" PROJECT_MAIN ${PROJECT_NAME_DIR})

set(PROJECT_ID ${PROJECT_MAIN})
project(${PROJECT_ID})
message(STATUS "PROJECT_ID is: " ${PROJECT_ID})

enable_testing()
add_subdirectory(src)
add_subdirectory(tests)

####################
# Packages & libs
find_package(Threads REQUIRED)

####################
# Main app
add_executable(${PROJECT_MAIN} main.cpp)
# target_include_directories(${PROJECT_MAIN} PRIVATE ${BEXT_DI_INCLUDE_DIRS})
target_link_libraries(${PROJECT_MAIN} PRIVATE ${PROJECT_LIB} ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(${PROJECT_MAIN} PRIVATE ${bext_di_SOURCE_DIR}/include)
target_compile_features(${PROJECT_MAIN} PUBLIC cxx_std_20)
