
add_executable(test_runner test/test_runner_main.cc)
target_include_directories(test_runner PUBLIC test/include)
target_link_libraries(test_runner protobuf)
target_link_libraries(test_runner pcap)
target_link_libraries(test_runner raft)
