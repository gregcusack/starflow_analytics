#include <utility>

#include "data.h"
#include <cfloat>
#include <iterator>
#include <math.h>
#include <string>

starflow::kernels::Data::Data() : raft::kernel() {
	//input.add_port<std::pair<types::Key, types::CLFR>>("in");
	input.add_port<types::CLFR>("in");
}

raft::kstatus starflow::kernels::Data::run() {
	//std::pair<types::Key, types::CLFR> k_c_pair;
	types::CLFR clfr;
	input["in"].pop(clfr);
	types::Key key = clfr.key();
	//std::cout << "key: " << clfr.key().str_desc() << std::endl;

	if(key.get_ip_src() == "10.12.16.101") {
		std::cout << "here" << std::endl;
	}
	
	unsigned long num_packs = clfr.n_packets();
	unsigned long num_bytes = clfr.n_bytes();
	std::string ip = key.str_desc();   

	std::list<types::Packet> clfr_pkts = clfr.packets();
	unsigned long flow_dur = flow_duration(clfr_pkts);
	unsigned long min_ia_time = min_interarrival_time(clfr_pkts);
	unsigned long mean_ia_time = mean_interarrival_time(clfr_pkts);
	unsigned long max_ia_time = max_interarrival_time(clfr_pkts);
	unsigned long stddev_ia_time = stddev_interarrival_time(clfr_pkts);
	unsigned long min_pkt_len = min_packet_length(clfr_pkts);
	unsigned long mean_pkt_len = mean_packet_length(clfr_pkts);
	unsigned long max_pkt_len = max_packet_length(clfr_pkts);
	unsigned long stddev_pkt_len = stddev_packet_length(clfr_pkts);
	
	//May want to add a check to make sure max >= min
	
	//implement this as a member function that just prints as
	//as comma separated values to be printed to CSV

	std::ofstream data_file;
	data_file.open("data_file.csv",std::ios_base::app);
	data_file << key.str_desc_for_df() << "," << num_packs;
	data_file << "," << num_bytes << "," << flow_dur;
	data_file << "," << min_ia_time << "," << mean_ia_time;
	data_file << "," << max_ia_time;
	data_file << "," << stddev_ia_time;
	data_file << "," << min_pkt_len;
	data_file << "," << mean_pkt_len;
	data_file << "," << max_pkt_len;
	data_file << "," << stddev_pkt_len;
	data_file << std::endl;
	data_file << std::endl;

	return (raft::proceed);
}

/* Flow Duration */
float starflow::kernels::Data::flow_duration(const std::list<types::Packet>& packets) const {
	return std::chrono::duration_cast<std::chrono::microseconds>(packets.back().ts - packets.front().ts).count();
}

/* Min. Interarrival Time */
float starflow::kernels::Data::min_interarrival_time(const std::list<types::Packet>& packets) const {
	if(packets.size() == 1)
		return 0;
	float diff = FLT_MAX;
	float tmp;
	std::list<starflow::types::Packet>::const_iterator itr;
	for(itr = packets.begin(); itr != packets.end(); ++itr) {
		if(next(itr) == packets.end())
			return diff;
		tmp = std::chrono::duration_cast<std::chrono::microseconds>(next(itr)->ts - itr->ts).count(); //maybe make it count() only once at end...???
		if(tmp < diff)
			diff = tmp;
	}
	return diff;	
}

/* Mean Interarrival Time */
float starflow::kernels::Data::mean_interarrival_time(const std::list<types::Packet>& packets) const {
	if(packets.size() == 1)
		return 0;
	std::chrono::microseconds prev;
	float total_diff = 0;
	bool flag = true;
	for(auto const& itr : packets) {
		//std::cout << "ts: " << std::chrono::microseconds(itr.ts).count() << std::endl;
		//better way to do this??  likely/unlikely macros???
		if(flag) {
			prev = itr.ts;
			flag = false;
			continue;
		}
		//std::cout << "diff: " << std::chrono::duration_cast<std::chrono::microseconds>(itr.ts - prev).count() << std::endl;
		total_diff += std::chrono::duration_cast<std::chrono::microseconds>(itr.ts - prev).count();
		prev = itr.ts;
	}
	//std::cout << "#pkts: " << packets.size() << std::endl;
	return total_diff/(packets.size()-1);
}

