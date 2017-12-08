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
#include "kernels/bi_data.h"
#include "kernels/end.h"
#include "types/key.h"
#include "types/packet.h"
#include "kernels/flow_table.h"
#include "kernels/clfr_table.h"
#include "kernels/bidirectional_flow_table.h"
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
	//starflow::kernels::Data data;
	starflow::kernels::FlowTable flow_table;
	starflow::kernels::BiFlowTable bi_flow_table;
	starflow::kernels::BiData bi_data;
	//starflow::kernels::CLFRTable clfr_table;	
	//starflow::kernels::End end;

/*
	std::ofstream data_file;
    data_file.open("data_file.csv");
    data_file << "proto" << "," << "ip_src" << "," << "ip_dest";
    data_file << "," << "s_port" << "," << "d_port" << ",";
	data_file << "i_npkts" << "," << "i_nbytes" << ",";
	data_file << "i_flow_dur" << ",";
	data_file << "i_min_ia" << ",";
	data_file << "i_mean_ia" << ",";
	data_file << "i_max_ia" << ",";
	data_file << "i_sdev_ia" << ",";
	data_file << "i_min_len" << ",";
	data_file << "i_mean_len" << ",";
	data_file << "i_max_len" << ",";
	data_file << "i_sdev_len" << ",";
	data_file << "i_min_burst" << ",";
	data_file << "i_mean_burst" << ",";
	data_file << "i_max_burst" << ",";


	data_file << "o_npkts" << "," << "o_nbytes" << ",";
	data_file << "o_flow_dur" << ",";
	data_file << "o_min_ia" << ",";
	data_file << "o_mean_ia" << ",";
	data_file << "o_max_ia" << ",";
	data_file << "o_sdev_ia" << ",";
	data_file << "o_min_len" << ",";
	data_file << "o_mean_len" << ",";
	data_file << "o_max_len" << ",";
	data_file << "o_sdev_len" << ",";
	data_file << "o_min_burst" << ",";
	data_file << "o_mean_burst" << ",";
	data_file << "o_max_burst" << ",";

	data_file << "flow_rat" << ",";
	data_file << "byte_rat" << ",";
	data_file << std::endl;
*/

	raft::map m;
	
	//m += pcap_reader >> packet_parser["in"] >> flow_table["packet_in"] >> data;
	m += pcap_reader >> packet_parser["in"] >> flow_table["packet_in"] >> bi_flow_table;
	m += bi_flow_table["clfr_out_flow"] >> bi_data["clfr_out_flow"];
	m += bi_flow_table["clfr_in_flow"] >> bi_data["clfr_in_flow"];

	m.exe();
	return 0;
}
