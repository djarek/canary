//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

// Test if header is self-contained
#include <canary/raw.hpp>

#include <boost/core/lightweight_test.hpp>
#include <canary/interface_index.hpp>

namespace
{

namespace net = canary::net;

void
test_sync_send()
{
    net::io_context ctx{1};
    canary::raw::socket sock1{
      ctx, canary::raw::endpoint{canary::get_interface_index("vcan0")}};
    canary::raw::socket sock2{
      ctx, canary::raw::endpoint{canary::get_interface_index("vcan0")}};
    ::can_frame out_frame{};
    out_frame.can_id = 0xFF;
    std::uint8_t const data[] = {0xDE, 0xAD, 0xBE, 0xEF};
    out_frame.can_dlc = sizeof(data);
    std::memcpy(out_frame.data, data, sizeof(data));
    sock1.send(net::buffer(&out_frame, sizeof(out_frame)));
    ::can_frame in_frame{};
    sock2.receive(net::buffer(&in_frame, sizeof(in_frame)));

    BOOST_TEST_EQ(in_frame.can_id, out_frame.can_id);
    BOOST_TEST_EQ(in_frame.can_dlc, out_frame.can_dlc);
    BOOST_TEST_ALL_EQ(&in_frame.data[0],
                      &in_frame.data[sizeof(data)],
                      &out_frame.data[0],
                      &out_frame.data[sizeof(data)]);
}

void
test_sync_sendto()
{
    net::io_context ctx{1};
    // Use vcan1 so that we can tell apart a default constructed endpoint and
    // one filled by receive_from()
    canary::raw::socket sock1{
      ctx, canary::raw::endpoint{canary::get_interface_index("vcan1")}};
    canary::raw::socket sock2{
      ctx, canary::raw::endpoint{canary::get_interface_index("vcan1")}};

    canary::raw::endpoint const ep1{canary::get_interface_index("vcan1")};
    canary::raw::endpoint ep2;
    ::can_frame out_frame{};
    out_frame.can_id = 0xFF;
    std::uint8_t const data[] = {0xDE, 0xAD, 0xBE, 0xEF};
    out_frame.can_dlc = sizeof(data);
    std::memcpy(out_frame.data, data, sizeof(data));
    sock1.send_to(net::buffer(&out_frame, sizeof(out_frame)), ep1);
    ::can_frame in_frame{};
    sock2.receive_from(net::buffer(&in_frame, sizeof(in_frame)), ep2);

    BOOST_TEST_EQ(in_frame.can_id, out_frame.can_id);
    BOOST_TEST_EQ(in_frame.can_dlc, out_frame.can_dlc);
    BOOST_TEST_ALL_EQ(&in_frame.data[0],
                      &in_frame.data[sizeof(data)],
                      &out_frame.data[0],
                      &out_frame.data[sizeof(data)]);
    BOOST_TEST_EQ(ep1.interface_index(), ep2.interface_index());
}

} // namespace

int
main()
{
    test_sync_send();
    test_sync_sendto();
    return boost::report_errors();
}
