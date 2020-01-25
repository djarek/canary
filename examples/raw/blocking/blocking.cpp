//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#include <canary/interface_index.hpp>
#include <canary/raw.hpp>
#include <canary/static_frame.hpp>
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
    canary::static_frame frame;
    sock.receive(frame.buffers());
    std::cout << "Received CAN frame, id: " << std::hex << frame.id()
              << " len: " << std::dec << frame.data().size() << '\n';
    frame = {};
    std::string const str{"hello"};
    frame.is_extended_format(true);
    frame.id(0x1EADBEEF);
    frame.resize(str.size());
    boost::asio::buffer_copy(frame.data(), boost::asio::buffer(str));
    sock.send(frame.buffers());
}
