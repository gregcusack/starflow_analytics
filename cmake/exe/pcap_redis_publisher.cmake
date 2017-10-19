
add_executable(pcap_redis_publisher
        src/pcap_redis_publisher_main.cc
        src/redis_flow_publisher.cc
        src/redis_client.cc
        src/flow_table.cc
        src/flow.cc
        src/proto/starflow.pb.cc
        src/import/pcap_replay.cc src/flow_file_redis_publisher_main.cc)

target_link_libraries(pcap_redis_publisher protobuf)
target_link_libraries(pcap_redis_publisher cpp_redis)
target_link_libraries(pcap_redis_publisher pcap)
target_link_libraries(pcap_redis_publisher tacopie)