#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "xlnt::xlnt" for configuration "MinSizeRel"
set_property(TARGET xlnt::xlnt APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(xlnt::xlnt PROPERTIES
  IMPORTED_IMPLIB_MINSIZEREL "${_IMPORT_PREFIX}/lib/xlnt.lib"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/bin/xlnt.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS xlnt::xlnt )
list(APPEND _IMPORT_CHECK_FILES_FOR_xlnt::xlnt "${_IMPORT_PREFIX}/lib/xlnt.lib" "${_IMPORT_PREFIX}/bin/xlnt.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
