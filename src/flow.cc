
#include "flow.h"

#include <sstream>
#include <iomanip>

starflow::proto::features starflow::Flow::packet_t::features_t::to_proto() const
{
	proto::features f {};
	f.set_ip_ttl(ttl);
	f.set_q_len(q_len);
	return f;
}

starflow::Flow::packet_t::features_t
starflow::Flow::packet_t::features_t::from_proto(const starflow::proto::features& p)
{
	features_t f {};
	f.q_len = (unsigned) p.q_len();
	f.ttl   = (unsigned) p.ip_ttl();
	return f;
}

starflow::Flow::packet_t::packet_t(std::chrono::microseconds ts, unsigned size, features_t features)
	: ts(ts), len(size), features(features) { }

starflow::proto::packet starflow::Flow::packet_t::to_proto() const
{
	proto::packet p {};
	proto::features* p_f = new proto::features(features.to_proto());
	p.set_len(len);
	p.set_ts(ts.count());
	p.set_allocated_features(p_f);
	return p;
}

starflow::Flow::packet_t starflow::Flow::packet_t::from_proto(const starflow::proto::packet& proto)
{
	packet_t packet {};
	packet.ts  = std::chrono::microseconds(proto.ts());
	packet.len = (unsigned) proto.len();
	packet.features = features_t::from_proto(proto.features());
	return packet;
}

void starflow::Flow::add_packet(std::chrono::microseconds ts, unsigned len,
								packet_t::features_t features)
{
	if (_packets.empty())
		_start_ts = ts;

	_packets.emplace_back(ts, len, features);
	_recent_ts = ts;
}

const std::list<starflow::Flow::packet_t> starflow::Flow::packets() const
{
	return _packets;
}

unsigned long starflow::Flow::n_packets() const
{
	return _packets.size();
}

std::chrono::microseconds starflow::Flow::start_ts() const
{
	return _start_ts;
}

std::chrono::microseconds starflow::Flow::recent_ts() const
{
	return _recent_ts;
}

void starflow::Flow::set_eviction_ts(std::chrono::microseconds ts)
{
	_eviction_ts = ts;
}

std::chrono::microseconds starflow::Flow::eviction_ts() const
{
	return _eviction_ts;
}

void starflow::Flow::set_complete()
{
	_complete = true;
}

bool starflow::Flow::complete() const
{
	return _complete;
}

std::string starflow::Flow::str_desc() const
{
	std::stringstream ss;
	ss << std::setw(3) << _packets.size() << " pkts";
	return ss.str();
}

starflow::proto::flow starflow::Flow::to_proto() const
{
	starflow::proto::flow p_flow;
	p_flow.set_evict_ts(_eviction_ts.count());

	for (const packet_t& p : _packets) {
		starflow::proto::packet* p_packet = p_flow.add_packets();
		*p_packet = p.to_proto();
	}

	return p_flow;
}

starflow::Flow starflow::Flow::from_proto(const starflow::proto::flow& p_flow)
{
	Flow flow {};
	flow._eviction_ts = std::chrono::microseconds(p_flow.evict_ts());

	for (auto& p_packet : p_flow.packets())
		flow._packets.push_back(packet_t::from_proto(p_packet));

	return flow;
}