/* Max Interarrival Time */
float starflow::kernels::Data::max_interarrival_time(const std::list<types::Packet>& packets) const {
	if(packets.size() == 1)
		return 0;
	float diff = 0;
	float tmp;
	std::list<starflow::types::Packet>::const_iterator itr;
	for(itr = packets.begin(); itr != packets.end(); ++itr) {
		if(next(itr) == packets.end())
			return diff;
		tmp = std::chrono::duration_cast<std::chrono::microseconds>(next(itr)->ts - itr->ts).count();
		if(tmp > diff)
			diff = tmp;
	}
	return diff;
}


float starflow::kernels::Data::stddev_interarrival_time(const std::list<types::Packet>& packets) const {
	if(packets.size() == 1)
		return 0;	
	float diff;
	float mean = starflow::kernels::Data::mean_interarrival_time(packets);
	float stddev = 0;
	std::chrono::microseconds prev;
	bool flag = true;
	for(auto const& itr : packets) {
		//better way to do this??  likely/unlikely macros???
		if(flag) {
			prev = itr.ts;
			flag = false;
			continue;
		}
		diff = std::chrono::duration_cast<std::chrono::microseconds>(itr.ts - prev).count();
		stddev += pow((long)(diff - mean), 2);
		prev = itr.ts;
	}
	return sqrt(stddev/(packets.size()-1));

}

unsigned long starflow::kernels::Data::min_packet_length(const std::list<types::Packet>& packets) const {
	unsigned long min = packets.front().len;
	for(auto const& itr : packets) {
		//std::cout << "len: " << itr.len << std::endl;
		if(itr.len < min)
			min = itr.len;
	}
	return min;
}

/* Mean Packet Length */
unsigned long starflow::kernels::Data::mean_packet_length(const std::list<types::Packet>& packets) const {
	unsigned long total_len = 0;
	for(auto const& itr : packets)
		total_len += itr.len;
	return total_len/packets.size();
}

unsigned long starflow::kernels::Data::max_packet_length(const std::list<types::Packet>& packets) const {
	unsigned long max = 0;
	for(auto const& itr : packets) {
		if(itr.len > max)
			max = itr.len;
	}
	return max;
}

unsigned long starflow::kernels::Data::stddev_packet_length(const std::list<types::Packet>& packets) const {
	unsigned long mean;
	unsigned long stddev = 0;
	mean = starflow::kernels::Data::mean_packet_length(packets);
	for(auto const& itr : packets)
		stddev += pow((long)(itr.len - mean), 2);
	return sqrt(stddev/packets.size());
}










		//std::cout << "here" << std::endl;

	/*
	if(key.get_ip_src() == "209.124.66.6") {
		std::cout << "----------------begin----------" << std::endl;
		std::cout << key.get_ip_src() << std::endl;
		unsigned long mean_ia_time = mean_interarrival_time(clfr_pkts);
		unsigned long min_pkt_len = min_packet_length(clfr_pkts);
		for(auto const& itr : clfr_pkts) {
			std::cout << "-----" << std::endl;
			std::cout << itr.len << ", " << itr.ts.count() << std::endl;
			std::cout << itr.features.tcp_flags.is_ack() << std::endl;
			std::cout << itr.features.tcp_flags.is_syn() << std::endl;
			std::cout << itr.features.tcp_flags.is_psh() << std::endl;
			std::cout << itr.features.tcp_flags.is_fin() << std::endl;
			std::cout << "-----" << std::endl;
			//tcp_flags_t().str_desc() << std::endl;
		}
		std::cout << "----------------end----------" << std::endl;
	}
	*/
	
	/*
	if(key.ip_src == 1712523786 && key.ip_dst == 1253695156) {
		std::cout << key.ip_src << ", " << key.ip_dst << ", " << key.th_sport << ", " << key.th_dport << std::endl;
		std::cout << max_pkt_len << std::endl;
		std::cout << "num packets in clfr: " << clfr.n_packets() << std::endl;
		for(auto const& itr : clfr_pkts) {
			std::cout << itr.len << ", " << itr.ts.count() << std::endl;
			//std::cout << itr.features.tcp_flags.is_ack() << std::endl;
			//tcp_flags_t().str_desc() << std::endl;
		}
		//std::cout << clfr.packets().features.str_desc() << std::endl;
	}
	*/
