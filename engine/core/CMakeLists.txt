project(TCore)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

aux_source_directory(./src/ SRCS)

add_subdirectory(./thirdparty)

add_library(${PROJECT_NAME} STATIC
$<TARGET_OBJECTS:VulkanMemoryAllocator>
$<TARGET_OBJECTS:MachineIndependent>
$<TARGET_OBJECTS:glslang>
$<TARGET_OBJECTS:OGLCompiler>
$<TARGET_OBJECTS:OSDependent>
$<TARGET_OBJECTS:GenericCodeGen>
$<TARGET_OBJECTS:SPIRV>
$<TARGET_OBJECTS:spirv-cross-core>
$<TARGET_OBJECTS:spirv-cross-glsl>
$<TARGET_OBJECTS:spirv-cross-hlsl>
${SRCS} 
)

target_include_directories(${PROJECT_NAME}
PUBLIC ./include
PRIVATE ./thirdparty/VulkanMemoryAllocator/include
PRIVATE ./thirdparty/SPIRV-Cross
PRIVATE ./thirdparty/glslang
)