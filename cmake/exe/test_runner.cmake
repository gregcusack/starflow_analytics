
set(TEST_SRC
        test/modules/flow_table_test.cc
        test/modules/pcap_reader_test.cc
        test/modules/raw_packet_parser_test.cc)

add_executable(test_runner
        test/test_runner_main.cc
        src/helpers.h src/helpers.cc
        ${MODULES_SRC}
        ${TYPES_SRC}
        ${TEST_SRC})

target_include_directories(test_runner PUBLIC test/include)
target_link_libraries(test_runner pcap)
