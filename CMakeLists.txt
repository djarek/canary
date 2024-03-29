#
# Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/djarek/canary
#

cmake_minimum_required(VERSION 3.8)
project(canary VERSION 2 LANGUAGES CXX)

# TODO: Don't require Boost if ASIO standalone option is required
find_package(Boost 1.70
             COMPONENTS
                system
             REQUIRED)

find_package (Threads)


add_library(canary INTERFACE)
add_library(canary::canary ALIAS canary)

target_include_directories(canary INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

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
endif()

option(CANARY_BUILD_EXAMPLES "Build examples." OFF)
option(CANARY_BUILD_COROUTINE_EXAMPLES "Build examples using C++20 coroutines." OFF)
if(CANARY_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


include(GNUInstallDirs)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
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
            "${CMAKE_CURRENT_BINARY_DIR}/canaryConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/canary)

install(EXPORT canaryTargets
        FILE canaryTargets.cmake
        NAMESPACE canary::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/canary)

option(CANARY_BUILD_DOCS "Build canary documentation." OFF)
if (CANARY_BUILD_DOCS)
    file(GLOB CANARY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/canary/*.hpp")
    find_package(standardese REQUIRED)
    standardese_generate(canary_docs
        INCLUDE_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${Boost_INCLUDE_DIR}
        MACRO_DEFINITION
            CANARY_SEPARATE_COMPILATION
        INPUT ${CANARY_HEADERS})
endif()
