cmake_minimum_required(VERSION 3.8)
project(starflow_analytics)

set(CMAKE_CXX_STANDARD 11)

include(cmake/protobuf.cmake)
include(cmake/pcap.cmake)
include(cmake/cpp_redis.cmake)
