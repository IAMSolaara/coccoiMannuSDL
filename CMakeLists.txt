cmake_minimum_required(VERSION 3.10)

project(CoccoiMannuSDL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(CoccoiMannuSDL main.cpp)

add_library(GLAD include/glad/glad.c)

add_library(stb_image include/stb_image/stb_image.c)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmakeModules")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_mixer REQUIRED)
include_directories(${SDL_MIXER_INCLUDE_DIRS})

find_package(assimp)
include_directories(${ASSIMP_INCLUDE_DIR})

target_include_directories(
	CoccoiMannuSDL PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/include/glad"
	"${PROJECT_SOURCE_DIR}/include/stb_image"
	"${PROJECT_SOURCE_DIR}/include/KHR"
)

target_link_libraries(CoccoiMannuSDL SDL2::SDL2 ${SDL2_MIXER_LIBRARIES} ${ASSIMP_LIBRARIES} GLAD stb_image)

file(COPY ${PROJECT_SOURCE_DIR}/res DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/)
