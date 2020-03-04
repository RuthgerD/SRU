cmake_minimum_required(VERSION 3.9.0)
project(sru VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")

message(${CMAKE_BUILD_TYPE})

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if (UNIX)
    find_package(Boost REQUIRED)

    if (${CMAKE_BUILD_TYPE} MATCHES "Release")
    message("building in release mode")
    add_compile_options("-O3")
    add_compile_options("-mtune=native")
    add_compile_options("-march=native")
    add_compile_options("-fno-exceptions")
else ()
    message("building in debug mode")
endif ()
add_compile_options("-fno-rtti")
add_compile_options("-Wshadow")
else()
    find_package(BOOST REQUIRED)

    set(VCPKG_LIBRARY_LINKAGE static)
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

add_compile_options("/MT")
add_compile_options("/GR-")
add_compile_options("/EHsc-")
add_compile_options("/GL")
add_compile_definitions(NOMINMAX)
endif()

if (DEV_BUILT)
    add_compile_definitions(DEV_BUILT)
endif ()

find_package(re2 REQUIRED)
find_package (Threads REQUIRED)

include_directories(${BOOST_INCLUDE_DIRS} ${RE2_INCLUDE_DIRS} thirdparty/ctre/include thirdparty/rapidjson/include thirdparty/gsl/include thirdparty/fmt/include thirdparty include)

add_compile_definitions(RAPIDJSON_HAS_STDSTRING)
file(GLOB regexs
    "src/util/regexs/*.cpp"
)
add_executable(sru main.cpp src/pdf/string_obj.cpp src/pdf/pdf_cluster.cpp src/pdf/pdf_file.cpp src/pdf/pdf_page.cpp src/pdf/object_config.cpp src/pdf/anchor_config.cpp src/pdf/page_config.cpp src/util/util.cpp src/util/qpdf_binding.cpp src/util/regex.cpp src/pdf/calc_config.cpp src/pdf/global_settings.cpp src/util/re_accel.cpp ${regexs})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(sru ${BOOST_LIBRARIES} Boost::headers ${RE2_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
