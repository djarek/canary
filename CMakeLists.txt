#
# Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/djarek/canary
#

# option(CMake_RUN_CLANG_TIDY "Run clang-tidy with the compiler." ON)
# if(CMake_RUN_CLANG_TIDY)
#   if(CMake_SOURCE_DIR STREQUAL CMake_BINARY_DIR)
#     message(FATAL_ERROR "CMake_RUN_CLANG_TIDY requires an out-of-source build!")
#   endif()
#   find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)
#   if(NOT CLANG_TIDY_COMMAND)
#     message(WARNING "CMake_RUN_CLANG_TIDY is ON but clang-tidy is not found!")
#     set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
#   else()
#     set(CLANG_TIDY_CHECKS "-*,modernize-*")
#     set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND};-header-filter='${CMAKE_SOURCE_DIR}/include/*'" CACHE STRING "" FORCE)
#   endif()

#   # Create a preprocessor definition that depends on .clang-tidy content so
#   # the compile command will change when .clang-tidy changes.  This ensures
#   # that a subsequent build re-runs clang-tidy on all sources even if they
#   # do not otherwise need to be recompiled.  Nothing actually uses this
#   # definition.  We add it to targets on which we run clang-tidy just to
#   # get the build dependency on the .clang-tidy file.
#   file(SHA1 ${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy clang_tidy_sha1)
#   set(CLANG_TIDY_DEFINITIONS "CLANG_TIDY_SHA1=${clang_tidy_sha1}")
#   unset(clang_tidy_sha1)

# endif()
# configure_file(.clang-tidy .clang-tidy COPYONLY)

cmake_minimum_required(VERSION 3.8)
project(canary VERSION 1 LANGUAGES CXX)

# TODO: This should be in a toolchain file
option(CANARY_SANITIZE "Build canary tests and examples with address & undefined sanitization enabled" OFF)
if (CANARY_SANITIZE)
    message(STATUS "canary: address & undefined sanitizers enabled")
    add_compile_options(-fsanitize=address,undefined)
    link_libraries(-fsanitize=address,undefined)
endif()

# TODO: Don't require Boost if ASIO standalone option is required
find_package(Boost 1.70
             COMPONENTS
                system
             REQUIRED)

find_package (Threads)


add_library(canary INTERFACE)
add_library(canary::canary ALIAS canary)

target_include_directories(canary INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(
    canary
    INTERFACE
        Boost::system
        Threads::Threads)

target_compile_features(canary INTERFACE cxx_std_11)

include(CTest)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
    add_subdirectory(examples)
endif()


include(GNUInstallDirs)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
            PATTERN "*.hpp"
            PATTERN "*.ipp")

install(TARGETS canary
        EXPORT canaryTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(canary_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "canaryConfigVersion.cmake"
    COMPATIBILITY AnyNewerVersion)

set(CMAKE_SIZEOF_VOID_P ${canary_SIZEOF_VOID_P})
unset(canary_SIZEOF_VOID_P)

install(FILES
            "canaryConfig.cmake"
            "${CMAKE_BINARY_DIR}/canaryConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/canary)

install(EXPORT canaryTargets
        FILE canaryTargets.cmake
        NAMESPACE canary::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/canary)
