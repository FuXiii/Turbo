#----------------------------------------------------------------
# Generated CMake target import file for configuration "debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "spirv-cross-msl" for configuration "debug"
set_property(TARGET spirv-cross-msl APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(spirv-cross-msl PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libspirv-cross-msld.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS spirv-cross-msl )
list(APPEND _IMPORT_CHECK_FILES_FOR_spirv-cross-msl "${_IMPORT_PREFIX}/lib/libspirv-cross-msld.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
