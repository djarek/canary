//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#include <canary/interface_index.hpp>
#include <canary/isotp.hpp>
#include <iostream>

int
main()
{
    boost::asio::io_context ioc;
    // Retrieve the interface index from the interface name
    const auto idx = canary::get_interface_index("vcan0");
    // Construct an endpoint using the index
    auto const ep = canary::isotp::endpoint{idx};
    // Construct and bind a raw CAN frame socket to the endpoint.
    canary::isotp::socket sock{ioc, ep};

    std::string buffer(3, '\0');
    buffer.resize(sock.receive(canary::net::buffer(buffer)));
    std::cout << "Received an ISO-TP datagram: "
              << " len: " << std::dec << buffer.size() << " :" << buffer
              << '\n';
    std::string const str{"buffer"};
    sock.send(canary::net::buffer(str));
}
