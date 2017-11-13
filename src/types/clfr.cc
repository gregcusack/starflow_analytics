
#include "clfr.h"

#include <numeric>
#include <iterator>
#include <climits>

void starflow::types::CLFR::add_packet(Packet p)
{
	_packets.push_back(p);
}

const starflow::types::Packet& starflow::types::CLFR::last_packet() const
{
	return _packets.back();
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

//gives us access to packet data in a clfr
std::list<starflow::types::Packet> &starflow::types::CLFR::get_pkt_list() {
	return _packets;
}

unsigned long starflow::types::CLFR::min_interarrival_time() const {
	unsigned long diff = ULONG_MAX; //may be better way to do this
	unsigned long tmp;
	std::list<starflow::types::Packet>::const_iterator itr;
	if(_packets.size() == 1)
		return 0;
	for(itr = _packets.begin(); itr != _packets.end(); ++itr) {
		if((itr != _packets.end()) && (next(itr) == _packets.end()))
			return diff;
		tmp = std::chrono::duration_cast<std::chrono::microseconds>(next(itr)->ts - itr->ts).count(); //may be able to make it count() only once at end...
		if(tmp < diff) 
		   diff = tmp;
	}
	return diff;
}

unsigned long starflow::types::CLFR::mean_interarrival_time() const {
	std::chrono::microseconds prev;
	unsigned long total_diff = 0;
	bool flag = true;
	for(auto const& itr : _packets) {	
		//may be better way to do this,, if flows are short, then this will be bad
		if(flag) {  //should i use unlikely() macro here?
			prev = itr.ts;
			flag = false;
			continue;
		}
		total_diff += std::chrono::duration_cast<std::chrono::microseconds>(itr.ts - prev).count();
		prev = itr.ts;
	}
	return total_diff/_packets.size();
}

unsigned long starflow::types::CLFR::mean_packet_length() const {
	unsigned long total_len = 0;
	for(auto const& itr : _packets)
		total_len += itr.len;
	return total_len/_packets.size();
}

unsigned long starflow::types::CLFR::flow_duration() const {
	return std::chrono::duration_cast<std::chrono::microseconds>(_packets.back().ts - _packets.front().ts).count(); 
}
