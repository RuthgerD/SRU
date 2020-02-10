cmake_minimum_required(VERSION 3.0.0)
project(cppthings VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")

if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
add_compile_options("-O3")
add_compile_options("-mtune=native")
add_compile_options("-march=native")
add_compile_options("-fno-except")

endif()
add_compile_options("-fno-rtti")

find_package(Boost 1.66 REQUIRED COMPONENTS program_options regex)
find_package (Threads)
find_package (RE2)
find_package(fmt)

if (Boost_FOUND AND Threads_FOUND AND RE2_FOUND AND fmt_FOUND)
include_directories(${Boost_INCLUDE_DIRS} ${RE2_INCLUDE_DIR} thirdparty/ctre/include thirdparty/rapidjson/include thirdparty/gsl/include thirdparty include)

add_compile_definitions(RAPIDJSON_HAS_STDSTRING)
add_library(re_accel src/util/re_accel.h src/util/re_accel.cpp)
add_executable(cppthings main.cpp src/pdf_structures/string_obj.cpp src/pdf_structures/pdf_cluster.cpp src/pdf_structures/pdf_file.cpp src/pdf_structures/pdf_page.cpp src/pdf_structures/object_config.cpp src/pdf_structures/anchor_config.cpp src/pdf_structures/page_config.cpp src/util/util.cpp src/util/qpdf_binding.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
target_link_libraries(cppthings ${Boost_LIBRARIES} ${RE2_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} fmt::fmt ${CMAKE_SOURCE_DIR}/cmake-build-release/libre_accel.a)

endif()
