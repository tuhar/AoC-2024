add_library(AdventOfCode2024 days.cpp days.hpp)
target_include_directories(AdventOfCode2024 PUBLIC ../src)
target_compile_features(AdventOfCode2024 PUBLIC cxx_std_20)

add_executable(Results main.cpp)
target_compile_features(Results PRIVATE cxx_std_20)
target_link_libraries(Results PRIVATE AdventOfCode2024)
