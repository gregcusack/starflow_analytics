
#include <raft>
#include <utility>

#include "kernels/live_capture.h"
#include "kernels/pcap_file_reader.h"
#include "kernels/printer.h"
#include "kernels/raw_packet_parser.h"
#include "proto/starflow.pb.h"
#include "types/key.h"
#include "types/packet.h"
#include "types/clfr.h"
#include "kernels/clfr_table.h"

int main(int argc, char** argv)
{
	auto hdr_type = starflow::kernels::RawPacketParser::outer_header_type::eth;

	raft::lambdak<std::pair<starflow::types::Key, starflow::types::Packet>>
		packet_printer(1, 0, [&] (Port& in, Port& out) {
		std::pair<starflow::types::Key, starflow::types::Packet> p {};
		in["0"].pop(p);
		std::cout << p.first.str_desc() << " " << p.second.str_desc() << std::endl;
//		std::cout << std::dec << (unsigned) p.second.features.ip_ttl << std::endl;
//		std::cout << p.second.features.tcp_flags.str_desc() << std::endl;
		return raft::proceed;
	});

	raft::lambdak<std::pair<starflow::types::Key, starflow::types::CLFR>>
		clfr_printer(1, 0, [&] (Port& in, Port& out) {
		std::pair<starflow::types::Key, starflow::types::CLFR> p {};
		in["0"].pop(p);
		std::cout << p.first.str_desc() << " " << p.second.str_desc() << std::endl;
		return raft::proceed;
	});


	starflow::kernels::LiveCapture live_capture("en10");
	starflow::kernels::PCAPFileReader pcap_file_reader("caida2015_02_dirA.pcap");
	starflow::kernels::RawPacketParser raw_packet_parser(hdr_type);
	starflow::kernels::CLFRTable clfr_table;

	raft::map m;
	m += live_capture >> raw_packet_parser >> clfr_table >> clfr_printer;
	m.exe();

	return 0;
}
