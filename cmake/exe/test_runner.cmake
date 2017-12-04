
set(TEST_SRC
        test/types/clfr_test.cc
        test/types/raw_packet_test.cc
        test/modules/flow_table_test.cc
        test/modules/pcap_reader_test.cc
        test/etc/tcp_flow_state_test.cc
        test/modules/raw_packet_parser_test.cc)

add_executable(test_runner
        test/test_runner_main.cc
        ${ETC_SRC}
        ${MODULES_SRC}
        ${TYPES_SRC}
        ${TEST_SRC})

target_include_directories(test_runner PUBLIC test/include)
target_link_libraries(test_runner pcap)
