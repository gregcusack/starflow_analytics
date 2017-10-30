
#include <raft>

#include "proto/starflow.pb.h"
#include "kernels/live_capture.h"
#include "kernels/printer.h"
#include "raw_packet.h"

int main(int argc, char** argv)
{
	starflow::kernels::LiveCapture live_capture("en10");
	starflow::kernels::Printer<starflow::RawPacket> printer;
	raft::map m;

	m += live_capture >> printer;

	m.exe();

	return 0;
}
