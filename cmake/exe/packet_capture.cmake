
add_executable(packet_capture
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/kernels/packet_capture.h src/kernels/packet_capture.cc
        src/kernels/printer.h
        src/packet_capture_main.cc)

target_link_libraries(packet_capture protobuf)
target_link_libraries(packet_capture pcap)
target_link_libraries(packet_capture raft)