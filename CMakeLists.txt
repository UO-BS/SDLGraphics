cmake_minimum_required(VERSION 3.10.0)
project(SDLGraphics LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(SDLGraphics 
    src/glad.c
    src/GMesh.cpp 
    src/GTextureManager.cpp 
    src/GMeshManager.cpp 
    src/GAssetManager.cpp
    src/GShaderLoader.cpp
    src/GMeshBuilder.cpp
    src/MainWindow.cpp 
    src/WorldPosition.cpp
    src/main.cpp 
)

# Find SDL2 Packages
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
# Find OpenGL
find_package(OpenGL REQUIRED)

# Link Libraries
target_link_libraries(SDLGraphics PRIVATE 
    mingw32
    SDL2::SDL2main 
    SDL2::SDL2 
    SDL2_ttf::SDL2_ttf
    OpenGL::GL
)
