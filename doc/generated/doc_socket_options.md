# Header file `socket_options.hpp`

``` cpp
namespace canary
{
    class flexible_data_rate;
}
```

### Class `canary::flexible_data_rate`

``` cpp
class flexible_data_rate
{
public:
    explicit flexible_data_rate(bool value = true);

    template <class Protocol>
    static int level(Protocol&&);

    template <class Protocol>
    static int name(Protocol&&);

    template <class Protocol>
    void const* data(Protocol&&) const;

    template <class Protocol>
    static std::size_t size(Protocol&&);
};
```

Enables the flexible data rate socket option. Allows a raw CAN socket to receive Flexible Data rate frames.

CAN FD frames support payloads up to 64 bytes, but standard data rate frames may still be received.

### Constructor `canary::flexible_data_rate::flexible_data_rate`

``` cpp
explicit flexible_data_rate(bool value = true);
```

Constructs the option object.

#### Parameters

  - `value` - Value of the option. True indicates the FD frame option is enabled.

-----

-----
