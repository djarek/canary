#ifndef CANARY_SOCKET_OPTIONS_HPP
#define CANARY_SOCKET_OPTIONS_HPP

#include <canary/detail/config.hpp>
#include <canary/filter.hpp>

#ifdef CANARY_HAS_STD_SPAN
#include <span>
#endif // CANARY_HAS_STD_SPAN
#include <cstdint>
#include <linux/can/raw.h>

namespace canary
{

/* tag::reference[]

[#canary_flexible_data_rate]
=== `canary::flexible_data_rate`

Defined in header `<canary/socket_options.hpp>`

[source, c++]
----
class flexible_data_rate;
----

Enables the flexible data rate socket option. Allows a raw CAN socket to receive
Flexible Data rate frames. CAN FD frames support payloads up to 64 bytes.

TIP: Standard data rate frames can still be received after this option is
enabled.

[#canary_flexible_data_rate_flexible_data_rate]
==== (constructor)
[source, c++]
----
explicit flexible_data_rate(bool value = true);
----
Constructs the option object. `value` equal to `true` enables support for CAN FD
frames.

''''

[#canary_filter_if_any]
=== `canary::filter_if_any`

Defined in header `<canary/socket_options.hpp>`

[source, c++]
----
class filter_if_any;
----

Enables the flexible data rate socket option. Allows a raw CAN socket to receive
Flexible Data rate frames. CAN FD frames support payloads up to 64 bytes, but
standard data rate frames may still be received.

[#canary_filter_if_any_filter_if_any]
==== (constructor)
[source, c++]
----
explicit filter_if_any(filter const* f, std::size_t n); <1>
explicit filter_if_any(std::span<filter const> fs);     <2>
----
Configures a raw CAN socket to use a disjunction of the filters provided to the
constructor. A frame is accepted if it matches any provided filter.

<1> Constructs the option. `[f, f + n)` must be a valid contiguous range.
<2> Construct the option. Available only in C++20 or higher if the standard
library implementation provides `<span>`.

''''

[#canary_filter_if_all]
=== `canary::filter_if_all`

Defined in header `<canary/socket_options.hpp>`

[source, c++]
----
class filter_if_all;
----

Enables the flexible data rate socket option. Allows a raw CAN socket to receive
Flexible Data rate frames. CAN FD frames support payloads up to 64 bytes, but
standard data rate frames may still be received.

[#canary_filter_if_all_filter_if_all]
==== (constructor)
[source, c++]
----
explicit filter_if_all(filter const* f, std::size_t n); <1>
explicit filter_if_all(std::span<filter const> fs);     <2>
----
Configures a raw CAN socket to use a conjunction of the filters provided to the
constructor. A frame is accepted if it matches all provided filters.

<1> Constructs the option. `[f, f + n)` must be a valid contiguous range.
<2> Construct the option. Available only in C++20 or higher if the standard
library implementation provides `<span>`.

''''

end::reference[] */

class flexible_data_rate
{
public:
    explicit flexible_data_rate(bool value = true)
      : value_{value}
    {
    }

    template<class Protocol>
    static int level(Protocol&& /*p*/)
    {
        return SOL_CAN_RAW;
    }

    template<class Protocol>
    static int name(Protocol&& /*p*/)
    {
        return CAN_RAW_FD_FRAMES;
    }

    template<class Protocol>
    void const* data(Protocol&& /*p*/) const
    {
        return &value_;
    }

    template<class Protocol>
    static std::size_t size(Protocol&& /*p*/)
    {
        return sizeof(value_);
    }

private:
    int value_;
};

class filter_if_any
{
public:
    explicit filter_if_any(filter const* f, std::size_t n)
      : filters_{f}
      , n_{n}
    {
    }

#ifdef CANARY_HAS_STD_SPAN
    explicit filter_if_any(std::span<filter const> fs)
      : filter_if_any(fs.data(), fs.size())
    {
    }
#endif // CANARY_HAS_STD_SPAN

    template<class Protocol>
    static int level(Protocol&& /*p*/)
    {
        return SOL_CAN_RAW;
    }

    template<class Protocol>
    static int name(Protocol&& /*p*/)
    {
        return CAN_RAW_FILTER;
    }

    template<class Protocol>
    void const* data(Protocol&& /*p*/) const
    {
        return filters_;
    }

    template<class Protocol>
    std::size_t size(Protocol&& /*p*/) const
    {
        return n_ * sizeof(filter);
    }

private:
    filter const* filters_;
    std::size_t n_;
};


class filter_if_all
{
public:
    explicit filter_if_all(filter const* f, std::size_t n)
      : filters_{f}
      , n_{n}
    {
    }

#ifdef CANARY_HAS_STD_SPAN
    explicit filter_if_all(std::span<filter const> fs)
      : filter_if_all(fs.data(), fs.size())
    {
    }
#endif // CANARY_HAS_STD_SPAN

    template<class Protocol>
    static int level(Protocol&& /*p*/)
    {
        return SOL_CAN_RAW;
    }

    template<class Protocol>
    static int name(Protocol&& /*p*/)
    {
        return CAN_RAW_JOIN_FILTERS;
    }

    template<class Protocol>
    void const* data(Protocol&& /*p*/) const
    {
        return filters_;
    }

    template<class Protocol>
    std::size_t size(Protocol&& /*p*/) const
    {
        return n_ * sizeof(filter);
    }

private:
    filter const* filters_;
    std::size_t n_;
};

} // namespace canary

#endif // CANARY_SOCKET_OPTIONS_HPP
