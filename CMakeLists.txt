cmake_minimum_required(VERSION 3.0.0)
project(sru VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")

if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
add_compile_options("-O3")
add_compile_options("-mtune=native")
add_compile_options("-march=native")
add_compile_options("-fno-except")
add_compile_options("-flto")

endif()
add_compile_options("-fno-rtti")
add_compile_options("-Wpedantic")
add_compile_options("-Wshadow")

if (DEV_BUILT)
add_compile_definitions(DEV_BUILT)
endif()

find_package(Boost 1.66 REQUIRED COMPONENTS program_options regex)
find_package (Threads)
find_package (RE2)

if (Boost_FOUND AND Threads_FOUND AND RE2_FOUND)
include_directories(${Boost_INCLUDE_DIRS} ${RE2_INCLUDE_DIR} thirdparty/ctre/include thirdparty/rapidjson/include thirdparty/gsl/include thirdparty/fmt/include thirdparty include)

add_compile_definitions(RAPIDJSON_HAS_STDSTRING)
add_library(re_accel src/util/re_accel.h src/util/re_accel.cpp)
add_executable(sru main.cpp src/pdf/string_obj.cpp src/pdf/pdf_cluster.cpp src/pdf/pdf_file.cpp src/pdf/pdf_page.cpp src/pdf/object_config.cpp src/pdf/anchor_config.cpp src/pdf/page_config.cpp src/util/util.cpp src/util/qpdf_binding.cpp src/util/regex.cpp src/pdf/calc_config.cpp src/pdf/global_settings.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
target_link_libraries(sru ${Boost_LIBRARIES} ${RE2_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_SOURCE_DIR}/build/release/libre_accel.a)

endif()
