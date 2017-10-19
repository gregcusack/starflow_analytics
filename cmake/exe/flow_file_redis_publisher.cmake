
add_executable(flow_file_redis_publisher
        src/flow_file_redis_publisher_main.cc
        src/redis_flow_publisher.cc
        src/redis_client.cc
        src/proto/starflow.pb.cc
        src/flow.cc
        src/flow_table.cc
        src/import/flow_file_replay.cc)

target_link_libraries(flow_file_redis_publisher protobuf)
target_link_libraries(flow_file_redis_publisher cpp_redis)
target_link_libraries(flow_file_redis_publisher tacopie)
