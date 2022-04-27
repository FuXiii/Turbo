# CMake generated Testfile for 
# Source directory: E:/Turbo/thirdparty/glslang
# Build directory: E:/Turbo/build/thirdparty/glslang
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(glslang-testsuite "bash" "-o" "igncr" "runtests" "E:/Turbo/build/thirdparty/glslang/localResults" "E:/Turbo/build/thirdparty/glslang/StandAlone/glslangValidator" "E:/Turbo/build/thirdparty/glslang/StandAlone/spirv-remap")
set_tests_properties(glslang-testsuite PROPERTIES  WORKING_DIRECTORY "E:/Turbo/thirdparty/glslang/Test/" _BACKTRACE_TRIPLES "E:/Turbo/thirdparty/glslang/CMakeLists.txt;362;add_test;E:/Turbo/thirdparty/glslang/CMakeLists.txt;0;")
subdirs("External")
subdirs("glslang")
subdirs("OGLCompilersDLL")
subdirs("StandAlone")
subdirs("SPIRV")
subdirs("hlsl")
subdirs("gtests")
