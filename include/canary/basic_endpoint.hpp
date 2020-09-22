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

#include <canary/detail/config.hpp>

#ifdef CANARY_STANDALONE_ASIO
#include <asio/error.hpp>
#else
#include <boost/asio/error.hpp>
#endif // CANARY_STANDALONE_ASIO

#include <linux/can/raw.h>

namespace canary
{

/* tag::reference[]

[#canary_basic_endpoint]
=== `canary::basic_endpoint`

Defined in header `<canary/basic_endpoint.hpp>`

[source, c++]
----
template<class Protocol>
class basic_endpoint;
----

Describes an endpoint for a CAN bus interface that can be bound to a CAN socket.

[#canary_basic_endpoint_protocol_type]
==== `protocol_type`
[source, c++]
----
using protocol_type = Protocol;
----
The protocol type associated with the endpoint.

[#canary_basic_endpoint_data_type]
==== `data_type`
[source, c++]
----
using data_type = IMPLEMENTATION_DEFINED;
----
Underlying type used to store the native endpoint information.

[#canary_basic_endpoint_basic_endpoint]
==== (constructor)
[source, c++]
----
basic_endpoint();                                                                 <1>
basic_endpoint(unsigned int interface_index);                                     <2>
basic_endpoint(unsigned int interface_index, std::uint32_t rx, std::uint32_t tx); <3>
----
Constructs the endpoint.

<1> Default constructor. The constructed endpoint represents any interface. A
socket bound to this endpoint will receive frames from all interfaces.

<2> Constructs an endpoint that represents the specified CAN interface index.

<3> Constructs an endpoint that represents the specified ISO-TP rx/tx pair on a specified CAN interface index.

[#canary_basic_endpoint_protocol]
==== `protocol()`
[source, c++]
----
protocol_type protocol() const noexcept;
----

Constructs an object of the protocol type associated with this endpoint.

[#canary_basic_endpoint_interface_index]
==== `interface_index()`

[source, c++]
----
unsigned int interface_index() const noexcept;
----

Returns the interface index that this endpoint represents.


[#canary_basic_endpoint_data]
==== `data()`

[source, c++]
----
data_type* data() noexcept;
----

Returns the underlying native endpoint type.

[#canary_basic_endpoint_size]
==== `size()`

[source, c++]
----
std::size_t size() const noexcept;
----

Returns the size of the underlying native endpoint type.


[#canary_basic_endpoint_capacity]
==== `capacity()`

[source, c++]
----
std::size_t capacity() const noexcept;
----
Returns the capacity of the underlying native endpoint type.


[#canary_basic_endpoint_resize]
==== `resize()`

[source, c++]
----
void resize(std::size_t n);
----
Sets the size of the underlying native endpoint type.

''''

end::reference[] */

/// Describes an endpoint for a CAN bus interface that can be bound to a CAN
/// socket.
template<class Protocol>
class basic_endpoint
{
public:
    using protocol_type = Protocol;
    using data_type = ::sockaddr;

    basic_endpoint()
      : basic_endpoint{0}
    {
    }

    basic_endpoint(unsigned int interface_index)
    {
        addr_.can_ifindex = static_cast<int>(interface_index);
        addr_.can_family =
          static_cast<unsigned short>(protocol_type{}.family());
    }

    basic_endpoint(unsigned int interface_index, std::uint32_t rx, std::uint32_t tx)
    {
        addr_.can_ifindex = static_cast<int>(interface_index);
        addr_.can_family =
          static_cast<unsigned short>(protocol_type{}.family());
        addr_.can_addr.tp.rx_id = rx;
        addr_.can_addr.tp.tx_id = tx;
    }

    protocol_type protocol() const noexcept
    {
        return protocol_type{};
    }

    unsigned int interface_index() const noexcept
    {
        return static_cast<unsigned int>(addr_.can_ifindex);
    }

    data_type* data() noexcept
    {
        return reinterpret_cast<::sockaddr*>(&addr_);
    }

    data_type const* data() const noexcept
    {
        return reinterpret_cast<::sockaddr const*>(&addr_);
    }

    std::size_t size() const noexcept
    {
        return sizeof(addr_);
    }

    std::size_t capacity() const noexcept
    {
        return sizeof(addr_);
    }

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
