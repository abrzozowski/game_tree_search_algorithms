cmake_minimum_required(VERSION 3.14.7)
project(game_tree_search_algorithms)

set (CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(main_tic_tac_toe src/main_tic_tac_toe.cpp)
