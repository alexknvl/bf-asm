# Copyright (c) 2013 Alexander Konovalov. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

project(BrainfuckAssembler)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
cmake_minimum_required(VERSION 2.6)

# List all the .h (header) files in the module in the alphabetic order.
set(APP_HXX
)

# List all the non-test .cpp files in the alphabetic order.
set(APP_CXX
  main.cpp
)

# List all the *_test.cpp files in the alphabetic order.
set(TEST_CXX
)

add_executable(bfa ${TEST_CXX} ${APP_CXX} ${APP_HXX})
set_target_properties(bfa PROPERTIES
  OUTPUT_NAME "bfa"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(bfa ${Boost_LIBRARIES})
