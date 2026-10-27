
cmake_minimum_required(VERSION 3.21)

project(SnakeGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(SnakeGame
    src/main.cpp
)

target_link_libraries(SnakeGame PRIVATE raylib)

if(MSVC)
    target_compile_options(SnakeGame PRIVATE /W4)
endif()
