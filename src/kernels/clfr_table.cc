
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

	auto i = _flows.find(key);

	if (i == std::end(_flows))
		i = _flows.emplace(key, types::CLFR{}).first;

	i->second.add_packet(packet);

	if (key.ip_proto == IPPROTO_TCP && packet.features.tcp_flags.is_fin()) {
		_evict_flow(i, packet.ts, true);
	} else if (_incomplete_evict_policy == incomplete_evict_policy::pkt_count
		&& i->second.n_packets() >= _incomplete_evict_pkt_count) {
		_evict_flow(i, packet.ts, false);
	}

	// set last t/o check to current packet ts on very first packet
	// (avoid t/o check on empty table)
	if (_n_packets_processed++ == 0)
		_last_to_check = packet.ts;

	if ((packet.ts.count() - _last_to_check.count()) > _to_check_interval.count())
		_check_timeouts(packet.ts);

	return raft::proceed;
}

void starflow::kernels::CLFRTable::set_to_check_interval(std::chrono::seconds to)
{
	_to_check_interval = to;
}

void starflow::kernels::CLFRTable::set_udp_timeout(std::chrono::seconds to)
{
	_udp_to = to;
}

void starflow::kernels::CLFRTable::set_incomplete_evict_policy(incomplete_evict_policy p)
{
	_incomplete_evict_policy = p;
}

void starflow::kernels::CLFRTable::set_incomplete_evict_to(std::chrono::seconds to)
{
	_incomplete_evict_to = to;

	if (_to_check_interval > _incomplete_evict_to)
		_to_check_interval = _incomplete_evict_to;
}

void starflow::kernels::CLFRTable::set_incomplete_evict_pkt_count(unsigned long c)
{
	_incomplete_evict_pkt_count = c;
}

void starflow::kernels::CLFRTable::_check_timeouts(std::chrono::microseconds trigger_ts)
{
	for (auto i = std::begin(_flows); i != std::end(_flows);) {

		// in-place removal requires setting iterator manually
		long long int since_last_packet
			= (trigger_ts.count() - i->second.last_packet().ts.count());

		if (i->first.ip_proto == IPPROTO_UDP && since_last_packet >= _udp_to.count()) {
			i = _evict_flow(i, trigger_ts, true);
		} else if (_incomplete_evict_policy == incomplete_evict_policy::to
				 && since_last_packet >= _incomplete_evict_to.count()) {
			i = _evict_flow(i, trigger_ts, false);
		} else {
			i = std::next(i, 1);
		}
	}

	_last_to_check = trigger_ts;
}

starflow::kernels::CLFRTable::flow_table_t::iterator
	starflow::kernels::CLFRTable::_evict_flow(const flow_table_t::iterator& i,
		std::chrono::microseconds evict_ts, bool complete)
{
	i->second._complete = complete;
	i->second._evict_ts = evict_ts;
	output["clfr_out"].push(*i);
	return _flows.erase(i);
}
