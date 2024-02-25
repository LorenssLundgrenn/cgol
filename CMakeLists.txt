# command:
# cmake -S ./ -B ./build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++

cmake_minimum_required(VERSION 3.28.3)
project(cgol)

set(SRC_DIR ./src)

set( SRC 
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/constants.cpp
    ${SRC_DIR}/state.cpp
    ${SRC_DIR}/util.cpp
)

add_executable(${PROJECT_NAME} ${SRC})

set(EXTLIB_DIR C:/dependencies)
set(SDL_DIR ${EXTLIB_DIR}/SDL2-2.28.0)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ./includes
    PRIVATE ${SDL_DIR}/include
)

target_link_directories( ${PROJECT_NAME} 
    PRIVATE ${SDL_DIR}/lib/x64
)

target_link_libraries( ${PROJECT_NAME} 
    SDL2main SDL2
)

file (
    COPY ${SDL_DIR}/bin/x64/SDL2.dll 
    DESTINATION ${CMAKE_BINARY_DIR}
)