#include <raft>
#include <raftio>
#include <utility>

#include "kernels/pcap_file_reader.h"
#include "kernels/raw_packet_parser.h"
#include "kernels/sink.h"
#include "types/key.h"
#include "types/packet.h"
#include "kernels/printer.h"
#include "flow_table.h"
//#include "proto/starflow.pb.h"

int main(int argc, char** argv) {
	if(argc != 2) {
		std::cerr << "usage: ./pcap_to_raw pcap_file.pcap" << std::endl;
		return 1;
	}
	auto hdr_type = starflow::kernels::RawPacketParser::outer_header_type::eth;

	raft::lambdak<std::pair<starflow::types::Key, starflow::types::Packet>>
		printer(1, 0, [&] (Port& in, Port& out) {
				std::pair<starflow::types::Key, starflow::types::Packet> p {};
				in["0"].pop(p);
				std::cout << p.first.str_desc() << " " << p.second.str_desc() << std::endl;
				std::cout << std::dec << (unsigned) p.second.features.ip_ttl << std::endl;
				std::cout << p.second.features.tcp_flags.str_desc() << std::endl;
				return raft::proceed;
		});


	starflow::kernels::PCAPFileReader pcap_reader(argv[1]);
	starflow::kernels::RawPacketParser packet_parser;
	starflow::kernels::Sink<std::pair<starflow::types::Key, starflow::types::Packet>> sink;
	starFlow::FlowTable
	
	raft::map m;

	m += pcap_reader >> packet_parser >> printer;
	m.exe();
	return 0;
}
