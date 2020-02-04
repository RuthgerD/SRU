FIND_PATH(SYSTEM_RE2_INCLUDE_DIR re2/re2.h)
IF (SYSTEM_RE2_INCLUDE_DIR)
  MESSAGE(STATUS "Found RE2 include dir")
  FIND_LIBRARY(SYSTEM_RE2_LIBRARY re2)
  IF (SYSTEM_RE2_LIBRARY)
    MESSAGE(STATUS "Found RE2 library")
    SET(RE2_INCLUDE_DIR ${SYSTEM_RE2_INCLUDE_DIR})
    SET(RE2_LIBRARY ${SYSTEM_RE2_LIBRARY})
    SET(RE2_FOUND TRUE)
  ELSE ()
    MESSAGE(ERROR "Found RE2 headers, but not the library")
  ENDIF ()
ELSE ()
  MESSAGE(STATUS "Did not find system RE2")
ENDIF ()