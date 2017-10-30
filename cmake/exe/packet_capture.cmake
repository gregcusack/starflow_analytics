
add_executable(packet_capture
        src/raw_packet.h src/raw_packet.cc
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/kernels/live_capture.h src/kernels/live_capture.cc
        src/kernels/printer.h
        src/packet_capture_main.cc)

target_link_libraries(packet_capture protobuf)
target_link_libraries(packet_capture pcap)
target_link_libraries(packet_capture raft)