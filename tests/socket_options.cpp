//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

// Test if header is self-contained
#include <canary/socket_options.hpp>

#include <boost/core/lightweight_test.hpp>
#include <canary/frame_header.hpp>
#include <canary/interface_index.hpp>
#include <canary/raw.hpp>

namespace
{

void
test_can_fd()
{
    canary::net::io_context ctx{1};
    canary::raw::socket sock1{
      ctx, canary::raw::endpoint{canary::get_interface_index("vcan0")}};
    canary::raw::socket sock2{
      ctx, canary::raw::endpoint{canary::get_interface_index("vcan0")}};

    sock1.set_option(canary::flexible_data_rate{true});
    sock2.set_option(canary::flexible_data_rate{true});

    struct frame
    {
        canary::frame_header fh;
        std::array<char, 64> data{};
    } fr;

    fr.fh.payload_length(fr.data.size());
    fr.fh.id(0xFF);

    sock1.send(canary::net::buffer(&fr, sizeof(fr)));
    sock2.receive(canary::net::buffer(&fr, sizeof(fr)));
}

void
test_if_any_filter()
{
    canary::net::io_context ctx{1};
    canary::raw::socket sock1{
      ctx, canary::raw::endpoint{canary::get_interface_index("vcan0")}};
    canary::raw::socket sock2{
      ctx, canary::raw::endpoint{canary::get_interface_index("vcan0")}};

    const canary::filter filters[] = {
      canary::filter{}.id(0xFF).id_mask(~0u),
      canary::filter{}.id(0xFFFF).id_mask(~0u).extended_format(true)};

#ifdef CANARY_HAS_STD_SPAN
    sock1.set_option(canary::filter_if_any{{filters}});
#else  // CANARY_HAS_STD_SPAN
    sock1.set_option(canary::filter_if_any{filters, 2});
#endif // CANARY_HAS_STD_SPAN

    struct frame
    {
        canary::frame_header header;
        std::array<char, 8> data{};
    } f;

    f.header.payload_length(f.data.size());
    f.header.id(0xFF);

    sock2.send(canary::net::buffer(&f, sizeof(f)));
    const auto n = sock1.receive(canary::net::buffer(&f, sizeof(f)));
    BOOST_TEST_EQ(n, sizeof(f));
    BOOST_TEST_EQ(f.header.id(), 0xFF);

    f.header.id(0xFFFE);
    f.header.extended_format(true);
    sock2.send(canary::net::buffer(&f, sizeof(f)));
    sock1.async_receive(canary::net::buffer(&f, sizeof(f)),
                        [](canary::error_code ec, std::size_t n) {
                            BOOST_TEST(ec);
                            BOOST_TEST_EQ(n, 0);
                        });
    const auto run_count = ctx.run_for(std::chrono::milliseconds{100});
    BOOST_TEST(run_count == 0);
    sock1.cancel();
}

void
test_if_filter_size_exceeded()
{
    canary::net::io_context ctx{1};
    canary::raw::socket sock{
      ctx, canary::raw::endpoint{canary::get_interface_index("vcan0")}};
    std::vector<canary::filter> filters(CAN_RAW_FILTER_MAX * 2);

    canary::error_code error;
    sock.set_option(canary::filter_if_any{filters.data(), filters.size()}, error);
    BOOST_TEST(error == std::errc::invalid_argument);

    sock.set_option(canary::filter_if_all{filters.data(), filters.size()}, error);
    BOOST_TEST(error == std::errc::invalid_argument);
}

} // namespace

int
main()
{
    test_can_fd();
    test_if_any_filter();
    test_if_filter_size_exceeded();
    return boost::report_errors();
}
