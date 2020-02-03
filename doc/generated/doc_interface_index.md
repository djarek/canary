# Header file `interface_index.hpp`

``` cpp
namespace canary
{
    unsigned int get_interface_index(std::string const& name, canary::error_code& ec);

    unsigned int get_interface_index(std::string const& name);
}
```

### Function `canary::get_interface_index`

``` cpp
unsigned int get_interface_index(std::string const& name, canary::error_code& ec);
```

Retrieves the interface index required to bind a CAN socket to a particular CAN interface.

#### Parameters

  - `name` - The network interface name.

### Parameter `canary::ec`

``` cpp
canary::error_code& ec
```

Set to indicate what error occurred when the function fails.

*Returns:* The interface index.

-----

-----

### Function `canary::get_interface_index`

``` cpp
unsigned int get_interface_index(std::string const& name);
```

Retrieves the interface index required to bind a CAN socket to a particular CAN interface.  Will throw an instance std::system\_error if the function fails. \\param name The network interface name. \\returns The interface index.

-----
