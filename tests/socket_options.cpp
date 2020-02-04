//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

// Test if header is self-contained
#include <canary/socket_options.hpp>

#include <boost/core/lightweight_test.hpp>
#include <canary/frame_header.hpp>
#include <canary/interface_index.hpp>
#include <canary/raw.hpp>

namespace
{

void
test_can_fd()
{
    canary::net::io_context ctx{1};
    canary::raw::socket sock1{
      ctx, canary::raw::endpoint{canary::get_interface_index("vcan0")}};
    canary::raw::socket sock2{
      ctx, canary::raw::endpoint{canary::get_interface_index("vcan0")}};

    sock1.set_option(canary::flexible_data_rate{true});
    sock2.set_option(canary::flexible_data_rate{true});

    struct frame
    {
        canary::frame_header fh;
        std::array<char, 64> data{};
    } fr;

    fr.fh.payload_length(fr.data.size());
    fr.fh.id(0xFF);

    sock1.send(canary::net::buffer(&fr, sizeof(fr)));
    sock2.receive(canary::net::buffer(&fr, sizeof(fr)));
}

} // namespace

int
main()
{
    test_can_fd();
    return boost::report_errors();
}
