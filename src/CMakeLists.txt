add_library(AdventOfCode2024 day_1.cpp day_2.cpp day_3.cpp day_4.cpp days.hpp)
target_include_directories(AdventOfCode2024 PUBLIC ../src)
target_compile_features(AdventOfCode2024 PUBLIC cxx_std_20)

add_executable(Results main.cpp)
target_compile_features(Results PRIVATE cxx_std_20)
target_link_libraries(Results PRIVATE AdventOfCode2024)
