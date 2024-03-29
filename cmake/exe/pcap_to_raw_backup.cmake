add_executable(pcap_to_raw src/pcap_to_raw.cc
        src/types/key.h src/types/key.cc
		src/types/features.h src/types/features.cc
		src/types/packet.h src/types/packet.cc
		src/types/raw_packet.h src/types/raw_packet.cc
		src/types/clfr.h src/types/clfr.cc
		src/kernels/data.h src/kernels/data.cc
		src/kernels/end.h src/kernels/end.cc
		src/flow.h src/flow.cc
		src/flow_table.h src/flow_table.cc
		src/proto/starflow.pb.h src/proto/starflow.pb.cc
		src/import/pcap_replay.cc
		src/kernels/pcap_file_reader.h src/kernels/pcap_file_reader.cc
		src/kernels/raw_packet_parser.h src/kernels/raw_packet_parser.cc
		src/kernels/clfr_table.h src/kernels/clfr_table.cc
		src/kernels/clfr_counter.h
)
target_link_libraries(pcap_to_raw protobuf)
target_link_libraries(pcap_to_raw raft)
target_link_libraries(pcap_to_raw pcap)
