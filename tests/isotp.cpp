//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

// Test if header is self-contained
#include <canary/isotp.hpp>

#include <boost/core/lightweight_test.hpp>
#include <canary/interface_index.hpp>

namespace
{

namespace net = canary::net;

void
test_sync_send()
{
    net::io_context ioc{1};
    canary::isotp::socket sock1{
      ioc,
      canary::isotp::endpoint{canary::get_interface_index("vcan0"), 0x1, 0x2}};
    canary::isotp::socket sock2{
      ioc,
      canary::isotp::endpoint{canary::get_interface_index("vcan0"), 0x2, 0x1}};
    sock1.send(net::buffer("abc"));
    std::string str(3, '\0');
    sock2.receive(net::buffer(str));
    BOOST_TEST_EQ("abc", str);
}

} // namespace

int
main()
{
    test_sync_send();
    return boost::report_errors();
}
