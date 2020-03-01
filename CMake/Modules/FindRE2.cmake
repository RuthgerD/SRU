FIND_PATH(SYSTEM_RE2_INCLUDE_DIR re2/re2.h)
IF (SYSTEM_RE2_INCLUDE_DIR)
  FIND_LIBRARY(SYSTEM_RE2_LIBRARY re2)
  IF (SYSTEM_RE2_LIBRARY)
    SET(RE2_INCLUDE_DIR ${SYSTEM_RE2_INCLUDE_DIR})
    SET(RE2_LIBRARY ${SYSTEM_RE2_LIBRARY})
    SET(RE2_FOUND TRUE)
  ELSE ()
    MESSAGE(ERROR "Found RE2 headers, but not the library")
  ENDIF ()
ELSE ()
  MESSAGE(STATUS "Did not find system RE2, compiling from source...")
  SET(RE2_FOUND TRUE)
  SET(RE2_INCLUDE_DIR ${CMAKE_BINARY_DIR}/re2)
  SET(RE2_LIBRARY ${CMAKE_BINARY_DIR}/re2/obj/so/libre2.so)
  add_custom_target(external_re2 ALL
          COMMAND cd ${CMAKE_BINARY_DIR} && [ -f ${RE2_LIBRARIES} ] || git clone https://github.com/google/re2.git
          COMMAND cd ${CMAKE_BINARY_DIR}/re2 && make
          COMMAND echo ${CMAKE_BINARY_DIR}/re2
          )
  add_library(re2compil SHARED IMPORTED)
  add_dependencies(re2compil external_re2)
ENDIF ()
