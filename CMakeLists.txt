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

# 1. Wskazanie folderu z plikami nagłówkowymi (.h)
include_directories(include
        include/core
        include/graphics
        include/system)

add_executable(${PROJECT_NAME}
        src/main.cpp

        # Logika rdzenia (core)
        src/core/engine.cpp
        include/core/engine.h
        src/core/timer.cpp
        include/core/timer.h

        # Grafika (graphics)
        src/graphics/primitiverenderer.cpp
        include/graphics/primitiverenderer.h

        # system
        src/system/math.cpp
        include/system/math.h
        src/system/logs.cpp
        include/system/logs.h
        src/system/input.cpp
        include/system/input.h
        src/system/error_handling.cpp
        include/system/error_handling.h
        src/graphics/point2D.cpp
        include/graphics/point2D.h
        include/graphics/linesegment.h
        src/graphics/linesegment.cpp
        include/core/object_hierarchy.h
        include/game/player.h
        src/game/player.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)