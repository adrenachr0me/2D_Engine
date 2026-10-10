cmake_minimum_required(VERSION 4.0)
project(Engine2D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        main
)

FetchContent_MakeAvailable(SDL3)

add_executable(${PROJECT_NAME} main.cpp
        engine.cpp
        engine.h
        math.h
        logs.h
        input.h
        timer.h
        render.h
        math.cpp
        logs.cpp
        input.cpp
        render.cpp
        timer.cpp
        error_handling.h
        error_handling.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)