//
// Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/djarek/canary
//

#ifndef CANARY_DETAIL_CONFIG_HPP
#define CANARY_DETAIL_CONFIG_HPP

#ifndef __linux__
#error Canary only supports Linux
#endif // __linux__

#ifdef CANARY_SEPARATE_COMPILATION
#ifndef CANARY_DECL
#define CANARY_DECL
#endif // CANARY_DECL
#else
#ifndef CANARY_DECL
#define CANARY_DECL inline
#endif // CANARY_DECL
#endif // CANARY_SEPARATE_COMPILATION

#ifdef CANARY_STANDALONE_ASIO

#include <system_error>

namespace asio
{
} // namespace asio

namespace canary
{
namespace net = asio;
using error_code = std::error_code;
using system_error = std::system_error;
using std::generic_category;
} // namespace canary

#else // CANARY_STANDALONE_ASIO

#include <boost/system/system_error.hpp>

namespace boost
{
namespace asio
{
} // namespace asio
} // namespace boost

namespace canary
{
namespace net = boost::asio;
using error_code = boost::system::error_code;
using system_error = boost::system::system_error;
using boost::system::generic_category;
} // namespace canary

#endif // CANARY_STANDALONE_ASIO

#endif // CANARY_DETAIL_CONFIG_HPP
