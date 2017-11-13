
#include "clfr.h"

#include <numeric>

void starflow::types::CLFR::add_packet(Packet p)
{
	_packets.push_back(p);
}

const starflow::types::Packet& starflow::types::CLFR::last_packet() const
{
	return _packets.back();
}

const std::list<starflow::types::Packet>& starflow::types::CLFR::packets() const
{
	return _packets;
}

std::string starflow::types::CLFR::str_desc() const
{
	std::string desc = "starflow::types::CLFR(status="
					   + (_complete ? std::string("complete") : std::string("incomplete"))
					   + ", n_packets=" + std::to_string(n_packets())
					   + ", n_bytes=" + std::to_string(n_bytes())
					   + ", evict_ts=" + std::to_string(_evict_ts.count() / 1000000) + ")";
	return desc;
}

bool starflow::types::CLFR::complete() const
{
	return _complete;
}

unsigned long starflow::types::CLFR::n_packets() const
{
	return _packets.size();
}

unsigned long starflow::types::CLFR::n_bytes() const
{
	return (unsigned long) std::accumulate(std::begin(_packets), std::end(_packets), 0,
		[](int s, const types::Packet& p) -> long { return s += p.len; });
}
