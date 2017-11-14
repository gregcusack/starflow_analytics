#include <utility>

#include "data.h"
#include <climits>
#include <iterator>

starflow::kernels::Data::Data() : raft::kernel() {
	input.add_port<std::pair<types::Key, types::CLFR>>("in");
}

raft::kstatus starflow::kernels::Data::run() {
	std::pair<types::Key, types::CLFR> k_c_pair;
	input["in"].pop(k_c_pair);
	types::Key key = k_c_pair.first;
	types::CLFR clfr = k_c_pair.second;

	unsigned long num_packs = clfr.n_packets();
	unsigned long num_bytes = clfr.n_bytes();
	std::string ip = key.str_desc();    

	std::list<types::Packet> clfr_pkts = clfr.packets();
	unsigned long flow_dur = flow_duration(clfr_pkts);
	unsigned long min_ia_time = min_interarrival_time(clfr_pkts);
	unsigned long mean_ia_time = mean_interarrival_time(clfr_pkts);
	unsigned long mean_pkt_len = mean_packet_length(clfr_pkts);

	//implement this as a member function that just prints as
	//as comma separated values to be printed to CSV
	std::ofstream data_file;
	data_file.open("data_file.csv",std::ios_base::app);
	data_file << key.str_desc_for_df() << "," << num_packs;
	data_file << "," << num_bytes << "," << flow_dur;
	data_file << "," << min_ia_time << "," << mean_ia_time;
	data_file << "," << mean_pkt_len;
	data_file << std::endl;
	
	return (raft::proceed);
}

unsigned long starflow::kernels::Data::flow_duration(const std::list<types::Packet>& packets) const {
	return std::chrono::duration_cast<std::chrono::microseconds>(packets.back().ts - packets.front().ts).count();
}

unsigned long starflow::kernels::Data::min_interarrival_time(const std::list<types::Packet>& packets) const {
	unsigned long diff = ULONG_MAX;
	unsigned long tmp;
	std::list<starflow::types::Packet>::const_iterator itr;
	if(packets.size() == 1)
		return 0;
	for(itr = packets.begin(); itr != packets.end(); ++itr) {
		if((itr != packets.end()) && (next(itr) == packets.end()))
			return diff;
		tmp = std::chrono::duration_cast<std::chrono::microseconds>(next(itr)->ts - itr->ts).count(); //maybe make it count() only once at end...???
		if(tmp < diff)
			diff = tmp;
	}
	return diff;	
}

unsigned long starflow::kernels::Data::mean_interarrival_time(const std::list<types::Packet>& packets) const {
	std::chrono::microseconds prev;
	unsigned long total_diff = 0;
	bool flag = true;
	for(auto const& itr : packets) {
		//better way to do this??  likely/unlikely macros???
		if(flag) {
			prev = itr.ts;
			flag = false;
			continue;
		}
		total_diff += std::chrono::duration_cast<std::chrono::microseconds>(itr.ts - prev).count();
		prev = itr.ts;
	}
	return total_diff/packets.size();
}

unsigned long starflow::kernels::Data::mean_packet_length(const std::list<types::Packet>& packets) const {
	unsigned long total_len = 0;
	for(auto const& itr : packets)
		total_len += itr.len;
	return total_len/packets.size();
}
