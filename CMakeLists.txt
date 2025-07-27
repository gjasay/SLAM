cmake_minimum_required(VERSION 3.15)
project(SLAM VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------
# Add submodules include directories
# ------------------------

# Raylib (assumes you use raylib as source, it has its own CMake)
add_subdirectory(extern/raylib)

# For jolt and json, they might not have CMake, so just add include dirs
include_directories(
    extern/jolt/include
    extern/json/include
)

# ------------------------
# Engine Library
# ------------------------

file(GLOB_RECURSE ENGINE_SOURCES
    engine/src/*.cpp
    engine/include/*.h
)

add_library(engine STATIC
    engine/src/Camera.cpp
    engine/src/ECS.cpp
    engine/src/Engine.cpp
    engine/src/Scene.cpp
    engine/src/Script.cpp
    engine/src/ui/Canvas.cpp
        engine/src/ui/Element.cpp
        engine/include/ui/Style.h
        engine/src/ui/Style.cpp
)

target_include_directories(engine PUBLIC
    engine/include
    extern/raylib/src
    extern/raygui/src
    extern/jolt/include
    extern/json/include
)

target_link_libraries(engine PUBLIC raylib) # Link raylib to engine if needed

# ------------------------
# Editor Executable
# ------------------------

file(GLOB_RECURSE EDITOR_SOURCES
    editor/src/*.cpp
    editor/include/*.h
)

add_executable(editor ${EDITOR_SOURCES}
        engine/include/Vector.h)

target_include_directories(editor PUBLIC
    editor/include
    engine/include
    extern/raylib/src
    extern/jolt/include
    extern/json/include
)

target_link_libraries(editor PRIVATE engine raylib)

# ------------------------
# Optional: set output directories for binaries and libs
# ------------------------

set_target_properties(engine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

set_target_properties(editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
