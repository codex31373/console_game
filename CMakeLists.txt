cmake_minimum_required(VERSION 3.16)

project(ConsoleGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create executable
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/GameObject.cpp
    src/Player.cpp
)

set(HEADERS
    src/Game.hpp
    src/GameObject.hpp
    src/Player.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${SDL2_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${SDL2_LIBRARIES}
)

# Compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
