
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
#include "kernels/heavy_hitter.h"

#include "kernels/tzsp_receiver.h"

int main(int argc, char** argv)
{
	auto hdr_type = starflow::kernels::RawPacketParser::outer_header_type::eth;
	auto cap_len  = starflow::kernels::RawPacketParser::capture_length::trunc;

	raft::lambdak<std::pair<starflow::types::Key, starflow::types::Packet>>
		packet_printer(1, 0, [&] (Port& in, Port& out) {
		std::pair<starflow::types::Key, starflow::types::Packet> p {};
		in["0"].pop(p);
		std::cout << p.first.str_desc() << " " << p.second.str_desc() << std::endl;
		std::cout << p.second.features.tcp_flags.str_desc() << std::endl;
		return raft::proceed;
	});

	raft::lambdak<std::pair<starflow::types::Key, starflow::types::CLFR>>
		clfr_printer(1, 0, [&] (Port& in, Port& out) {
		std::pair<starflow::types::Key, starflow::types::CLFR> p {};
		in["0"].pop(p);
		std::cout << p.first.str_desc() << " " << p.second.str_desc() << std::endl;
		return raft::proceed;
	});

//
//	starflow::kernels::LiveCapture live_capture("en0");
	starflow::kernels::PCAPFileReader pcap_file_reader("test/data/http.pcap");


//
//	raft::map m;
//	m += live_capture >> raw_packet_parser >> clfr_table >> clfr_printer;
//	m.exe();

//	raft::lambdak<starflow::types::RawPacket> p(1, 0, [&] (Port& in, Port& out) {
//		starflow::types::RawPacket p;
//		in["0"].pop(p);
//		std::cout << std::dec << p.len << ": ";
//
//		for (unsigned i = 0; i < p.len; i++)
//			std::cout << std::hex << (unsigned) p.pl[i] << " ";
//
//		std::cout << std::endl;
//
//
//		return raft::proceed;
//	});

//	starflow::kernels::TZSPReceiver tzsp_receiver(37008);
	starflow::kernels::RawPacketParser raw_packet_parser(hdr_type, cap_len);
//	starflow::kernels::CLFRTable clfr_table;
//	starflow::kernels::HeavyHitter heavy_hitter;

//	auto incomplete_evict_policy = starflow::kernels::CLFRTable::incomplete_evict_policy::to;
//	clfr_table.set_incomplete_evict_policy(incomplete_evict_policy);
//	clfr_table.set_incomplete_evict_pkt_count(10);
//	clfr_table.set_incomplete_evict_to(std::chrono::seconds(15));

	raft::map m;
	m += pcap_file_reader >> raw_packet_parser >> packet_printer;
//	m += tzsp_receiver >> raw_packet_parser >> packet_printer;
	m.exe();

	return 0;
}
