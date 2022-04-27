#----------------------------------------------------------------
# Generated CMake target import file for configuration "debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "spirv-cross-c" for configuration "debug"
set_property(TARGET spirv-cross-c APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(spirv-cross-c PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libspirv-cross-cd.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS spirv-cross-c )
list(APPEND _IMPORT_CHECK_FILES_FOR_spirv-cross-c "${_IMPORT_PREFIX}/lib/libspirv-cross-cd.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
