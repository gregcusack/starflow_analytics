
#include <utility>

#include "clfr_table.h"

starflow::kernels::CLFRTable::CLFRTable()
	: _flows()
{
	input.add_port<std::pair<types::Key, types::Packet>>("in");
}

raft::kstatus starflow::kernels::CLFRTable::run()
{
	std::pair<types::Key, types::Packet> k_p_pair;
	input["in"].pop(k_p_pair);

	types::Key key       = k_p_pair.first;
	types::Packet packet = k_p_pair.second;


	if (_flows.find(key) == std::end(_flows)) {
		_flows[key] = {};
		std::cout << "new flow: " << k_p_pair.first.str_desc() << std::endl;
	} else {
		std::cout << "existing flow" << std::endl;
	}

	_flows[key].add_packet(packet);

	return raft::proceed;
}
