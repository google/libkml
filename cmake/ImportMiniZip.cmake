find_package(MiniZip)
mark_as_advanced(MINIZIP_INCLUDE_DIR)
mark_as_advanced(MINIZIP_LIBRARY)
if(MINIZIP_FOUND)
  set(HAVE_SYSTEM_MINIZIP TRUE)
else()
  set(HAVE_SYSTEM_MINIZIP FALSE)
endif()