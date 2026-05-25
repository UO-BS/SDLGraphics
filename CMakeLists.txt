cmake_minimum_required(VERSION 3.10.0)
project(SDLGraphics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SDLGraphics 
    GGraphicalComponent.cpp 
    GTexturePolygon.cpp
    GTextureSet.cpp 
    MainWindow.cpp 
    WorldPosition.cpp
    main.cpp 
)

# Find SDL2 Packages
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Link SDL2
target_link_libraries(SDLGraphics PRIVATE 
    mingw32
    SDL2::SDL2main 
    SDL2::SDL2 
    SDL2_ttf::SDL2_ttf
)
