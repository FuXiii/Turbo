#----------------------------------------------------------------
# Generated CMake target import file for configuration "debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "spirv-cross-cpp" for configuration "debug"
set_property(TARGET spirv-cross-cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(spirv-cross-cpp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libspirv-cross-cppd.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS spirv-cross-cpp )
list(APPEND _IMPORT_CHECK_FILES_FOR_spirv-cross-cpp "${_IMPORT_PREFIX}/lib/libspirv-cross-cppd.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
