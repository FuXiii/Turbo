#----------------------------------------------------------------
# Generated CMake target import file for configuration "debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "glslang" for configuration "debug"
set_property(TARGET glslang APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(glslang PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libglslangd.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS glslang )
list(APPEND _IMPORT_CHECK_FILES_FOR_glslang "${_IMPORT_PREFIX}/lib/libglslangd.a" )

# Import target "MachineIndependent" for configuration "debug"
set_property(TARGET MachineIndependent APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(MachineIndependent PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libMachineIndependentd.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS MachineIndependent )
list(APPEND _IMPORT_CHECK_FILES_FOR_MachineIndependent "${_IMPORT_PREFIX}/lib/libMachineIndependentd.a" )

# Import target "GenericCodeGen" for configuration "debug"
set_property(TARGET GenericCodeGen APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(GenericCodeGen PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libGenericCodeGend.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS GenericCodeGen )
list(APPEND _IMPORT_CHECK_FILES_FOR_GenericCodeGen "${_IMPORT_PREFIX}/lib/libGenericCodeGend.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
