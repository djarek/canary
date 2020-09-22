//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#ifndef CANARY_INTERFACE_INDEX_HPP
#define CANARY_INTERFACE_INDEX_HPP

#include <canary/detail/config.hpp>
#include <string>

namespace canary
{

/* tag::reference[]

[#canary_get_interface_index]
=== `canary::get_interface_index`

Defined in header `<canary/interface_index.hpp>`

[source, c++]
----
unsigned int get_interface_index(std::string const& name, error_code& ec); <1>
unsigned int get_interface_index(std::string const& name);                 <2>
----
Returns the interface index assigned to a CAN interface name, e.g. `can0`.

<1> On failure, `ec` is set to a non-zero value and the
return value is unspecified.

<2> On failure, throws https://en.cppreference.com/w/cpp/error/system_error[`std::system_error`].

CAUTION: Calling `get_interface_index` with a `name` parameter that is not a
valid 0-terminated string invokes Undefined Behavior.

''''

[#canary_any_interface]
=== `canary::any_interface`

[source, c++]
----
unsigned int any_interface();
----
Returns the index which represents any CAN interface.

TIP: Use `receive_from`/`send_to` family of functions on sockets bound to
this index to avoid accidentally sending CAN frames to the wrong interface.

''''

end::reference[] */

CANARY_DECL unsigned int
get_interface_index(std::string const& name, error_code& ec);

CANARY_DECL unsigned int
get_interface_index(std::string const& name);

CANARY_DECL unsigned int
any_interface();

} // namespace canary

#ifndef CANARY_SEPARATE_COMPILATION
#include <canary/impl/interface_index.ipp>
#endif // CANARY_SEPARATE_COMPILATION

#endif // CANARY_INTERFACE_INDEX_HPP
