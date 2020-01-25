//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

// Test if header is self-contained
#include <canary/interface_index.hpp>

#include <boost/core/lightweight_test.hpp>

namespace
{

void
check_valid_interfaces()
{
    canary::get_interface_index("vcan0");
    canary::get_interface_index("vcan1");

    canary::error_code ec;
    canary::get_interface_index("vcan0", ec);
    BOOST_TEST_NOT(ec);

    canary::get_interface_index("vcan1", ec);
    BOOST_TEST_NOT(ec);
}

void
check_nonexistant_interfaces()
{
    BOOST_TEST_THROWS(
      canary::get_interface_index("doesnotexistdefinitelyvcan0"),
      canary::system_error);
    canary::error_code ec;
    canary::get_interface_index("doesnotexistdefinitelyvcan0", ec);
    BOOST_TEST(ec == std::errc::no_such_device);
}

} // namespace

int
main()
{
    check_valid_interfaces();
    check_nonexistant_interfaces();
    return boost::report_errors();
}
