//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#ifndef CANARY_FRAME_FILTER_HPP
#define CANARY_FRAME_FILTER_HPP

#include <canary/detail/config.hpp>

#include <cstdint>

namespace canary
{

/// Represents a single filter condition. A filter matches a received frame, if
/// `frame.id() & filter.raw_mask() == filter.id() & filter.raw_mask()`. In its
/// default-constructed state, the filter matches any frame.
struct filter
{
public:
    /// Sets the CAN ID used by this filter.
    filter& id(std::uint32_t value)
    {
        id_ = ((value & id_bitmask) | (id_ & ~id_bitmask));
        return *this;
    }

    /// Gets the CAN ID used by this filter.
    std::uint32_t id() const noexcept
    {
        return (id_ & id_bitmask);
    }

    /// Sets the mask applied to the ID of a frame. In other words, it allows
    /// you to select which bits of a CAN ID are relevant to the filter.
    filter& id_mask(std::uint32_t mask)
    {
        mask_ = (mask_ & (~id_bitmask)) | (mask & id_bitmask);
        return *this;
    }

    /// Gets the mask applied to the ID of a frame.
    std::uint32_t id_mask()
    {
        return mask_ & id_bitmask;
    }

    /// Enables the remote transmission flag filter and sets the expected value
    /// of the flag.
    filter& remote_transmission(bool value)
    {
        if (value)
        {
            id_ |= rtr_flag;
        }
        else
        {
            id_ &= ~rtr_flag;
        }
        mask_ |= rtr_flag;
        return *this;
    }

    /// Gets the expected value of the remote transmission flag.
    bool remote_transmission() const noexcept
    {
        return (id_ & rtr_flag);
    }

    /// Disables the remote transmission flag filter. This filter will not
    /// consider the remote transmission flag.
    filter& clear_remote_transmission()
    {
        mask_ &= ~rtr_flag;
        return *this;
    }

    /// Enables the extended format flag filter and sets the expected value
    /// of the flag.
    filter& extended_format(bool value)
    {
        if (value)
        {
            id_ |= format_flag;
        }
        else
        {
            id_ &= ~format_flag;
        }
        mask_ |= format_flag;
        return *this;
    }

    /// Gets the expected value of the extended format flag.
    bool extended_format() const noexcept
    {
        return (id_ & format_flag);
    }

    /// Disables the extended format flag filter. This filter will not
    /// consider the extended format flag.
    filter& clear_extended_format()
    {
        mask_ &= ~format_flag;
        return *this;
    }

    /// Applies a logical negation to the filter. A frame will match the filter
    /// if `frame.id() & filter.raw_mask() != filter.id() & filter.raw_mask()`.
    filter& negation(bool value)
    {
        if (value)
        {
            id_ |= invert_flag;
        }
        else
        {
            id_ &= ~invert_flag;
        }
        return *this;
    }

    /// Checks whether a logical negation was applied to this filter.
    bool negation() const noexcept
    {
        return (id_ & invert_flag);
    }

private:
    static constexpr std::uint32_t id_bitmask = 0x1FFFFFFF;
    static constexpr std::uint32_t format_flag = 0x80000000;
    static constexpr std::uint32_t rtr_flag = 0x40000000;
    static constexpr std::uint32_t invert_flag = 0x20000000;

    std::uint32_t id_ = 0;
    std::uint32_t mask_ = 0;
};

} // namespace canary
#endif // CANARY_FRAME_FILTER_HPP
