
add_executable(packet_capture
        ${KERNELS_SRC}
        ${MODULES_SRC}
        ${TYPES_SRC}
        src/helpers.h src/helpers.cc
        src/packet_capture_main.cc)

target_link_libraries(packet_capture protobuf)
target_link_libraries(packet_capture pcap)
target_link_libraries(packet_capture raft)