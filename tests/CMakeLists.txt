#
# Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/djarek/canary
#

function(canary_add_test test_name)
    add_executable(${test_name} "${test_name}.cpp")
    target_link_libraries(${test_name} PRIVATE canary::canary)
    add_test("${test_name}_test" ${test_name})
endfunction(canary_add_test)

canary_add_test(basic_endpoint)
canary_add_test(frame_header)
canary_add_test(interface_index)
canary_add_test(raw)
canary_add_test(socket_options)
