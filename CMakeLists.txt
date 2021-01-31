cmake_minimum_required(VERSION 3.10)

project(CoccoiMannuSDL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(CoccoiMannuSDL main.cpp)

add_library(GLAD include/glad/glad.c)

add_library(stb_image include/stb_image/stb_image.c)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmakeModules")

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	set(USE_FLAGS "-s USE_SDL=2 -s USE_SDL_MIXER=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
else()
	find_package(SDL2 REQUIRED)

	find_package(SDL2_mixer REQUIRED)

	find_package(assimp)
endif()
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL_MIXER_INCLUDE_DIRS})
include_directories(${ASSIMP_INCLUDE_DIR})

target_include_directories(
	CoccoiMannuSDL PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/include/glad"
	"${PROJECT_SOURCE_DIR}/include/stb_image"
	"${PROJECT_SOURCE_DIR}/include/KHR"
)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	target_link_libraries(CoccoiMannuSDL ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${ASSIMP_LIBRARIES} GLAD stb_image)
else()
	target_link_libraries(CoccoiMannuSDL SDL2::SDL2 ${SDL2_MIXER_LIBRARIES} ${ASSIMP_LIBRARIES} GLAD stb_image ${CMAKE_DL_LIBS})
endif()

file(COPY ${PROJECT_SOURCE_DIR}/res DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/)
