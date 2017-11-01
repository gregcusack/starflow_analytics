
#include <utility>

#include "clfr_table.h"

starflow::kernels::CLFRTable::CLFRTable()
	: _flows()
{
	input.add_port<std::pair<types::Key, types::Packet>>("packet_in");
	output.add_port<std::pair<types::Key, types::CLFR>>("clfr_out");
}

raft::kstatus starflow::kernels::CLFRTable::run()
{
	std::pair<types::Key, types::Packet> k_p_pair;
	input["packet_in"].pop(k_p_pair);

	types::Key key       = k_p_pair.first;
	types::Packet packet = k_p_pair.second;

	if (_flows.find(key) == std::end(_flows))
		_flows[key] = {};

	_flows[key].add_packet(packet);

	if (key.ip_proto == IPPROTO_TCP && packet.features.tcp_flags.is_fin()) {
		output["clfr_out"].push(std::make_pair(key, _flows[key]));
		_flows.erase(key);
	}

	return raft::proceed;
}
