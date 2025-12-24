cmake_minimum_required(VERSION 3.21)

project(SDL3Wheels VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL2
find_package(SDL2 REQUIRED CONFIG)

# Source files
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/kart.cpp
    src/track.cpp
    src/input_handler.cpp
    src/renderer.cpp
)

set(HEADERS
    include/game.h
    include/kart.h
    include/track.h
    include/input_handler.h
    include/renderer.h
)

# Create executable
add_executable(SDL3Wheels ${SOURCES} ${HEADERS})

target_include_directories(SDL3Wheels PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link SDL2
target_link_libraries(SDL3Wheels PRIVATE 
    SDL2::SDL2
    SDL2::SDL2main
)

# Copy assets to build directory
add_custom_command(TARGET SDL3Wheels POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SDL3Wheels>/assets
)

# Installation
install(TARGETS SDL3Wheels
    RUNTIME DESTINATION bin
)

install(DIRECTORY assets/
    DESTINATION bin/assets
)
