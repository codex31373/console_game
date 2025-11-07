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
    src/Platform.cpp
    src/Bird.cpp
)

set(HEADERS
    src/Game.hpp
    src/GameObject.hpp
    src/Player.hpp
    src/Platform.hpp
    src/Bird.hpp
    src/Common.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Find required packages - platform-specific
if(WIN32)
    # Windows: Use CMake's find modules (works with vcpkg)
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG QUIET)
    if(NOT SDL2_ttf_FOUND)
        find_package(sdl2-ttf CONFIG REQUIRED)
    endif()
    set(SDL2_TARGETS SDL2::SDL2)
    if(TARGET SDL2::SDL2main)
        list(APPEND SDL2_TARGETS SDL2::SDL2main)
    endif()
    if(TARGET SDL2_ttf::SDL2_ttf)
        set(SDL2_TTF_TARGETS SDL2_ttf::SDL2_ttf)
    elseif(TARGET SDL2::SDL2_ttf)
        set(SDL2_TTF_TARGETS SDL2::SDL2_ttf)
    else()
        message(FATAL_ERROR "SDL2_ttf imported target not found after find_package")
    endif()
else()
    # Linux/macOS: Use pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    
    # Find SDL2_ttf using pkg-config
    execute_process(
        COMMAND pkg-config --libs SDL2_ttf
        OUTPUT_VARIABLE SDL2_TTF_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    execute_process(
        COMMAND pkg-config --cflags SDL2_ttf
        OUTPUT_VARIABLE SDL2_TTF_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    # Set the variables that our code expects
    set(SDL2_TTF_LIBRARIES ${SDL2_TTF_LIBS})
    set(SDL2_TTF_INCLUDE_DIRS "")
    
    # If pkg-config didn't find it, try manual search
    if(NOT SDL2_TTF_LIBS)
        find_path(SDL2_TTF_INCLUDE_DIRS SDL_ttf.h
            PATH_SUFFIXES SDL2
            /usr/include/SDL2
            /usr/local/include/SDL2
        )
        
        find_library(SDL2_TTF_LIBRARIES
            NAMES SDL2_ttf
            PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
        )
        
        if(NOT SDL2_TTF_INCLUDE_DIRS OR NOT SDL2_TTF_LIBRARIES)
            message(FATAL_ERROR "SDL2_ttf not found! Please install libsdl2-ttf-dev package")
        endif()
    endif()

    set(SDL2_TARGETS ${SDL2_LIBRARIES})
    set(SDL2_TTF_TARGETS ${SDL2_TTF_LIBRARIES})
endif()

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(NOT WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
    )
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${SDL2_TARGETS}
        ${SDL2_TTF_TARGETS}
)

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
