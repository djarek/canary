//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#ifndef CANARY_ISOTP_HPP
#define CANARY_ISOTP_HPP

#include <canary/basic_endpoint.hpp>

#ifdef CANARY_STANDALONE_ASIO
#include <asio/basic_datagram_socket.hpp>
#else
#include <boost/asio/basic_datagram_socket.hpp>
#endif // CANARY_STANDALONE_ASIO

namespace canary
{

// /// Encapsulates flags necessary for creation of ISO-TP sockets.
class isotp
{
public:
    /// Obtain an identifier for the protocol type.
    int type() const noexcept
    {
        return SOCK_DGRAM;
    }

    /// Obtain an identifier for the protocol.
    int protocol() const noexcept
    {
        return CAN_ISOTP;
    }

    /// Obtain an identifier for the address family.
    int family() const noexcept
    {
        return AF_CAN;
    }

    /// CAN endpoint type, represents a CAN interface (e.g. vcan0)
    using endpoint = basic_endpoint<isotp>;
    /// Raw CAN socket type
    using socket = net::basic_datagram_socket<isotp>;
};

} // namespace canary

#endif // CANARY_ISOTP_HPP
