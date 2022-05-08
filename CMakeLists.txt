cmake_minimum_required(VERSION 3.0)

project(Turbo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_BUILD_TYPE "debug")

add_library(vulkan STATIC IMPORTED GLOBAL)
set_target_properties(vulkan PROPERTIES IMPORTED_LOCATION F:/VulkanSDK/1.3.204.1/Lib/vulkan-1.lib)

aux_source_directory(./engine/src/ SRCS)

add_executable(${PROJECT_NAME} ./main.cpp ${SRCS})

add_subdirectory(./thirdparty)

add_subdirectory(./engine)

add_dependencies(${PROJECT_NAME} SDL2-static)

target_include_directories(${PROJECT_NAME}
    PUBLIC ./engine/include
    PUBLIC ./thirdparty/SPIRV-Cross
    PUBLIC ./thirdparty/SDL2/include/
    )

target_link_directories(${PROJECT_NAME} 
PUBLIC thirdparty/VulkanMemoryAllocator/src
PUBLIC thirdparty/glslang/glslang
PUBLIC thirdparty/glslang/glslang/OSDependent

PUBLIC thirdparty/glslang/hlsl
PUBLIC thirdparty/glslang/OGLCompilersDLL
PUBLIC thirdparty/glslang/SPIRV

PUBLIC thirdparty/SPIRV-Cross
PUBLIC /thirdparty/SDL2/
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
vulkan
VulkanMemoryAllocator
glslang
OGLCompiler
OSDependent
SPIRV
spirv-cross-core
spirv-cross-glsl
spirv-cross-hlsl
${SDL2_LIBRARIES}
SDL2-static
)
