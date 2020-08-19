//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#ifndef CANARY_INTERFACE_INDEX_IPP
#define CANARY_INTERFACE_INDEX_IPP

#include <canary/interface_index.hpp>
#include <net/if.h>

namespace canary
{

unsigned int
get_interface_index(std::string const& name, error_code& ec)
{
    auto ret = ::if_nametoindex(name.c_str());
    if (ret == 0)
    {
        ec.assign(errno, canary::generic_category());
    }
    else
    {
        ec.clear();
    }
    return ret;
}

unsigned int
get_interface_index(std::string const& name)
{
    error_code ec;
    auto ret = canary::get_interface_index(name, ec);
    if (ec)
    {
        canary::detail::throw_exception(system_error{ec});
    }
    return ret;
}

unsigned int
any_interface()
{
    return 0;
}

} // namespace canary

#endif // CANARY_INTERFACE_INDEX_IPP
