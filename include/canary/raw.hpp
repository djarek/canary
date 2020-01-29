//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#ifndef CANARY_RAW_HPP
#define CANARY_RAW_HPP

#include <canary/basic_endpoint.hpp>

#ifdef CANARY_STANDALONE_ASIO
#include <asio/basic_raw_socket.hpp>
#else
#include <boost/asio/basic_raw_socket.hpp>
#endif // CANARY_STANDALONE_ASIO

namespace canary
{

/// Encapsulates flags necessary for creation of raw CAN sockets.
class raw
{
public:
    /// Obtain an identifier for the protocol type.
    int type() const noexcept
    {
        return SOCK_RAW;
    }

    /// Obtain an identifier for the protocol.
    int protocol() const noexcept
    {
        return CAN_RAW;
    }

    /// Obtain an identifier for the address family.
    int family() const noexcept
    {
        return AF_CAN;
    }

    /// CAN endpoint type, represents a CAN interface (e.g. vcan0)
    using endpoint = basic_endpoint<raw>;
    /// Raw CAN socket type
    using socket = net::basic_raw_socket<raw>;
};

} // namespace canary

#endif // CANARY_RAW_HPP
