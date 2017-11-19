#include <raft>
#include <raftio>
#include <utility>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "kernels/pcap_file_reader.h"
#include "kernels/raw_packet_parser.h"
//#include "kernels/sink.h"
#include "kernels/data.h"
#include "types/key.h"
#include "types/packet.h"
#include "kernels/printer.h"
#include "flow_table.h"
#include "kernels/clfr_table.h"
#include "types/clfr.h"

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
	//starflow::kernels::Sink<std::pair<starflow::types::Key, starflow::types::CLFR>> sink;
	starflow::kernels::Data data;
	starflow::kernels::CLFRTable cflr_table;	


	std::ofstream data_file;
    data_file.open("data_file.csv");
    data_file << "proto" << "," << "ip_src" << "," << "ip_dest";
    data_file << "," << "s_port" << "," << "d_port" << ",";
	data_file << "#pkts" << "," << "#bytes" << ",";
	data_file << "flow_duration (us)" << ",";
	data_file << "min_ia_time (us)" << ",";
	data_file << "mean_ia_time (us)" << ",";
	data_file << "max_ia_time (us)" << ",";
	data_file << "stddev_ia_time (us)" << ",";
	data_file << "min_pkt_len (bytes)" << ",";
	data_file << "mean_pkt_len (bytes)" << ",";
	data_file << "max_pkt_len (bytes)" << ",";
	data_file << "stddev_pkt_len (bytes)" << ",";
	data_file << std::endl;
	
	raft::map m;

	//m += pcap_reader["out"] >> packet_parser["in"] >> printer;
	m += pcap_reader >> packet_parser["in"] >> cflr_table["packet_in"] >> data;// >> printer;
	m.exe();
	return 0;
}
