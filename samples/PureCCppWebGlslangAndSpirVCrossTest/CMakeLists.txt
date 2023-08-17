cmake_minimum_required(VERSION 2.9)

project(PureCCppWebGlslangAndSpirVCrossTest)

add_subdirectory(./glslang)
add_subdirectory(./SPIRV-Cross)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-nonportable-include-path -fno-exceptions -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-nonportable-include-path -fno-rtti")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -D_DEBUG=1 -Wno-unused -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g0 -DNDEBUG=1 -flto -O3")

if (EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	# TODO: all the debug flags are link only
	#---set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=2")
	#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s DISABLE_EXCEPTION_CATCHING=1")
	# --closure 1

	#---set(CMAKE_EXE_LINKER_FLAGS "-s ASSERTIONS=0 -s WASM=1 -s USE_WEBGPU=1 -s NO_EXIT_RUNTIME=1 -s STRICT=1 --shell-file ${CMAKE_CURRENT_LIST_DIR}/shell_minimal_runtime.html")
	set(CMAKE_EXE_LINKER_FLAGS "-s WASM=1 --shell-file ${CMAKE_CURRENT_LIST_DIR}/shell.html")
	# -s EVAL_CTORS=1

	# Linker flags to optimize for smallest output code size
	#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ENVIRONMENT=web -s MINIMAL_RUNTIME=2 -s TEXTDECODER=2 -s ABORTING_MALLOC=0 -s ALLOW_MEMORY_GROWTH=0 -s SUPPORT_ERRNO=0 -s MALLOC=emmalloc -s NO_FILESYSTEM=1 --output_eol=linux")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
elseif (WIN32)
	file(GLOB_RECURSE platform_sources src/win/*)
elseif (APPLE)
	file(GLOB_RECURSE platform_sources src/mac/*)
endif()

message("CMAKE_CXX_FLAGS::${CMAKE_CXX_FLAGS}")
message("CMAKE_EXE_LINKER_FLAGS::${CMAKE_EXE_LINKER_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG::${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE::${CMAKE_CXX_FLAGS_RELEASE}")

message("sources::${sources}")
message("platform_sources::${platform_sources}")
message("headers::${headers}")

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE 
${EMSDK}/upstream/emscripten/cache/sysroot/include
./glslang
./SPIRV-Cross
)

target_link_libraries(${PROJECT_NAME} 
MachineIndependent 
glslang 
glslang-default-resource-limits 
OGLCompiler 
OSDependent 
GenericCodeGen 
SPIRV
spirv-cross-core
spirv-cross-glsl
spirv-cross-hlsl
)



# emcmake cmake -G"MinGW Makefiles" ..