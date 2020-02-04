//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#ifndef CANARY_BASIC_ENDPOINT_HPP
#define CANARY_BASIC_ENDPOINT_HPP

#ifdef CANARY_STANDALONE_ASIO
#include <asio/error.hpp>
#else
#include <boost/asio/error.hpp>
#endif // CANARY_STANDALONE_ASIO

#include <canary/detail/config.hpp>
#include <linux/can/raw.h>

namespace canary
{

/// Describes an endpoint for a CAN bus interface that can be bound to a CAN
/// socket.
template<class Protocol>
class basic_endpoint
{
public:
    /// The protocol type associated with the endpoint.
    using protocol_type = Protocol;

    /// Underlying type used to store the endpoint information.
    using data_type = ::sockaddr;

    /// Default constructor
    basic_endpoint()
      : basic_endpoint{0}
    {
    }

    /// Construct an endpoint that represents a particular CAN interface index.
    basic_endpoint(unsigned int interface_index)
    {
        addr_.can_ifindex = static_cast<int>(interface_index);
        addr_.can_family =
          static_cast<unsigned short>(protocol_type{}.family());
    }

    /// Constructs an object of the protocol type associated with this endpoint.
    protocol_type protocol() const noexcept
    {
        return protocol_type{};
    }

    /// Returns the interface index that this endpoint represents.
    unsigned int interface_index() const noexcept
    {
        return static_cast<unsigned int>(addr_.can_ifindex);
    }

    /// Get the underlying endpoint in the native type.
    data_type* data() noexcept
    {
        return reinterpret_cast<::sockaddr*>(&addr_);
    }

    /// Get the underlying endpoint in the native type.
    data_type const* data() const noexcept
    {
        return reinterpret_cast<::sockaddr const*>(&addr_);
    }

    /// Get the underlying size of the endpoint in the native type.
    std::size_t size() const noexcept
    {
        return sizeof(addr_);
    }

    /// Get the underlying capacity of the endpoint in the native type.
    std::size_t capacity() const noexcept
    {
        return sizeof(addr_);
    }

    /// Set the underlying size of the endpoint in the native type.
    void resize(std::size_t n)
    {
        if (n > capacity())
        {
            error_code ec{net::error::invalid_argument};
            throw system_error{ec};
        }
    }

private:
    ::sockaddr_can addr_{};
};

} // namespace canary

#endif // CANARY_BASIC_ENDPOINT_HPP
