
add_executable(pcap_to_raw
        ${KERNELS_SRC}
        ${MODULES_SRC}
        ${TYPES_SRC}
        src/helpers.h src/helpers.cc
        src/pcap_to_raw.cc)

target_link_libraries(pcap_to_raw protobuf)
target_link_libraries(pcap_to_raw pcap)
target_link_libraries(pcap_to_raw raft)