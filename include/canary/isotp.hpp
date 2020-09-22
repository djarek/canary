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

/* tag::reference[]

[#canary_isotp]
=== `canary::isotp`

Defined in header `<canary/isotp.hpp>`

[source, c++]
----
class isotp;
----

Encapsulates flags necessary for creation of ISO-TP sockets.

TIP: You can use `net::basic_datagram_socket<canary::isotp, Executor>` to construct a socket that uses the specified Executor type.

[#canary_isotp_endpoint]
==== `endpoint`
[source, c++]
----
using endpoint = basic_endpoint<isotp>;
----
The endpoint type, which represents an ISO-TP triplet (interface index, RX id, TX ID).

[#canary_isotp_socket]
==== `endpoint`
[source, c++]
----
using socket = net::basic_datagram_socket<isotp>;
----
ISO-TP socket type. Uses a polymorphic executor type.

''''

end::reference[] */
class isotp
{
public:
    int type() const noexcept
    {
        return SOCK_DGRAM;
    }

    int protocol() const noexcept
    {
        return CAN_ISOTP;
    }

    int family() const noexcept
    {
        return AF_CAN;
    }

    using endpoint = basic_endpoint<isotp>;
    using socket = net::basic_datagram_socket<isotp>;
};

} // namespace canary

#endif // CANARY_ISOTP_HPP
