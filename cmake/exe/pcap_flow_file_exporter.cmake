
add_executable(pcap_flow_file_exporter
        src/pcap_flow_file_exporter_main.cc
        src/flow_table.cc
        src/flow.cc
        src/proto/starflow.pb.cc
        src/import/pcap_replay.cc)

target_link_libraries(pcap_flow_file_exporter protobuf)
target_link_libraries(pcap_flow_file_exporter pcap)