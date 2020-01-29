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

/// A CAN frame header, used for transmission of both standard and flexible data
/// rate frames.
class frame_header
{
public:
    /// Default constructor, performs zero-initialization.
    frame_header() = default;

    /// Sets the CAN ID of this frame. CAN IDs are 29-bit integers for
    /// extended-format frames and 11-bit for standard-format frames.
    /// \param value the integral value of the ID.
    void id(std::uint32_t value)
    {
        id_ = ((value & id_mask) | (id_ & ~id_mask));
    }

    /// Gets the CAN ID of this frame. CAN IDs are 29-bit integers for
    /// extended-format frames and 11-bit for standard-format frames.
    /// \returns The CAN ID of this frame.
    std::uint32_t id() const noexcept
    {
        return (id_ & id_mask);
    }

    /// Sets the error flag of this frame.
    /// \notes Transmitting an error frame over SocketCAN may not be
    /// meaningful.
    /// \param value The value of the flag. True indicates this frame will be an
    /// error frame.
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
    /// Gets the error flag of this frame.
    /// \notes Transmitting an error frame over SocketCAN may not be
    /// meaningful.
    /// \returns The value of the flag. True indicates this frame is an error
    /// frame.
    bool error() const noexcept
    {
        return (id_ & error_flag);
    }

    /// Sets the remote transmission flag of this frame.
    /// \notes Frames with this flag enabled must not contain a payload.
    /// \param value The value of the flag. True indicates this frame will be a
    /// remote transmission request.
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

    /// Gets the remote transmission flag of this frame.
    /// \notes Frames with this flag enabled must not contain a payload.
    /// \returns The value of the flag. True indicates this frame is a remote
    /// transmission request.
    bool remote_transmission() const noexcept
    {
        return (id_ & rtr_flag);
    }

    /// Sets the extended format flag. The flag determines whether this frame
    /// uses 29-bit CAN IDs.
    /// \param value The value of the flag. True indicates this frame will use
    /// the extended format.
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

    /// Gets the extended format flag. The flag determines whether this frame
    /// uses 29-bit CAN IDs.
    /// \returns The value of the flag. True indicates this frame uses the
    /// extended format.
    bool extended_format() const noexcept
    {
        return (id_ & format_flag);
    }

    /// Sets the payload length of this frame.
    /// \notes  payload length must not exceed 8 bytes for standard data rate
    /// frames and 64 bytes for flexible data rate frames.
    /// \param n The length of the payload.
    void payload_length(std::size_t n)
    {
        assert(n <= 64 && "CAN frame payloads must not exceed 64 bytes.");
        length_ = static_cast<std::uint8_t>(n);
    }

    /// Gets the payload length of this frame.
    /// \notes  payload length must not exceed 8 bytes for standard data rate
    /// frames and 64 bytes for flexible data rate frames.
    /// \returns The length of the payload.
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
