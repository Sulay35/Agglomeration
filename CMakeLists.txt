cmake_minimum_required(VERSION 3.10)

project(attraction)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# find package
find_package(SDL2 CONFIG REQUIRED)

# find package sdl2_ttf using FindSDL2_ttf in cmake folder
find_package(SDL2_ttf REQUIRED)

# find package sdl2_image using FindSDL2_image in cmake folder
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# sources
set(SOURCES
    src/attraction.cpp
    src/cell.cpp
    src/flowpoint.cpp
    src/vector2.cpp
)

# Find all .hpp headers
file(GLOB_RECURSE HEADERS
    include/*.hpp
    lib/PerlinNoise/PerlinNoise.hpp
)

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -w -Wall -lSDL2 -lSDL2_image -lSDL2_ttf")

# executable
add_executable(main ${SOURCES} ${HEADERS})

# include directories
target_include_directories(main PRIVATE
        include
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
)

#message(STATUS "${SDL2_IMAGE_INCLUDE_DIRS}")
#message(STATUS "${SDL2_IMAGE_FOUND}")
#message(STATUS "${SDL2_IMAGE_LIBRARIES}")
#message(STATUS "${SDL2_TTF_FOUND}")

# link librairies :
# SDL2, SDL2 ttf, SDL2 Image
target_link_libraries(main PRIVATE
        SDL2::SDL2
        ${SDL2_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
)

