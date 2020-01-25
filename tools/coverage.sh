#! /bin/bash

#
# Copyright (c) 2020 Damian Jarek (damian.jarek93@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/djarek/canary
#

lcov --directory build --capture --output-file build/coverage.info && \
lcov --extract build/coverage.info $(pwd)'/include/*' --output-file build/coverage.info && \
lcov --list build/coverage.info
