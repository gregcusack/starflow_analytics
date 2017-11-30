
add_executable(test_runner
        ${MODULES_SRC}
        test/test_runner_main.cc
        test/modules/pcap_reader_test.cc)

target_include_directories(test_runner PUBLIC test/include)
target_link_libraries(test_runner protobuf)
target_link_libraries(test_runner pcap)
target_link_libraries(test_runner raft)
