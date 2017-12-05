
#include "flow_table.h"

starflow::kernels::FlowTable::FlowTable()
	: _flow_table()
{
	input.add_port<std::pair<types::Key, types::Packet>>("packet_in");
	output.add_port<std::pair<types::Key, types::CLFR>>("clfr_out");

	_flow_table.set_mode(modules::FlowTable::mode::callback);
	_flow_table.set_callback([this](types::Key k, types::CLFR f) {
		this->_flow_export(k, f);
	});
}

raft::kstatus starflow::kernels::FlowTable::run()
{
	std::pair<types::Key, types::Packet> k_p_pair;
	input["packet_in"].pop(k_p_pair);
	_flow_table.add_packet(k_p_pair);
	return raft::proceed;
}

void starflow::kernels::FlowTable::_flow_export(starflow::types::Key k, starflow::types::CLFR f)
{
	output["clfr_out"].push(std::make_pair(k, f));
}
