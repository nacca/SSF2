cmake_minimum_required(VERSION 3.8)
project(SSF2)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Find and link SDL2, SDL2_image, SDL2_mixer, JsonCpp
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(JsonCpp REQUIRED)

#set(SDL_DISABLE_IMMINTRIN_H 1)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} src/Main.cpp ${HEADERS} ${CPP_SOURCES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY} ${JSONCPP_IMPORTED_LIBRARY})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")