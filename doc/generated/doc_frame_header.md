# Header file `frame_header.hpp`

``` cpp
namespace canary
{
    class frame_header;
}
```

### Class `canary::frame_header`

``` cpp
class frame_header
{
public:
    frame_header() = default;

    void id(std::uint32_t value);

    std::uint32_t id() const noexcept;

    void error(bool value);

    bool error() const noexcept;

    void remote_transmission(bool value);

    bool remote_transmission() const noexcept;

    void extended_format(bool value);

    bool extended_format() const noexcept;

    void payload_length(std::size_t n);

    std::size_t payload_length() const noexcept;
};
```

A CAN frame header, used for transmission of both standard and flexible data rate frames.

### Constructor `canary::frame_header::frame_header`

``` cpp
frame_header() = default;
```

Default constructor, performs zero-initialization.

-----

### Function `canary::frame_header::id`

``` cpp
void id(std::uint32_t value);
```

Sets the CAN ID of this frame. CAN IDs are 29-bit integers for extended-format frames and 11-bit for standard-format frames.

#### Parameters

  - `value` - the integral value of the ID.

-----

### Function `canary::frame_header::id`

``` cpp
std::uint32_t id() const noexcept;
```

Gets the CAN ID of this frame. CAN IDs are 29-bit integers for extended-format frames and 11-bit for standard-format frames.

*Returns:* The CAN ID of this frame.

-----

### Function `canary::frame_header::error`

``` cpp
void error(bool value);
```

Sets the error flag of this frame.

*Notes:* Transmitting an error frame over SocketCAN may not be meaningful.

#### Parameters

  - `value` - The value of the flag. True indicates this frame will be an error frame.

-----

### Function `canary::frame_header::error`

``` cpp
bool error() const noexcept;
```

Gets the error flag of this frame.

*Notes:* Transmitting an error frame over SocketCAN may not be meaningful.

*Returns:* The value of the flag. True indicates this frame is an error frame.

-----

### Function `canary::frame_header::remote_transmission`

``` cpp
void remote_transmission(bool value);
```

Sets the remote transmission flag of this frame.

*Notes:* Frames with this flag enabled must not contain a payload.

#### Parameters

  - `value` - The value of the flag. True indicates this frame will be a remote transmission request.

-----

### Function `canary::frame_header::remote_transmission`

``` cpp
bool remote_transmission() const noexcept;
```

Gets the remote transmission flag of this frame.

*Notes:* Frames with this flag enabled must not contain a payload.

*Returns:* The value of the flag. True indicates this frame is a remote transmission request.

-----

### Function `canary::frame_header::extended_format`

``` cpp
void extended_format(bool value);
```

Sets the extended format flag. The flag determines whether this frame uses 29-bit CAN IDs.

#### Parameters

  - `value` - The value of the flag. True indicates this frame will use the extended format.

-----

### Function `canary::frame_header::extended_format`

``` cpp
bool extended_format() const noexcept;
```

Gets the extended format flag. The flag determines whether this frame uses 29-bit CAN IDs.

*Returns:* The value of the flag. True indicates this frame uses the extended format.

-----

### Function `canary::frame_header::payload_length`

``` cpp
void payload_length(std::size_t n);
```

Sets the payload length of this frame.

*Notes:* payload length must not exceed 8 bytes for standard data rate frames and 64 bytes for flexible data rate frames.

#### Parameters

  - `n` - The length of the payload.

-----

### Function `canary::frame_header::payload_length`

``` cpp
std::size_t payload_length() const noexcept;
```

Gets the payload length of this frame.

*Notes:* payload length must not exceed 8 bytes for standard data rate frames and 64 bytes for flexible data rate frames.

*Returns:* The length of the payload.

-----

-----
