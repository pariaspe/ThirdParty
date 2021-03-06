######################################################################################
# Fakenect Mock Library
######################################################################################
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/fakenect)
add_library (fakenect SHARED fakenect.c)
set_target_properties ( fakenect PROPERTIES
  VERSION ${PROJECT_VER}
  SOVERSION ${PROJECT_APIVER}
  OUTPUT_NAME freenect)
target_link_libraries(fakenect ${MATH_LIB})

install (TARGETS fakenect
  DESTINATION "${PROJECT_LIBRARY_INSTALL_DIR}/fakenect")

add_executable(fakenect-record record.c)
target_link_libraries(fakenect-record freenect ${MATH_LIB})
install (TARGETS fakenect-record
  DESTINATION ${PROJECT_BIN_INSTALL_DIR})

CONFIGURE_FILE("fakenect.sh.in"
  "fakenect.sh"
  IMMEDIATE @ONLY)
install (PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/fakenect.sh
  DESTINATION ${PROJECT_BIN_INSTALL_DIR}
  RENAME fakenect)

install(FILES fakenect.1 fakenect-record.1
  DESTINATION ${PROJECT_MANPAGE_INSTALL_DIR}/man1)
