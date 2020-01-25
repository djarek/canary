//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#ifndef CANARY_STATIC_FRAME_HPP
#define CANARY_STATIC_FRAME_HPP

#include <cstdint>
#include <linux/can.h>

namespace canary
{

class static_frame
{
public:
    static_frame() = default;

    void id(std::uint32_t val)
    {
        frame_.can_id = (val & CAN_EFF_MASK | (frame_.can_id & ~CAN_EFF_MASK));
    }

    std::uint32_t id() const noexcept
    {
        return (frame_.can_id & CAN_EFF_MASK);
    }

    void error(bool val)
    {
        if (val)
        {
            frame_.can_id |= CAN_ERR_FLAG;
        }
        else
        {
            frame_.can_id &= ~CAN_ERR_FLAG;
        }
    }

    bool error() const noexcept
    {
        return (frame_.can_id & CAN_ERR_FLAG);
    }

    void remote_transmission(bool val)
    {
        if (val)
        {
            frame_.can_id |= CAN_RTR_FLAG;
        }
        else
        {
            frame_.can_id &= ~CAN_RTR_FLAG;
        }
    }

    bool remote_transmission()
    {
        return (frame_.can_id & CAN_RTR_FLAG);
    }

    void is_extended_format(bool val)
    {
        if (val)
        {
            frame_.can_id |= CAN_EFF_FLAG;
        }
        else
        {
            frame_.can_id &= ~CAN_EFF_FLAG;
        }
    }

    bool is_extended_format() const noexcept
    {
        return (frame_.can_id & CAN_EFF_FLAG);
    }

    void resize(std::size_t n)
    {
        if (n > capacity())
        {
            throw std::length_error{
              "static_frame resize exceeds CAN frame size"};
        }

        frame_.can_dlc = n;
    }

    std::size_t size() const noexcept
    {
        return frame_.can_dlc;
    }

    std::size_t capacity() const noexcept
    {
        return sizeof(frame_.data);
    }

    boost::asio::mutable_buffer data() noexcept
    {
        assert(frame_.can_dlc <= sizeof(frame_.data));
        return {&frame_.data, frame_.can_dlc};
    }

    boost::asio::const_buffer data() const noexcept
    {
        assert(frame_.can_dlc < sizeof(frame_.data));
        return {&frame_.data, frame_.can_dlc};
    }

    boost::asio::mutable_buffer buffers() noexcept
    {
        return {&frame_, sizeof(frame_)};
    }

    boost::asio::const_buffer buffers() const noexcept
    {
        return {&frame_, sizeof(frame_)};
    }

private:
    ::can_frame frame_{};
};

} // namespace canary

#endif // CANARY_STATIC_FRAME_HPP
