cmake_minimum_required(VERSION 3.29)

project(AdventOfCode2024 VERSION 1.0.0
                         DESCRIPTION "Advent of Code 2024"
                         LANGUAGES CXX)

add_subdirectory(src)

add_custom_target(
    run ALL,
    COMMAND Results
)