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


/* tag::reference[]

[#canary_filter]
=== `canary::filter`

Defined in header `<canary/filter.hpp>`

[source, c++]
----
class filter;
----

Represents a single filter condition. A filter matches a received frame, if
`frame.id() & filter.raw_mask() == filter.id() & filter.raw_mask()`. In its
default-constructed state, the filter matches any frame.

[#canary_filter_id]
==== `id()`
[source, c++]
----
filter& id(std::uint32_t value);   <1>
std::uint32_t id() const noexcept; <2>
----
Accessors for the CAN ID.

<1> Sets the CAN ID value. Returns a reference to `this`, which enables fluent use.
<2> Returns the CAN ID value.

[#canary_filter_id_mask]
==== `id_mask()`
[source, c++]
----
filter& id_mask(std::uint32_t mask); <1>
std::uint32_t id_mask();             <2>
----
Accessors for the CAN ID mask. The mask determines which bits of a frame's CAN ID are relevant to the filter.

<1> Sets the CAN ID mask value. Returns a reference to `this`, which enables fluent use.
<2> Returns the CAN ID mask value.

[#canary_filter_remote_transmission]
==== `remote_transmission()`
[source, c++]
----
filter& remote_transmission(bool value);   <1>
bool remote_transmission() const noexcept; <2>
filter& clear_remote_transmission();       <3>
----
Accessors for the remote transmission flag.

<1> Enables the remote transmission flag filter and sets the expected value of
the flag. Returns a reference to `this`, which enables fluent use.
<2> Returns the remote transmission flag value.
<3> Disables the remote transmission flag filter (returns to the default state).

[#canary_filter_extended_format]
==== `extended_format()`
[source, c++]
----
    filter& extended_format(bool value);   <1>
    bool extended_format() const noexcept; <2>
    filter& clear_extended_format();       <3>
----
Accessors for the extended format flag.

<1> Enables the extended format flag filter and sets the expected value of
the flag. Returns a reference to `this`, which enables fluent use.
<2> Returns the extended format flag value.
<3> Disables the extended format flag filter (returns to the default state).


[#canary_filter_negation]
==== `negation()`
[source, c++]
----
    filter& negation(bool value);   <1>
    bool negation() const noexcept; <2>
----
Accessors for the logical negation flag. When true, a frame will match the
filter if `frame.id() & filter.raw_mask() != filter.id() & filter.raw_mask()`.

<1> Sets the value of the negation flag. Returns a reference to `this`, which enables fluent use.
<2> Returns the value of the negation flag.

''''

end::reference[] */

namespace canary
{


class filter
{
public:
    filter& id(std::uint32_t value)
    {
        id_ = ((value & id_bitmask) | (id_ & ~id_bitmask));
        return *this;
    }

    std::uint32_t id() const noexcept
    {
        return (id_ & id_bitmask);
    }

    filter& id_mask(std::uint32_t mask)
    {
        mask_ = (mask_ & (~id_bitmask)) | (mask & id_bitmask);
        return *this;
    }

    std::uint32_t id_mask()
    {
        return mask_ & id_bitmask;
    }

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

    bool remote_transmission() const noexcept
    {
        return (id_ & rtr_flag);
    }

    filter& clear_remote_transmission()
    {
        mask_ &= ~rtr_flag;
        return *this;
    }

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

    bool extended_format() const noexcept
    {
        return (id_ & format_flag);
    }

    filter& clear_extended_format()
    {
        mask_ &= ~format_flag;
        return *this;
    }

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
