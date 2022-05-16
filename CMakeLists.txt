cmake_minimum_required(VERSION 3.0)

project(Turbo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_BUILD_TYPE "debug")

add_subdirectory(./engine)

add_subdirectory(./thirdparty)

add_executable(${PROJECT_NAME} ./main.cpp)

add_dependencies(${PROJECT_NAME} glfw)

target_include_directories(${PROJECT_NAME}
    PUBLIC ./engine/core/include
    )

target_link_directories(${PROJECT_NAME} 
PUBLIC /engine/core/include/
PUBLIC /thirdparty/glfw/src/
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
TCore
glfw
)
