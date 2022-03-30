cmake_minimum_required(VERSION 3.16)
project(NotificationCenter++)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)
include_directories(include)

add_executable(NotificationCenter++
        src/main.cpp
        src/NotificationCenter.cpp)

set_target_properties(NotificationCenter++ PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(NotificationCenter++ PUBLIC cxx_std_20)