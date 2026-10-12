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

<<<<<<< Updated upstream
add_executable(${PROJECT_NAME} main.cpp
        engine.cpp
        engine.h)
=======
# 1. Wskazanie folderu z plikami nagłówkowymi (.h)
include_directories(include)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/engine.cpp
        src/math.cpp
        src/logs.cpp
        src/input.cpp
        src/render.cpp
        src/timer.cpp
        src/error_handling.cpp

        include/engine.h
        include/math.h
        include/logs.h
        include/input.h
        include/timer.h
        include/render.h
        include/error_handling.h)
>>>>>>> Stashed changes

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)