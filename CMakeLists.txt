# Set VCPKG_ROOT to your vcpkg installation directory or add the following to your cmake call:
# -DCMAKE_TOOLCHAIN=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake

cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(cpp-tdd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}/bin")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}/bin")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}/lib")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}/lib")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}/lib")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}/lib")


####################
include(FetchContent)
# GTest
FetchContent_Declare(
    googletest 
    URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

####################
# Catch2
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

##############
## Trompeloeil
Include(FetchContent)
FetchContent_Declare(
  Trompeloeil
  GIT_REPOSITORY https://github.com/rollbear/trompeloeil.git
  GIT_TAG        v49 # or a later release
)
FetchContent_MakeAvailable(Trompeloeil)

##############
Include(FetchContent)
FetchContent_Declare(
  bext_di
  GIT_REPOSITORY https://github.com/boost-ext/di.git
  GIT_TAG        v1.3.2
)
FetchContent_MakeAvailable(bext_di)

include(CTest)

enable_testing()

add_subdirectory(_project-template-catch)
add_subdirectory(_project-template-gtest)
add_subdirectory(catch-demo)
add_subdirectory(googlemock-demo)
add_subdirectory(injecting-dependencies)
add_subdirectory(mocks-vs-stubs)
add_subdirectory(mother-vs-builder)

add_subdirectory(tdd-katas/catch2/bowling-catch)
add_subdirectory(tdd-katas/catch2/recently-used-list-catch)
add_subdirectory(tdd-katas/catch2/mars-rover-catch)

add_subdirectory(tdd-katas/gtest/bowling)
add_subdirectory(tdd-katas/gtest/recently-used-list)
add_subdirectory(tdd-katas/gtest/mars-rover)

# add_subdirectory(document-editor)
