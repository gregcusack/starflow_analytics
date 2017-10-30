
#include <raft>

#include "proto/starflow.pb.h"
#include "kernels/live_capture.h"
#include "kernels/printer.h"
#include "types/raw_packet.h"
#include "kernels/pcap_file_reader.h"
#include "kernels/raw_packet_parser.h"

int main(int argc, char** argv)
{
	starflow::kernels::LiveCapture live_capture("en10");
	starflow::kernels::PCAPFileReader pcap_file_reader("caida2015_02_dirA.pcap");
	starflow::kernels::RawPacketParser raw_packet_parser;
	starflow::kernels::Printer<starflow::types::RawPacket> printer;
	raft::map m;

	m += pcap_file_reader >> raw_packet_parser;

//	m += live_capture >> printer;

	m.exe();

	return 0;
}
