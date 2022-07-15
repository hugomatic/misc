cmake_minimum_required(VERSION 3.7)
project(sdlgl)


find_package(SDL2 REQUIRED)
include_directories(sdlgl ${SDL2_INCLUDE_DIRS})

add_subdirectory(glad/)

add_executable(sdlgl sdl.cpp)
target_link_libraries(sdlgl
                      PRIVATE glad
                      ${SDL2_LIBRARIES}
)


