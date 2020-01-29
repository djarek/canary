//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#include <canary/frame_header.hpp>
#include <canary/interface_index.hpp>
#include <canary/raw.hpp>
#include <iostream>

int
main()
{
    boost::asio::io_context ioc;
    // Retrieve the interface index from the interface name
    const auto idx = canary::get_interface_index("vcan0");
    // Construct an endpoint using the index
    auto const ep = canary::raw::endpoint{idx};
    // Construct and bind a raw CAN frame socket to the endpoint.
    canary::raw::socket sock{ioc, ep};
    struct frame
    {
        canary::frame_header header;
        std::array<std::uint8_t, 8> payload;
    } f;

    sock.receive(canary::net::buffer(&f, sizeof(f)));
    std::cout << "Received CAN frame, id: " << std::hex << f.header.id()
              << " len: " << std::dec << f.header.payload_length() << '\n';
    f = {};
    std::string const str{"hello"};
    f.header.extended_format(true);
    f.header.id(0x1EADBEEF);
    f.header.payload_length(boost::asio::buffer_copy(
      canary::net::buffer(f.payload), boost::asio::buffer(str)));
    sock.send(canary::net::buffer(&f, sizeof(f)));
}
