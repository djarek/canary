//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

// Test if header is self-contained
#include <canary/basic_endpoint.hpp>

#include <boost/core/lightweight_test.hpp>
#include <canary/raw.hpp>

namespace
{

void
test_constructors()
{
    {
        canary::basic_endpoint<canary::raw> ep;
        BOOST_TEST_EQ(ep.interface_index(), 0);
        BOOST_TEST_EQ(ep.data()->sa_family, AF_CAN);
    }

    {
        canary::basic_endpoint<canary::raw> ep{42};
        BOOST_TEST_EQ(ep.interface_index(), 42);
        BOOST_TEST_EQ(ep.data()->sa_family, AF_CAN);
    }
}

void
test_const_data()
{
    canary::basic_endpoint<canary::raw> const ep;
    auto const* p = ep.data();
    BOOST_TEST_EQ(p->sa_family, AF_CAN);
}

void
test_size()
{
    {
        canary::basic_endpoint<canary::raw> ep;
        BOOST_TEST_EQ(ep.size(), sizeof(::sockaddr_can));
        BOOST_TEST_EQ(ep.capacity(), sizeof(::sockaddr_can));
    }

    {
        canary::basic_endpoint<canary::raw> ep;
        BOOST_TEST_THROWS(ep.resize(sizeof(::sockaddr_can) + 1),
                          canary::system_error);
    }

    {
        canary::basic_endpoint<canary::raw> ep;
        ep.resize(sizeof(::sockaddr_can));
        BOOST_TEST_EQ(ep.size(), sizeof(::sockaddr_can));
        BOOST_TEST_EQ(ep.capacity(), sizeof(::sockaddr_can));
        ep.resize(0);
        BOOST_TEST_EQ(ep.size(), sizeof(::sockaddr_can));
        BOOST_TEST_EQ(ep.capacity(), sizeof(::sockaddr_can));
    }
}

} // namespace

int
main()
{
    test_constructors();
    test_const_data();
    test_size();
    return boost::report_errors();
}
