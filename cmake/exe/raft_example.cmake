
add_executable(raft_example src/raft_example_main.cc
        src/kernels/counter.h
        src/kernels/printer.h
        src/kernels/redis_flow_subscriber.h
        src/kernels/ip_source_timestamps.h
        src/kernels/sink.h
        src/flow.h src/flow.cc
        src/flow_table.h src/flow_table.cc
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/redis_subscriber.h src/redis_subscriber.cc
        src/redis_flow_subscriber.h src/redis_flow_subscriber.cc)

target_link_libraries(raft_example protobuf)
target_link_libraries(raft_example cpp_redis)
target_link_libraries(raft_example tacopie)
target_link_libraries(raft_example raft)
