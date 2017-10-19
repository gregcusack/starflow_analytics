
add_executable(test_publisher src/test_publisher_main.cc
        src/flow.h src/flow.cc
        src/flow_table.h src/flow_table.cc
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/redis_client.h src/redis_client.cc
        src/redis_flow_publisher.h src/redis_flow_publisher.cc)

target_link_libraries(test_publisher protobuf)
target_link_libraries(test_publisher cpp_redis)
target_link_libraries(test_publisher tacopie)
