# Install script for directory: E:/Turbo/thirdparty/glslang/SPIRV

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "F:/llvm-mingw-20211002-msvcrt-x86_64/bin/llvm-objdump.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/Turbo/build/thirdparty/glslang/SPIRV/libSPVRemapperd.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/Turbo/build/thirdparty/glslang/SPIRV/libSPIRVd.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPVRemapperTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPVRemapperTargets.cmake"
         "E:/Turbo/build/thirdparty/glslang/SPIRV/CMakeFiles/Export/lib/cmake/SPVRemapperTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPVRemapperTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPVRemapperTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "E:/Turbo/build/thirdparty/glslang/SPIRV/CMakeFiles/Export/lib/cmake/SPVRemapperTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "E:/Turbo/build/thirdparty/glslang/SPIRV/CMakeFiles/Export/lib/cmake/SPVRemapperTargets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPIRVTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPIRVTargets.cmake"
         "E:/Turbo/build/thirdparty/glslang/SPIRV/CMakeFiles/Export/lib/cmake/SPIRVTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPIRVTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SPIRVTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "E:/Turbo/build/thirdparty/glslang/SPIRV/CMakeFiles/Export/lib/cmake/SPIRVTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "E:/Turbo/build/thirdparty/glslang/SPIRV/CMakeFiles/Export/lib/cmake/SPIRVTargets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/glslang/SPIRV" TYPE FILE FILES
    "E:/Turbo/thirdparty/glslang/SPIRV/bitutils.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/spirv.hpp"
    "E:/Turbo/thirdparty/glslang/SPIRV/GLSL.std.450.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/GLSL.ext.EXT.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/GLSL.ext.KHR.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/GlslangToSpv.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/hex_float.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/Logger.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/SpvBuilder.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/spvIR.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/doc.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/SpvTools.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/disassemble.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/GLSL.ext.AMD.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/GLSL.ext.NV.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/NonSemanticDebugPrintf.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/SPVRemapper.h"
    "E:/Turbo/thirdparty/glslang/SPIRV/doc.h"
    )
endif()

