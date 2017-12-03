#include <utility>

#include "end.h"
#include <cfloat>
#include <iterator>
#include <math.h>
#include <string>
#include "../types/raw_packet.h"

starflow::kernels::End::End() : raft::kernel() {
	//input.add_port<std::pair<types::Key, types::CLFR>>("in");
	input.add_port<std::pair<types::Key, types::Packet>>("packet_in");
	//input.add_port<types::RawPacket>("packet_in");
}

raft::kstatus starflow::kernels::End::run() {
/*	types::Key key;
	types::RawPacket raw_packet {};

	input["packet_in"].pop(raw_packet);
	types::Packet packet(raw_packet.ts, raw_packet.len);
*/
	std::pair<types::Key, types::Packet> dummy;
	input["packet_in"].pop(dummy);
	//types::Key key = k_p_pair.first;
	//types::CLFR clfr = k_p_pair.second;

	return (raft::proceed);
}



