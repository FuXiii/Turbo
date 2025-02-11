project(TCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(VMA_STATIC_VULKAN_FUNCTIONS OFF)
set(VMA_DYNAMIC_VULKAN_FUNCTIONS ON)

aux_source_directory(./src/ SRCS)

add_subdirectory(./thirdparty)

#if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/VulkanMemoryAllocator/src/VmaUsage.cpp")
#set(VMA_SRCS 
#./thirdparty/VulkanMemoryAllocator/src/VmaUsage.cpp
#./thirdparty/VulkanMemoryAllocator/include/vk_mem_alloc.h
#)
#else()
#message(FATAL_ERROR "Can not find VulkanMemoryAllocator")
#endif()

set(VMA_SRCS 
${CMAKE_CURRENT_SOURCE_DIR}/VmaUsage/TVmaUsage.h
${CMAKE_CURRENT_SOURCE_DIR}/VmaUsage/TVmaUsage.cpp
)

find_package(Python COMPONENTS Interpreter)
if(Python_Interpreter_FOUND)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glslang/update_glslang_sources.py")
    execute_process(
        COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glslang/update_glslang_sources.py"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glslang"
        RESULT_VARIABLE update_glslang_sources_result_variable
    )
    if(NOT update_glslang_sources_result_variable EQUAL 0)
    message(FATAL_ERROR "")
    endif()
    else()
    message(FATAL_ERROR "Can not find update_glslang_sources.py in glslang")
    endif()
else()
    message(FATAL_ERROR "Python interpreter not found")
endif()

if(
EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SPIRV-Reflect/spirv_reflect.h" AND 
EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SPIRV-Reflect/spirv_reflect.c" AND 
EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SPIRV-Reflect/include/spirv/unified1/spirv.h"
)
set(SPIRV_REFLECT_SRCS 
${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SPIRV-Reflect/spirv_reflect.c
)
set(SPIRV_REFLECT_HEADERS
${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SPIRV-Reflect/spirv_reflect.h
)
set(SPIRV_REFLECT_INCLUDE_PATH
${CMAKE_SOURCE_DIR}/engine/core/thirdparty/SPIRV-Reflect
${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SPIRV-Reflect/include
)
else()
message(FATAL_ERROR "Can not find SPIRV-Reflect")
endif()

option(TCORE_SHARED_LIBS "flag to create TCore shared libraries if on" OFF)

if(TCORE_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_library(${PROJECT_NAME} SHARED
        ${VMA_SRCS}
        ${SRCS}
        ${SPIRV_REFLECT_SRCS}
        ${SPIRV_REFLECT_HEADERS}
    )
    target_compile_definitions(${PROJECT_NAME} PUBLIC TCORE_SHARED_LIBS)
else() 
    add_library(${PROJECT_NAME} STATIC
        ${VMA_SRCS}
        ${SRCS}
        ${SPIRV_REFLECT_SRCS}
        ${SPIRV_REFLECT_HEADERS}
    )
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} 
PRIVATE glslang
PRIVATE glslang-default-resource-limits
#PRIVATE SPIRV-Tools
#PRIVATE SPIRV-Tools-lint
#PRIVATE SPIRV-Tools-opt
#PRIVATE SPIRV-Tools-reduce
#PRIVATE SPIRV-Tools-shared
#PRIVATE SPIRV-Tools-diff
#PRIVATE SPIRV-Tools-link
PRIVATE spirv-cross-core
PRIVATE spirv-cross-glsl
PRIVATE spirv-cross-hlsl
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    VMA_STATIC_VULKAN_FUNCTIONS=$<BOOL:${VMA_STATIC_VULKAN_FUNCTIONS}>
    VMA_DYNAMIC_VULKAN_FUNCTIONS=$<BOOL:${VMA_DYNAMIC_VULKAN_FUNCTIONS}>
    VMA_DEBUG_ALWAYS_DEDICATED_MEMORY=$<BOOL:${VMA_DEBUG_ALWAYS_DEDICATED_MEMORY}>
    VMA_DEBUG_INITIALIZE_ALLOCATIONS=$<BOOL:${VMA_DEBUG_INITIALIZE_ALLOCATIONS}>
    VMA_DEBUG_GLOBAL_MUTEX=$<BOOL:${VMA_DEBUG_GLOBAL_MUTEX}>
    VMA_DEBUG_DONT_EXCEED_MAX_MEMORY_ALLOCATION_COUNT=$<BOOL:${VMA_DEBUG_DONT_EXCEED_MAX_MEMORY_ALLOCATION_COUNT}>
    SPIRV_REFLECT_USE_SYSTEM_SPIRV_H
)

target_include_directories(${PROJECT_NAME}
PUBLIC ./include
PRIVATE ./thirdparty/VulkanMemoryAllocator/include
PRIVATE ./thirdparty/SPIRV-Cross
PRIVATE ./thirdparty/glslang
PRIVATE ${SPIRV_REFLECT_INCLUDE_PATH}
)