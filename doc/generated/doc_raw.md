# Header file `raw.hpp`

``` cpp
#include <canary/basic_endpoint.hpp>

namespace canary
{
    class raw;
}
```

### Class `canary::raw`

``` cpp
class raw
{
public:
    int type() const noexcept;

    int protocol() const noexcept;

    int family() const noexcept;

    using endpoint = basic_endpoint<canary::raw>;

    using socket = net::basic_raw_socket<raw>;
};
```

Encapsulates flags necessary for creation of raw CAN sockets.

### Function `canary::raw::type`

``` cpp
int type() const noexcept;
```

Obtain an identifier for the protocol type.

-----

### Function `canary::raw::protocol`

``` cpp
int protocol() const noexcept;
```

Obtain an identifier for the protocol.

-----

### Function `canary::raw::family`

``` cpp
int family() const noexcept;
```

Obtain an identifier for the address family.

-----

### Type alias `canary::raw::endpoint`

``` cpp
using endpoint = basic_endpoint<canary::raw>;
```

CAN endpoint type, represents a CAN interface (e.g. vcan0)

-----

### Type alias `canary::raw::socket`

``` cpp
using socket = net::basic_raw_socket<raw>;
```

Raw CAN socket type

-----

-----
