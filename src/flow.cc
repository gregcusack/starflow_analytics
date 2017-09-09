
#include "flow.h"

#include <sstream>
#include <iomanip>

starflow::Flow::packet_t::packet_t(std::chrono::microseconds ts, unsigned size, features_t features)
	: ts(ts), len(size), features(features) { }

void starflow::Flow::add_packet(std::chrono::microseconds ts, unsigned len,
								packet_t::features_t features)
{
	if (_packets.empty())
		_start_ts = ts;

	_packets.emplace_back(ts, len, features);
	_recent_ts = ts;
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
