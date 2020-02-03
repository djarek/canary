# Header file `basic_endpoint.hpp`

``` cpp
namespace canary
{
    template <class Protocol>
    class basic_endpoint;
}
```

### Class `canary::basic_endpoint`

``` cpp
template <class Protocol>
class basic_endpoint
{
public:
    using protocol_type = Protocol;

    using data_type = ::sockaddr;

    basic_endpoint();

    basic_endpoint(unsigned int interface_index);

    protocol_type protocol() const noexcept;

    unsigned int interface_index() const noexcept;

    data_type* data() noexcept;

    data_type* data() const noexcept;

    std::size_t size() const noexcept;

    std::size_t capacity() const noexcept;

    void resize(std::size_t n);
};
```

Describes an endpoint for a CAN bus interface that can be bound to a CAN socket.

### Type alias `canary::basic_endpoint::protocol_type`

``` cpp
using protocol_type = Protocol;
```

The protocol type associated with the endpoint.

-----

### Type alias `canary::basic_endpoint::data_type`

``` cpp
using data_type = ::sockaddr;
```

Underlying type used to store the endpoint information.

-----

### Constructor `canary::basic_endpoint::basic_endpoint`

``` cpp
basic_endpoint();
```

Default constructor

-----

### Constructor `canary::basic_endpoint::basic_endpoint`

``` cpp
basic_endpoint(unsigned int interface_index);
```

Construct an endpoint that represents a particular CAN interface index.

-----

### Function `canary::basic_endpoint::protocol`

``` cpp
protocol_type protocol() const noexcept;
```

Constructs an object of the protocol type associated with this endpoint.

-----

### Function `canary::basic_endpoint::interface_index`

``` cpp
unsigned int interface_index() const noexcept;
```

Returns the interface index that this endpoint represents.

-----

### Function `canary::basic_endpoint::data`

``` cpp
data_type* data() noexcept;
```

Get the underlying endpoint in the native type.

-----

### Function `canary::basic_endpoint::data`

``` cpp
data_type* data() const noexcept;
```

Get the underlying endpoint in the native type.

-----

### Function `canary::basic_endpoint::size`

``` cpp
std::size_t size() const noexcept;
```

Get the underlying size of the endpoint in the native type.

-----

### Function `canary::basic_endpoint::capacity`

``` cpp
std::size_t capacity() const noexcept;
```

Get the underlying capacity of the endpoint in the native type.

-----

### Function `canary::basic_endpoint::resize`

``` cpp
void resize(std::size_t n);
```

Set the underlying size of the endpoint in the native type.

-----

-----
