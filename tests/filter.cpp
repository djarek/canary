//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

// Test if header is self-contained
#include <canary/filter.hpp>

#include <boost/core/lightweight_test.hpp>
#include <linux/can.h>

namespace
{

static_assert(sizeof(canary::filter) == sizeof(::can_filter),
              "Filter size mismatch");
static_assert(std::is_trivially_copyable<canary::filter>::value,
              "Filter must be trivially copyable");
static_assert(std::is_standard_layout<canary::filter>::value,
              "Filter must be standard layout");

void
test_layout()
{
    canary::filter filter;
    ::can_filter native_filter;
    filter.id(0xFEFE);

    std::memcpy(&native_filter, &filter, sizeof(filter));
    BOOST_TEST_EQ(native_filter.can_id, 0xFEFE);
    BOOST_TEST_EQ(filter.id(), 0xFEFE);

    filter.id_mask(0xADAD);
    std::memcpy(&native_filter, &filter, sizeof(filter));
    BOOST_TEST_EQ(native_filter.can_id, 0xFEFE);
    BOOST_TEST_EQ(native_filter.can_mask, 0xADAD);
    BOOST_TEST_EQ(filter.id(), 0xFEFE);
    BOOST_TEST_EQ(filter.id_mask(), 0xADAD);

    filter.remote_transmission(true);
    std::memcpy(&native_filter, &filter, sizeof(filter));
    BOOST_TEST_EQ(native_filter.can_id, CAN_RTR_FLAG | 0xFEFE);
    BOOST_TEST_EQ(native_filter.can_mask, CAN_RTR_FLAG | 0xADAD);
    BOOST_TEST_EQ(filter.id(), 0xFEFE);
    BOOST_TEST_EQ(filter.id_mask(), 0xADAD);
    BOOST_TEST(filter.remote_transmission());

    filter.remote_transmission(false);
    std::memcpy(&native_filter, &filter, sizeof(filter));
    BOOST_TEST_EQ(native_filter.can_id, 0xFEFE);
    BOOST_TEST_EQ(native_filter.can_mask, CAN_RTR_FLAG | 0xADAD);
    BOOST_TEST_EQ(filter.id(), 0xFEFE);
    BOOST_TEST_EQ(filter.id_mask(), 0xADAD);
    BOOST_TEST_NOT(filter.remote_transmission());

    filter.clear_remote_transmission();
    std::memcpy(&native_filter, &filter, sizeof(filter));
    BOOST_TEST_EQ(native_filter.can_id, 0xFEFE);
    BOOST_TEST_EQ(native_filter.can_mask, 0xADAD);
    BOOST_TEST_EQ(filter.id(), 0xFEFE);
    BOOST_TEST_EQ(filter.id_mask(), 0xADAD);
    BOOST_TEST_NOT(filter.remote_transmission());

    filter.extended_format(true);
    std::memcpy(&native_filter, &filter, sizeof(filter));
    BOOST_TEST_EQ(native_filter.can_id, CAN_EFF_FLAG | 0xFEFE);
    BOOST_TEST_EQ(native_filter.can_mask, CAN_EFF_FLAG | 0xADAD);
    BOOST_TEST_EQ(filter.id(), 0xFEFE);
    BOOST_TEST_EQ(filter.id_mask(), 0xADAD);
    BOOST_TEST(filter.extended_format());

    filter.extended_format(false);
    std::memcpy(&native_filter, &filter, sizeof(filter));
    BOOST_TEST_EQ(native_filter.can_id, 0xFEFE);
    BOOST_TEST_EQ(native_filter.can_mask, CAN_EFF_FLAG | 0xADAD);
    BOOST_TEST_EQ(filter.id(), 0xFEFE);
    BOOST_TEST_EQ(filter.id_mask(), 0xADAD);
    BOOST_TEST_NOT(filter.extended_format());

    filter.clear_extended_format();
    std::memcpy(&native_filter, &filter, sizeof(filter));
    BOOST_TEST_EQ(native_filter.can_id, 0xFEFE);
    BOOST_TEST_EQ(native_filter.can_mask, 0xADAD);
    BOOST_TEST_EQ(filter.id(), 0xFEFE);
    BOOST_TEST_EQ(filter.id_mask(), 0xADAD);
    BOOST_TEST_NOT(filter.extended_format());

    filter.negation(true);
    std::memcpy(&native_filter, &filter, sizeof(filter));
    BOOST_TEST_EQ(native_filter.can_id, CAN_INV_FILTER | 0xFEFE);
    BOOST_TEST_EQ(native_filter.can_mask, 0xADAD);
    BOOST_TEST(filter.negation());

    filter.negation(false);
    std::memcpy(&native_filter, &filter, sizeof(filter));
    BOOST_TEST_EQ(native_filter.can_id, 0xFEFE);
    BOOST_TEST_EQ(native_filter.can_mask, 0xADAD);
    BOOST_TEST_NOT(filter.negation());
}
} // namespace

int
main()
{
    test_layout();
    return boost::report_errors();
}
