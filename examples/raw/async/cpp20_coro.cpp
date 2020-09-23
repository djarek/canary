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
#include <canary/socket_options.hpp>
#include <iostream>

// We use this alias to make this program independent of whether ASIO or
// Boost.ASIO is used.
#ifdef CANARY_STANDALONE_ASIO
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>

namespace net = asio;

#else // CANARY_STANDALONE_ASIO
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>

namespace net = boost::asio;
#endif // CANARY_STANDALONE_ASIO

int
main()
{
    // The io_context used to run the async operation.
    net::io_context ioc;

    // Retrieve the interface index from the interface name
    auto const idx = canary::get_interface_index("vcan0");
    // Construct an endpoint using the index
    auto const ep = canary::raw::endpoint{idx};
    net::co_spawn(
      ioc,
      // Construct and bind a raw CAN frame socket to the endpoint.
      [socket = canary::raw::socket{ioc, ep}]() mutable -> net::awaitable<void> {
          // Set up a filter. This step is optional, by default any frame will
          // be accepted.
          socket.set_option(canary::filter_if_any{{{
            canary::filter{}
              .id_mask(~0xFF)            // Only IDs 0x00-0xFF.
              .remote_transmission(true) // Only remote transmission frames.
              .extended_format(false)    // Only standard format frames.
          }}});

          canary::frame_header header;
          std::array<std::uint8_t, 8> payload;

          while (true)
          {
              // Receives a frame. The buffer sequences point at function
              // local objects. C++20 coroutines guarantee that the compiler
              // will place them in the coroutine activation frame if needed.
              co_await socket.async_receive(
                std::array<net::mutable_buffer, 2>{
                  {net::buffer(&header, sizeof(header)), net::buffer(payload)}},
                net::use_awaitable);
              std::cout << "Received CAN frame, id: " << std::hex << header.id()
                        << " len: " << std::dec << header.payload_length()
                        << '\n';

              header.payload_length(1);
              header.remote_transmission(false);
              payload[0] = 42;
              // Sends a response. Note that we're providing the entire buffer,
              // but the payload length is set to 1 byte, so only 1 byte will be
              // sent.
              co_await socket.async_send(
                std::array<net::const_buffer, 2>{
                  {net::buffer(&header, sizeof(header)), net::buffer(payload)}},
                net::use_awaitable);
          }
      },
      net::detached);

    ioc.run();
}

// end::example[]
