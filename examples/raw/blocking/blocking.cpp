//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

// tag::example[]

#include <canary/frame_header.hpp>
#include <canary/interface_index.hpp>
#include <canary/raw.hpp>
#include <iostream>

// We use this alias to make this program independent of whether ASIO or
// Boost.ASIO is used.
#ifdef CANARY_STANDALONE_ASIO
namespace net = asio;
#else  // CANARY_STANDALONE_ASIO
namespace net = boost::asio;
#endif // CANARY_STANDALONE_ASIO

int
main()
{
    net::io_context ioc;
    // Retrieve the interface index from the interface name
    const auto idx = canary::get_interface_index("vcan0");
    // Construct an endpoint using the index
    auto const ep = canary::raw::endpoint{idx};
    // Construct and bind a raw CAN frame socket to the endpoint.
    canary::raw::socket sock{ioc, ep};
    struct frame_t
    {
        canary::frame_header header;
        std::array<std::uint8_t, 8> payload;
    } frame;

    static_assert(sizeof(frame) == sizeof(frame.header) + sizeof(frame.payload),
                  "Expect no padding in frame_t");

    // Receives a frame, in a blocking way.
    sock.receive(net::buffer(&frame, sizeof(frame)));
    std::cout << "Received CAN frame, id: " << std::hex << frame.header.id()
              << " len: " << std::dec << frame.header.payload_length() << '\n';
    frame = {};
    std::string const str{"hello"};
    frame.header.extended_format(true);
    frame.header.id(0x1EADBEEF);
    frame.header.payload_length(
      net::buffer_copy(net::buffer(frame.payload), net::buffer(str)));
    sock.send(net::buffer(&frame, sizeof(frame)));
}

// end::example[]
