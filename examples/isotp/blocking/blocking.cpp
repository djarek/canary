//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

// tag::example[]

#include <canary/interface_index.hpp>
#include <canary/isotp.hpp>
#include <iostream>

// We use this alias to make this program independent of whether ASIO or
// Boost.ASIO is used.
#ifdef CANARY_STANDALONE_ASIO
namespace net = asio;
#else  // CANARY_STANDALONE_ASIO
namespace net = boost::asio;
#endif // CANARY_STANDALONE_ASIO

unsigned const rx_address = 0x100;
unsigned const tx_address = 0x200;

int
main()
{
    net::io_context ioc;
    // Retrieve the interface index from the interface name
    auto const idx = canary::get_interface_index("vcan0");
    // Construct an endpoint using the index and specified RX/TX pair
    auto const ep = canary::isotp::endpoint{idx, rx_address, tx_address};
    // Construct and bind a raw CAN frame socket to the endpoint.
    canary::isotp::socket socket{ioc, ep};

    // Construct an input buffer with a specified size. Note, that the frame
    // will be truncated if the buffer is not large enough.
    std::string buffer(8, '\0');
    // Receive a datagram. `receive` returns the number of bytes transferred, so
    // we can use it to set the string size.
    buffer.resize(socket.receive(net::buffer(buffer)));
    std::cout << "Received an ISO-TP datagram: "
              << " len: " << std::dec << buffer.size() << " :" << buffer
              << '\n';

    std::string const response{"buffer"};
    // Send a datagram. Note that the payload will not be 0-terminated.
    socket.send(net::buffer(response));
}

// end::example[]
