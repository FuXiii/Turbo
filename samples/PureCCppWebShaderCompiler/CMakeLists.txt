cmake_minimum_required(VERSION 2.9)

project(WasmShaderCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(./glslang)
add_subdirectory(./SPIRV-Cross)
#add_subdirectory(./SPIRV-Tools)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-nonportable-include-path -fno-exceptions -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-function -Wno-format-security -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-reorder-ctor -Wno-sign-compare -Wno-nonportable-include-path -fno-rtti")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -D_DEBUG=1 -Wno-unused -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g0 -DNDEBUG=1 -flto -O3")

if (EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	# TODO: all the debug flags are link only
	#---set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=2")
	#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s DISABLE_EXCEPTION_CATCHING=1")
	# --closure 1
set(SHELL_FILE "${CMAKE_CURRENT_LIST_DIR}/shell.html")
#set(SHELL_FILE "${CMAKE_CURRENT_LIST_DIR}/mini_shell.html")
	#---set(CMAKE_EXE_LINKER_FLAGS "-s ASSERTIONS=0 -s WASM=1 -s USE_WEBGPU=1 -s NO_EXIT_RUNTIME=1 -s STRICT=1 --shell-file ${SHELL_FILE}")
	set(CMAKE_EXE_LINKER_FLAGS "-s USE_WEBGPU=1 -s USE_GLFW=3 -s WASM=1 --shell-file ${SHELL_FILE}")
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

set(IMGUI_PATH "${CMAKE_CURRENT_LIST_DIR}/imgui")
set(IMGUI_COLOR_TEXT_EDIT_PATH "${CMAKE_CURRENT_LIST_DIR}/ImGuiColorTextEdit")

set(IMGUI_SOURCES
"${IMGUI_PATH}/imgui_demo.cpp"
"${IMGUI_PATH}/imgui_draw.cpp"
"${IMGUI_PATH}/imgui_tables.cpp"
"${IMGUI_PATH}/imgui_widgets.cpp"
"${IMGUI_PATH}/imgui.cpp"

"${IMGUI_PATH}/backends/imgui_impl_glfw.cpp"
"${IMGUI_PATH}/backends/imgui_impl_wgpu.cpp"
)

set(IMGUI_COLOR_TEXT_EDIT_SOURCES
"${IMGUI_COLOR_TEXT_EDIT_PATH}/TextEditor.cpp"
)

add_library(SpirVToolStatic STATIC "E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/include/spirv-tools/libspirv.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/bitutils.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/bit_vector.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/hash_combine.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/hex_float.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/make_unique.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/parse_number.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/small_vector.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/string_utils.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/timer.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/assembly_grammar.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/binary.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/cfa.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/common_debug_info.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/diagnostic.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/disassemble.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/enum_set.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/enum_string_mapping.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/ext_inst.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/extensions.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/instruction.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/latest_version_glsl_std_450_header.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/latest_version_opencl_std_header.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/latest_version_spirv_header.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/macro.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/name_mapper.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/opcode.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/operand.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/parsed_operand.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/print.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_constant.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_definition.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_endian.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_fuzzer_options.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_optimizer_options.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_reducer_options.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_target_env.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_validator_options.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/table.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/text.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/text_handler.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/bit_vector.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/parse_number.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/util/string_utils.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/assembly_grammar.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/binary.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/diagnostic.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/disassemble.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/enum_string_mapping.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/ext_inst.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/extensions.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/libspirv.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/name_mapper.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/opcode.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/operand.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/parsed_operand.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/print.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/software_version.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_endian.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_fuzzer_options.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_optimizer_options.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_reducer_options.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_target_env.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/spirv_validator_options.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/table.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/text.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/text_handler.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_adjacency.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_annotation.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_arithmetics.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_atomics.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_barriers.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_bitwise.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_builtins.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_capability.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_cfg.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_composites.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_constants.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_conversion.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_debug.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_decorations.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_derivatives.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_extensions.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_execution_limitations.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_function.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_id.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_image.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_interfaces.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_instruction.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_layout.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_literals.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_logicals.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_memory.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_memory_semantics.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_mesh_shading.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_misc.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_mode_setting.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_non_uniform.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_primitives.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_ray_query.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_ray_tracing.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_ray_tracing_reorder.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_scopes.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_small_type_uses.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validate_type.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/decoration.h;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/basic_block.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/construct.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/function.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/instruction.cpp;E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/val/validation_state.cpp")

target_include_directories(SpirVToolStatic
PUBLIC "E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools"
PUBLIC "E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/include"
PUBLIC "E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/include"
PUBLIC "E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools/source/wasm/external/spirv-headers/include"
#PUBLIC "E:/TestDelete/SPIRV-ToolsTest/build/SPIRV-Tools"
PUBLIC "E:/TestDelete/WebAssemblyTest/WasmImGuiTest/build/glslang/External/spirv-tools"
)

# E:/TestDelete/WebAssemblyTest/WasmImGuiTest/SPIRV-Tools

add_executable(${PROJECT_NAME} main.cpp ${IMGUI_SOURCES} ${IMGUI_COLOR_TEXT_EDIT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
${EMSDK}/upstream/emscripten/cache/sysroot/include
"${CMAKE_CURRENT_LIST_DIR}/imgui"
"${CMAKE_CURRENT_LIST_DIR}/imgui/backends"
"${IMGUI_COLOR_TEXT_EDIT_PATH}"
./glslang
./SPIRV-Cross
./SPIRV-Tools/include
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
spirv-cross-cpp
spirv-cross-reflect
spirv-cross-msl
spirv-cross-util

#SPIRV-Tools-opt
#SPIRV-Tools-static
SpirVToolStatic
)



# emcmake cmake -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DENABLE_GLSLANG_JS=ON -DENABLE_HLSL=ON -DBUILD_TESTING=OFF -DENABLE_OPT=OFF -DINSTALL_GTEST=OFF ..