#----------------------------------------------------------------
# Generated CMake target import file for configuration "debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SPIRV" for configuration "debug"
set_property(TARGET SPIRV APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SPIRV PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libSPIRVd.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SPIRV )
list(APPEND _IMPORT_CHECK_FILES_FOR_SPIRV "${_IMPORT_PREFIX}/lib/libSPIRVd.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
