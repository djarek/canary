#
# Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/djarek/canary
#

add_executable(raw_blocking raw/blocking/blocking.cpp)
target_link_libraries(raw_blocking canary::canary)

if(${CANARY_BUILD_COROUTINE_EXAMPLES})
    add_executable(async_cpp20_coro raw/async/cpp20_coro.cpp)
    target_link_libraries(async_cpp20_coro PUBLIC canary::canary)
    target_compile_features(async_cpp20_coro PUBLIC cxx_std_20)
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        target_link_libraries(async_cpp20_coro PUBLIC -lc++abi -stdlib=libc++)
        target_compile_options(async_cpp20_coro PUBLIC -stdlib=libc++)
    endif ()
    target_compile_definitions(async_cpp20_coro PUBLIC BOOST_ASIO_DISABLE_CONCEPTS)
endif()
add_executable(isotp_blocking isotp/blocking/blocking.cpp)
target_link_libraries(isotp_blocking canary::canary)
