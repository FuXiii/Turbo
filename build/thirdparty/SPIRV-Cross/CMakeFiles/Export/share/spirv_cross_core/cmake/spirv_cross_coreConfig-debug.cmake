#----------------------------------------------------------------
# Generated CMake target import file for configuration "debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "spirv-cross-core" for configuration "debug"
set_property(TARGET spirv-cross-core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(spirv-cross-core PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libspirv-cross-cored.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS spirv-cross-core )
list(APPEND _IMPORT_CHECK_FILES_FOR_spirv-cross-core "${_IMPORT_PREFIX}/lib/libspirv-cross-cored.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
