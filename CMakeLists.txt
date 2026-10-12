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

set(ENGINE_SOURCES
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

add_executable(${PROJECT_NAME} main.cpp ${ENGINE_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

include(CTest)
if(BUILD_TESTING)
    add_executable(engine_tests tests/engine_tests.cpp ${ENGINE_SOURCES})
    target_include_directories(engine_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(engine_tests PRIVATE SDL3::SDL3)
    add_test(NAME lab01_engine COMMAND engine_tests)
    set_tests_properties(lab01_engine PROPERTIES
        ENVIRONMENT "SDL_VIDEODRIVER=dummy;SDL_RENDER_DRIVER=software"
        TIMEOUT 15)
    add_test(NAME lab01_help COMMAND Engine2D --help)
    add_test(NAME lab01_invalid_fps COMMAND Engine2D --fps 0)
    set_tests_properties(lab01_invalid_fps PROPERTIES WILL_FAIL TRUE)
endif()
