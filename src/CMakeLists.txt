cmake_minimum_required(VERSION 3.7)

project(Breeding)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define sources and executable
set(EXECUTABLE_NAME Breeding)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

add_executable(${EXECUTABLE_NAME} main.cpp global.cpp Display.cpp Brain.cpp Snake.cpp)

target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

add_definitions("-g")
#add_definitions("-Wall -O3")
