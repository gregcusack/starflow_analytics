
#include "clfr.h"

#include <numeric>

void starflow::types::CLFR::add_packet(Packet p)
{
	_packets.push_back(p);
}

std::string starflow::types::CLFR::str_desc() const
{
	std::string desc = "starflow::types::CLFR(n_packets=" + std::to_string(n_packets())
					   + ", n_bytes=" + std::to_string(n_bytes()) + ")";
	return desc;
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
