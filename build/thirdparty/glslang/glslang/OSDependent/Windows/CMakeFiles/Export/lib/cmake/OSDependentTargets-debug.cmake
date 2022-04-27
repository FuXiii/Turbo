#----------------------------------------------------------------
# Generated CMake target import file for configuration "debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "OSDependent" for configuration "debug"
set_property(TARGET OSDependent APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(OSDependent PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libOSDependentd.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS OSDependent )
list(APPEND _IMPORT_CHECK_FILES_FOR_OSDependent "${_IMPORT_PREFIX}/lib/libOSDependentd.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
