find_package(Catch2 3 REQUIRED)

add_executable(tests day_5_test.cpp)
target_link_libraries(tests PRIVATE AdventOfCode2024 Catch2::Catch2WithMain)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

include(CTest)
include(Catch)
catch_discover_tests(tests)