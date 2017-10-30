
#include <raft>

#include "proto/starflow.pb.h"
#include "kernels/packet_capture.h"
#include "kernels/printer.h"

int main(int argc, char** argv)
{
	starflow::kernels::PacketCapture packet_capture("en10");
	starflow::kernels::Printer<starflow::proto::raw_packet> printer;
	raft::map m;

	m += packet_capture >> printer;

	m.exe();

	return 0;
}
