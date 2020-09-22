//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#ifndef CANARY_FRAME_HEADER_HPP
#define CANARY_FRAME_HEADER_HPP

#include <boost/asio/buffer.hpp>

#include <cassert>
#include <cstdint>

namespace canary
{

/* tag::reference[]

[#canary_frame_header]
=== `canary::frame_header`

Defined in header `<canary/frame_header.hpp>`

[source, c++]
----
class frame_header;
----

A CAN frame header, used for transmission of both standard and flexible data
rate frames.

[#canary_frame_header_frame_header]
==== (constructor)
[source, c++]
----
frame_header() = default;
----
Default constructor, performs zero-initialization.

[#canary_id]
==== `id()`
[source, c++]
----
void id(std::uint32_t value);      <1>
std::uint32_t id() const noexcept; <2>
----
Accessors for the CAN ID of the frame. CAN IDs are 29-bit integers for
extended-format frames and 11-bit for standard-format frames.

<1> Sets the value of the CAN ID. The 3 most significant bits of the value are
ignored.
<2> Returns the value of the CAN ID.

[#canary_error]
==== `error()`
[source, c++]
----
void error(bool value);      <1>
bool error() const noexcept; <2>
----
Accessors for the error flag of the frame.

<1> Sets the value of the error flag. True indicates the frame is an error
frame.
<2> Returns the value of the error flag.

[#canary_remote_transmission]
==== `remote_transmission()`
[source, c++]
----
void remote_transmission(bool value);      <1>
bool remote_transmission() const noexcept; <2>
----
Accessors for the remote transmission flag of the frame. A remote transmission
request indicates to the receiver that the value of the CAN ID should be sent.

<1> Sets the value of the remote transmission flag. True indicates the frame is
a remote transmission request.
<2> Returns the value of the remote transmission flag.

[#canary_extended_format]
==== `extended_format()`
[source, c++]
----
void extended_format(bool value);      <1>
bool extended_format() const noexcept; <2>
----
Accessors for the extended format flag of the frame. An extended format frame
uses 29 bit CAN IDs in contrast to a standard format frame, which uses 11 bit
CAN IDs.

<1> Sets the value of the extended format flag. True indicates the frame uses extended format CAN IDs.
<2> Returns the value of the extended format flag.

WARNING: The implementation may truncate the CAN ID if extended frame format is
disabled and the ID is larger than 0x7FF.


[#canary_payload_length]
==== `payload_length()`
[source, c++]
----
void payload_length(bool value);      <1>
bool payload_length() const noexcept; <2>
----
Accessors for the payload_length of the frame. Payloads of up to 8 bytes are
allowed for standard data rate sockets. If the flexible data rate option is
enabled, payloads of up to 64 bytes are allowed.

<1> Sets the the payload length.
<2> Returns the payload length.


''''

end::reference[] */

class frame_header
{
public:
    frame_header() = default;

    void id(std::uint32_t value)
    {
        id_ = ((value & id_mask) | (id_ & ~id_mask));
    }

    std::uint32_t id() const noexcept
    {
        return (id_ & id_mask);
    }

    void error(bool value)
    {
        if (value)
        {
            id_ |= error_flag;
        }
        else
        {
            id_ &= ~error_flag;
        }
    }

    bool error() const noexcept
    {
        return (id_ & error_flag);
    }

    void remote_transmission(bool value)
    {
        if (value)
        {
            id_ |= rtr_flag;
        }
        else
        {
            id_ &= ~rtr_flag;
        }
    }

    bool remote_transmission() const noexcept
    {
        return (id_ & rtr_flag);
    }

    void extended_format(bool value)
    {
        if (value)
        {
            id_ |= format_flag;
        }
        else
        {
            id_ &= ~format_flag;
        }
    }

    bool extended_format() const noexcept
    {
        return (id_ & format_flag);
    }

    void payload_length(std::size_t n)
    {
        assert(n <= 64 && "CAN frame payloads must not exceed 64 bytes.");
        length_ = static_cast<std::uint8_t>(n);
    }

    std::size_t payload_length() const noexcept
    {
        return length_;
    }

private:
    static constexpr std::uint32_t id_mask = 0x1FFFFFFF;
    static constexpr std::uint32_t format_flag = 0x80000000;
    static constexpr std::uint32_t rtr_flag = 0x40000000;
    static constexpr std::uint32_t error_flag = 0x20000000;

    std::uint32_t id_ = 0;
    std::uint8_t length_ = 0;
    std::uint8_t flags_ = 0;
    std::uint8_t padding_[2];
};

static_assert(sizeof(frame_header) == sizeof(std::uint32_t) * 2,
              "Size of frame header must be exactly 8 bytes");

} // namespace canary

#endif // CANARY_FRAME_HEADER_HPP
