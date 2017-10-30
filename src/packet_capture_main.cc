
#include <raft>

#include "proto/starflow.pb.h"
#include "kernels/live_capture.h"
#include "kernels/printer.h"
#include "raw_packet.h"
#include "kernels/pcap_file_reader.h"

int main(int argc, char** argv)
{
	starflow::kernels::LiveCapture live_capture("en10");
	starflow::kernels::PCAPFileReader pcap_file_reader("caida2015_02_dirA.pcap");
	starflow::kernels::Printer<starflow::RawPacket> printer;
	raft::map m;

	m += pcap_file_reader >> printer;

//	m += live_capture >> printer;

	m.exe();

	return 0;
}
