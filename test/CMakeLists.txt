cmake_minimum_required(VERSION 3.5)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

target_include_directories(paillier_small PUBLIC ${CMAKE_SOURCE_DIR}/src)

find_package(Catch2 REQUIRED)

add_executable(unit_tests test.cpp)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

project(testing LANGUAGES CXX VERSION 0.0.1)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(unit_tests)
