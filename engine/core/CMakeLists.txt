project(TCore)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

aux_source_directory(./src/ SRCS)

add_library(${PROJECT_NAME} STATIC ${SRCS})

add_subdirectory(./thirdparty)

target_include_directories(${PROJECT_NAME}
    PUBLIC ./include
    PUBLIC ./thirdparty/SPIRV-Cross
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

target_link_libraries(${PROJECT_NAME}  
PUBLIC VulkanMemoryAllocator
PUBLIC glslang
PUBLIC OGLCompiler
PUBLIC OSDependent
PUBLIC SPIRV
PUBLIC spirv-cross-core
PUBLIC spirv-cross-glsl
PUBLIC spirv-cross-hlsl
)