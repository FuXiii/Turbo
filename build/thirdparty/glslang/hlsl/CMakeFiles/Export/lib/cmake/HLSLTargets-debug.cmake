#----------------------------------------------------------------
# Generated CMake target import file for configuration "debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "HLSL" for configuration "debug"
set_property(TARGET HLSL APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(HLSL PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libHLSLd.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS HLSL )
list(APPEND _IMPORT_CHECK_FILES_FOR_HLSL "${_IMPORT_PREFIX}/lib/libHLSLd.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
