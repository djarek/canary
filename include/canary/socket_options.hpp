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

/// Enables the flexible data rate socket option.
///
/// Allows a raw CAN socket to receive Flexible Data rate frames. CAN FD frames
/// support payloads up to 64 bytes, but standard data rate frames may still be
/// received.
class flexible_data_rate
{
public:
    /// Constructs the option object.
    /// \param value Value of the option. True indicates the FD frame option is
    /// enabled.
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

/// Configures a raw CAN socket to use a disjunction of the filters provided to
/// the constructor. A frame is accepted if it matches any provided filter.
class filter_if_any
{
public:
    /// Constructs the option object.
    /// \param f a non-null pointer to an array of filter objects
    /// \param n number of filters
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

/// Configures a raw CAN socket to use a disjunction of the filters provided to
/// the constructor. A frame is accepted if it matches all provided filters.
class filter_if_all
{
public:
    /// Constructs the option object.
    /// \param f a non-null pointer to an array of filter objects
    /// \param n number of filters
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
