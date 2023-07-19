cmake_minimum_required(VERSION 3.0)

project(PureCCppWebGPUTest)

if (EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s DISABLE_EXCEPTION_CATCHING=1")
	set(CMAKE_EXE_LINKER_FLAGS "-s ASSERTIONS=0 -s WASM=1 -s USE_WEBGPU=1 -s NO_EXIT_RUNTIME=1 -s STRICT=1 --shell-file ${CMAKE_CURRENT_LIST_DIR}/shell.html")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ENVIRONMENT=web -s MINIMAL_RUNTIME=2 -s TEXTDECODER=2 -s ABORTING_MALLOC=0 -s ALLOW_MEMORY_GROWTH=0 -s SUPPORT_ERRNO=0 -s MALLOC=emmalloc -s NO_FILESYSTEM=1 --output_eol=linux")

    add_executable(${PROJECT_NAME} main.cpp)
    
    target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{EMSDK}/upstream/emscripten/cache/sysroot/include")
endif()