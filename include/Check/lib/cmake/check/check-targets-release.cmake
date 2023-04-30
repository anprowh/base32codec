#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Check::check" for configuration "Release"
set_property(TARGET Check::check APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Check::check PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libcheck.a"
  )

list(APPEND _cmake_import_check_targets Check::check )
list(APPEND _cmake_import_check_files_for_Check::check "${_IMPORT_PREFIX}/lib/libcheck.a" )

# Import target "Check::checkShared" for configuration "Release"
set_property(TARGET Check::checkShared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Check::checkShared PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libcheck.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libcheck.dll"
  )

list(APPEND _cmake_import_check_targets Check::checkShared )
list(APPEND _cmake_import_check_files_for_Check::checkShared "${_IMPORT_PREFIX}/lib/libcheck.dll.a" "${_IMPORT_PREFIX}/bin/libcheck.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
