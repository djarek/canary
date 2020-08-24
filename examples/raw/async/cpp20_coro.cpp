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
#include <canary/socket_options.hpp>
#include <iostream>

#ifdef CANARY_STANDALONE_ASIO
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#else // CANARY_STANDALONE_ASIO
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#endif // CANARY_STANDALONE_ASIO

int
main()
{
    canary::net::io_context ioc;
    canary::net::co_spawn(
      ioc,
      [&ioc]() -> canary::net::awaitable<void> {
          // Retrieve the interface index from the interface name
          const auto idx = canary::get_interface_index("vcan0");
          // Construct an endpoint using the index
          auto const ep = canary::raw::endpoint{idx};
          // Construct and bind a raw CAN frame socket to the endpoint.
          canary::raw::socket sock{ioc, ep};
          sock.set_option(canary::filter_if_any{{{
            canary::filter{}
              .id_mask(~0xFF)            // Only IDs 0x00-0xFF.
              .remote_transmission(true) // Only remote transmission frames.
              .extended_format(false)    // Only standard format frames.
          }}});

          struct frame
          {
              canary::frame_header header;
              std::array<std::uint8_t, 8> payload;
          } f;

          while (true)
          {
              co_await sock.async_receive(canary::net::buffer(&f, sizeof(f)),
                                          canary::net::use_awaitable);
              std::cout << "Received CAN frame, id: " << std::hex
                        << f.header.id() << " len: " << std::dec
                        << f.header.payload_length() << '\n';
          }
      },
      canary::net::detached);

    ioc.run();
}
