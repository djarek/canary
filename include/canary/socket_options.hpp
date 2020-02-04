#ifndef CANARY_SOCKET_OPTIONS_HPP
#define CANARY_SOCKET_OPTIONS_HPP

#include <cstdint>
#include <linux/can/raw.h>

namespace canary
{

/// Enables the flexible data rate socket option.

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

} // namespace canary

#endif // CANARY_SOCKET_OPTIONS_HPP
