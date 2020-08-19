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

/// Retrieves the interface index required to bind a CAN socket to a particular
/// CAN interface.
/// \param name The network interface name.
/// \param ec Set to indicate what error occurred when the function fails.
/// \returns The interface index.
CANARY_DECL unsigned int
get_interface_index(std::string const& name, error_code& ec);

/// Retrieves the interface index required to bind a CAN socket to a particular
/// CAN interface.  Will throw an instance std::system_error if the function
/// fails. \param name The network interface name. \returns The interface index.
CANARY_DECL unsigned int
get_interface_index(std::string const& name);

/// Returns the index which represents any CAN interface.
/// \note When using any CAN interface, it is recommended to use
/// `receive_from`/`send_to` functions to avoid unintentionally sending frames
/// to the wrong CAN interface.
/// \returns The interface index.
CANARY_DECL unsigned int
any_interface();

} // namespace canary

#ifndef CANARY_SEPARATE_COMPILATION
#include <canary/impl/interface_index.ipp>
#endif // CANARY_SEPARATE_COMPILATION

#endif // CANARY_INTERFACE_INDEX_HPP
