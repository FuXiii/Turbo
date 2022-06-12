project(TCore)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_library(vulkan STATIC IMPORTED GLOBAL)
set_target_properties(vulkan PROPERTIES IMPORTED_LOCATION /vulkan/1.3.211.0/x86_64/lib/libvulkan.so)

aux_source_directory(./src/ SRCS)

add_library(${PROJECT_NAME} STATIC ${SRCS})

add_subdirectory(./thirdparty)

target_include_directories(${PROJECT_NAME}
    PUBLIC ./include
    PUBLIC ./thirdparty/SPIRV-Cross
    PUBLIC ./thirdparty/glfw/include/
    )

target_link_directories(${PROJECT_NAME} 
PUBLIC ./thirdparty/VulkanMemoryAllocator/src
PUBLIC ./thirdparty/glslang/glslang
PUBLIC ./thirdparty/glslang/glslang/OSDependent

PUBLIC ./thirdparty/glslang/hlsl
PUBLIC ./thirdparty/glslang/OGLCompilersDLL
PUBLIC ./thirdparty/glslang/SPIRV

PUBLIC ./thirdparty/SPIRV-Cross
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
)