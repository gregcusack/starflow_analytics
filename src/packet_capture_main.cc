
#include <raft>
#include <utility>

#include "kernels/live_capture.h"
#include "kernels/pcap_file_reader.h"
#include "kernels/printer.h"
#include "kernels/raw_packet_parser.h"
#include "proto/starflow.pb.h"
#include "types/key.h"
#include "types/packet.h"

int main(int argc, char** argv)
{
	auto hdr_type = starflow::kernels::RawPacketParser::outer_header_type::eth;

	raft::lambdak<std::pair<starflow::types::Key, starflow::types::Packet>>
		printer(1, 0, [&] (Port& in, Port& out) {
		std::pair<starflow::types::Key, starflow::types::Packet> p {};
		in["0"].pop(p);
		std::cout << p.first.str_desc() << " " << p.second.str_desc() << std::endl;
		return raft::proceed;
	});

	starflow::kernels::LiveCapture live_capture("en10");
	starflow::kernels::PCAPFileReader pcap_file_reader("caida2015_02_dirA.pcap");
	starflow::kernels::RawPacketParser raw_packet_parser(hdr_type);

	raft::map m;
	m += live_capture >> raw_packet_parser >> printer;
	m.exe();

	return 0;
}
