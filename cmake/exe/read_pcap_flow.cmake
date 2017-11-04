
add_executable(read_pcap_flow
        src/read_pcap_flow.cc
        src/flow_table.cc
        src/flow.cc
        src/proto/starflow.pb.cc
        src/import/pcap_replay.cc)

target_link_libraries(read_pcap_flow protobuf)
target_link_libraries(read_pcap_flow pcap)
