
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

	if (key.ip_proto == IPPROTO_TCP && packet.features.tcp_flags.is_fin())
		_evict_flow(key);

	if ((packet.ts.count() - _last_to_check.count()) > _to_check_interval.count())
		_check_timeouts(packet.ts);

	return raft::proceed;
}

void starflow::kernels::CLFRTable::_check_timeouts(std::chrono::microseconds trigger_ts)
{
//  TODO: implement
//	void starflow::FlowTable::_check_flow_timeouts(std::chrono::microseconds ts)
//	{
//		for (auto& f : _flows) {
//
//			long long int since_last_packet = (ts.count() - f.second.recent_ts().count()) / 1000000;
//
//			if (f.first.ip_proto == IPPROTO_TCP && since_last_packet >= _tcp_timeout.count())
//				_evict_flow(f.first, ts, eviction_type::tcp_to);
//			else if(f.first.ip_proto == IPPROTO_UDP && since_last_packet >= _udp_timeout.count())
//				_evict_flow(f.first, ts, eviction_type::udp_to);
//		}
//
//		_last_timeout_check = ts;
//	}

	_last_to_check = trigger_ts;
}

void starflow::kernels::CLFRTable::_evict_flow(const types::Key& key)
{
	output["clfr_out"].push(std::make_pair(key, _flows[key]));
	_flows.erase(key);
}
