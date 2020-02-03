//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

// Test if header is self-contained
#include <canary/frame_header.hpp>

#include <array>
#include <boost/core/lightweight_test.hpp>
#include <linux/can.h>

namespace
{

void
test_layout()
{
    struct frame
    {
        canary::frame_header fh;
        std::array<std::uint8_t, 8> data;
    } f{};
    ::can_frame cf;
    static_assert(sizeof(f) == sizeof(cf),
                  "Frame header layout must match native can_frame struct.");

    f.fh.id(0x1EAD);
    BOOST_TEST_EQ(f.fh.id(), 0x1EAD);

    f.fh.extended_format(true);
    f.fh.error(false);
    f.fh.remote_transmission(false);
    f.fh.payload_length(2);
    std::memcpy(&cf, &f, sizeof(f));
    BOOST_TEST_EQ(cf.can_id & CAN_EFF_MASK, 0x1EAD);
    BOOST_TEST_EQ((cf.can_id & CAN_EFF_FLAG) >> 31, true);
    BOOST_TEST_EQ((cf.can_id & CAN_RTR_FLAG) >> 30, false);
    BOOST_TEST_EQ((cf.can_id & CAN_ERR_FLAG) >> 29, false);
    BOOST_TEST_EQ(static_cast<int>(cf.can_dlc), 2);

    f.fh.extended_format(false);
    f.fh.error(false);
    f.fh.remote_transmission(true);
    f.fh.payload_length(3);
    std::memcpy(&cf, &f, sizeof(f));
    BOOST_TEST_EQ(cf.can_id & CAN_EFF_MASK, 0x1EAD);
    BOOST_TEST_EQ((cf.can_id & CAN_EFF_FLAG) >> 31, false);
    BOOST_TEST_EQ((cf.can_id & CAN_RTR_FLAG) >> 30, true);
    BOOST_TEST_EQ((cf.can_id & CAN_ERR_FLAG) >> 29, false);
    BOOST_TEST_EQ(static_cast<int>(cf.can_dlc), 3);

    f.fh.extended_format(false);
    f.fh.error(true);
    f.fh.remote_transmission(false);
    f.fh.payload_length(4);
    std::memcpy(&cf, &f, sizeof(f));
    BOOST_TEST_EQ(cf.can_id & CAN_EFF_MASK, 0x1EAD);
    BOOST_TEST_EQ((cf.can_id & CAN_EFF_FLAG) >> 31, false);
    BOOST_TEST_EQ((cf.can_id & CAN_RTR_FLAG) >> 30, false);
    BOOST_TEST_EQ((cf.can_id & CAN_ERR_FLAG) >> 29, true);
    BOOST_TEST_EQ(static_cast<int>(cf.can_dlc), 4);
}

} // namespace

int
main()
{
    test_layout();
    return boost::report_errors();
}
