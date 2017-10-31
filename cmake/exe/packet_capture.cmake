
add_executable(packet_capture
        src/types/key.h src/types/key.cc
        src/types/features.h src/types/features.cc
        src/types/packet.h src/types/packet.cc
        src/types/raw_packet.h src/types/raw_packet.cc
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/kernels/live_capture.h src/kernels/live_capture.cc
        src/kernels/pcap_file_reader.h src/kernels/pcap_file_reader.cc
        src/kernels/raw_packet_parser.h src/kernels/raw_packet_parser.cc
        src/kernels/printer.h
        src/packet_capture_main.cc)

target_link_libraries(packet_capture protobuf)
target_link_libraries(packet_capture pcap)
target_link_libraries(packet_capture raft)