
add_executable(test_subscriber src/test_subscriber_main.cc
        src/flow.h src/flow.cc
        src/flow_table.h src/flow_table.cc
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/redis_subscriber.h src/redis_subscriber.cc
        src/redis_flow_subscriber.h src/redis_flow_subscriber.cc)

target_link_libraries(test_subscriber protobuf)
target_link_libraries(test_subscriber cpp_redis)
target_link_libraries(test_subscriber tacopie)
