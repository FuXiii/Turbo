#----------------------------------------------------------------
# Generated CMake target import file for configuration "debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "glslang-default-resource-limits" for configuration "debug"
set_property(TARGET glslang-default-resource-limits APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(glslang-default-resource-limits PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libglslang-default-resource-limitsd.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS glslang-default-resource-limits )
list(APPEND _IMPORT_CHECK_FILES_FOR_glslang-default-resource-limits "${_IMPORT_PREFIX}/lib/libglslang-default-resource-limitsd.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
