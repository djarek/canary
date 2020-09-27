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
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>

namespace net = asio;

#else // CANARY_STANDALONE_ASIO
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>

namespace net = boost::asio;
#endif // CANARY_STANDALONE_ASIO

unsigned const rx_address = 0x100;
unsigned const tx_address = 0x200;

int
main()
{
    // The io_context used to run the async operation.
    net::io_context ioc;

    // Retrieve the interface index from the interface name
    auto const idx = canary::get_interface_index("vcan0");
    // Construct an endpoint using the index
    auto const ep = canary::isotp::endpoint{idx, rx_address, tx_address};
    net::co_spawn(
      ioc,
      // Construct and bind an ISO-TP CAN socket to the endpoint.
      [socket =
         canary::isotp::socket{ioc, ep}]() mutable -> net::awaitable<void> {
          while (true)
          {
              std::string buffer(8, '\0');
              // Receives a datagram. The input buffer sequence points at a
              // local object. C++20 coroutines guarantee that the compiler will
              // place them in the coroutine activation frame if needed.
              buffer.resize(co_await socket.async_receive(net::buffer(buffer),
                                                          net::use_awaitable));
              // Note: ISO-TP is usually used to transport binary data, however,
              // here we use a string for simplicity.
              std::cout << "Received ISO-TP frame"
                        << " len: " << buffer.size() << " payload: " << buffer
                        << '\n';

              buffer = "Response";
              // Sends a response.
              co_await socket.async_send(net::buffer(buffer),
                                         net::use_awaitable);
          }
      },
      net::detached);

    ioc.run();
}

// end::example[]
