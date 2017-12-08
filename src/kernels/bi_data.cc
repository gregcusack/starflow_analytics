#include <utility>

#include "bi_data.h"
#include <cfloat>
#include <iterator>
#include <math.h>
#include <string>

starflow::kernels::BiData::BiData() : raft::kernel() {
	input.add_port<types::CLFR>("clfr_out_flow");
	input.add_port<types::CLFR>("clfr_in_flow");
}

raft::kstatus starflow::kernels::BiData::run() {
	types::CLFR clfr_in;
	types::CLFR clfr_out;
	input["clfr_in_flow"].pop(clfr_in);
	input["clfr_out_flow"].pop(clfr_out);
	types::Key key_in = clfr_in.key();
	types::Key key_out = clfr_out.key();
	/*
	std::cout << "key_in_proto: " << clfr_in.key().get_proto() << std::endl;
	std::cout << "key_in_src : " << clfr_in.key().get_ip_src() << std::endl;
	std::cout << "key_in_dst : " << clfr_in.key().get_ip_dst() << std::endl;
	std::cout << "key_in_sport : " << clfr_in.key().get_sport() << std::endl;
	std::cout << "key_in_dport : " << clfr_in.key().get_dport() << std::endl;
	std::cout << "-------------------------------------------------" << std::endl;
	std::cout << "key_out_proto: " << clfr_out.key().get_proto() << std::endl;
	std::cout << "key_out_src: " << clfr_out.key().get_ip_src() << std::endl;
	std::cout << "key_out_dst: " << clfr_out.key().get_ip_dst() << std::endl;
	std::cout << "key_out_sport : " << clfr_out.key().get_sport() << std::endl;
	std::cout << "key_out_dport : " << clfr_out.key().get_dport() << std::endl;
	std::cout << "#################################################" << std::endl;
	std::cout << "#################################################" << std::endl;
	*/
	//std::cout << "here" << std::endl;
	std::list<types::Packet> i_clfr_pkts = clfr_in.packets();
	std::list<types::Packet> o_clfr_pkts = clfr_out.packets();
	std::list<std::pair<types::Packet, bool>> i_pairs, o_pairs;
	for(auto const& itr : i_clfr_pkts) {
		i_pairs.push_back(std::make_pair(itr, 1));
	}
	for(auto const& itr : o_clfr_pkts) {
		o_pairs.push_back(std::make_pair(itr, 0));
	}

	unsigned long i_num_packs = clfr_in.n_packets();
	unsigned long o_num_packs = clfr_out.n_packets();
	if(i_num_packs != 1 && o_num_packs != 1) {
		unsigned long i_num_bytes = clfr_in.n_bytes();
		unsigned long i_flow_dur = flow_duration(i_clfr_pkts);
		unsigned long i_min_ia_time = min_interarrival_time(i_clfr_pkts);
		unsigned long i_mean_ia_time = mean_interarrival_time(i_clfr_pkts);
		unsigned long i_max_ia_time = max_interarrival_time(i_clfr_pkts);
		unsigned long i_stddev_ia_time = stddev_interarrival_time(i_clfr_pkts);
		unsigned long i_min_pkt_len = min_packet_length(i_clfr_pkts);
		unsigned long i_mean_pkt_len = mean_packet_length(i_clfr_pkts);
		unsigned long i_max_pkt_len = max_packet_length(i_clfr_pkts);
		unsigned long i_stddev_pkt_len = stddev_packet_length(i_clfr_pkts);
		
		
		unsigned long o_num_bytes = clfr_out.n_bytes();
		unsigned long o_flow_dur = flow_duration(o_clfr_pkts);
		unsigned long o_min_ia_time = min_interarrival_time(o_clfr_pkts);
		unsigned long o_mean_ia_time = mean_interarrival_time(o_clfr_pkts);
		unsigned long o_max_ia_time = max_interarrival_time(o_clfr_pkts);
		unsigned long o_stddev_ia_time = stddev_interarrival_time(o_clfr_pkts);
		unsigned long o_min_pkt_len = min_packet_length(o_clfr_pkts);
		unsigned long o_mean_pkt_len = mean_packet_length(o_clfr_pkts);
		unsigned long o_max_pkt_len = max_packet_length(o_clfr_pkts);
		unsigned long o_stddev_pkt_len = stddev_packet_length(o_clfr_pkts);

		//Outflow/Inflow
		double biflow_rat = static_cast<double>(o_num_packs)/static_cast<double>(i_num_packs);
		double byte_rat = static_cast<double>(o_num_bytes)/static_cast<double>(i_num_bytes);
		//std::cout << biflow_rat << std::endl;
		
		bool toggle; 
		std::array<float, 3> i_data_arr;
		i_data_arr = min_mean_max_burst_length(i_pairs, o_pairs, i_num_packs, o_num_packs, i_data_arr, toggle=0);
		//std::cout << "out burst: [" << i_data_arr[0] << ", " << i_data_arr[1] << ", " << i_data_arr[2] << "]" << std::endl;
		
		std::array<float, 3> o_data_arr;
		o_data_arr = min_mean_max_burst_length(i_pairs, o_pairs, i_num_packs, o_num_packs, o_data_arr, toggle=1);
		//std::cout << "in burst : [" << o_data_arr[0] << ", " << o_data_arr[1] << ", " << o_data_arr[2] << "]" << std::endl;
		//std::cout << "-------------------------------------------------" << std::endl;

		std::ofstream data_file;
		data_file.open("data_file.csv",std::ios_base::app);
		data_file << key_out.str_desc_for_df() << "," << i_num_packs;
		data_file << "," << i_num_bytes << "," << i_flow_dur;
		data_file << "," << i_min_ia_time << "," << i_mean_ia_time;
		data_file << "," << i_max_ia_time;
		data_file << "," << i_stddev_ia_time;
		data_file << "," << i_min_pkt_len;
		data_file << "," << i_mean_pkt_len;
		data_file << "," << i_max_pkt_len;
		data_file << "," << i_stddev_pkt_len;
		data_file << "," << i_data_arr[0] << "," << i_data_arr[1];
		data_file << "," << i_data_arr[2];

		data_file << "," << o_num_packs;
		data_file << "," << o_num_bytes << "," << o_flow_dur;
		data_file << "," << o_min_ia_time << "," << o_mean_ia_time;
		data_file << "," << o_max_ia_time;
		data_file << "," << o_stddev_ia_time;
		data_file << "," << o_min_pkt_len;
		data_file << "," << o_mean_pkt_len;
		data_file << "," << o_max_pkt_len;
		data_file << "," << o_stddev_pkt_len;
		data_file << "," << o_data_arr[0] << "," << o_data_arr[1];
		data_file << "," << o_data_arr[2];

		data_file << "," << biflow_rat;
		data_file << "," << byte_rat;
		data_file << std::endl;
	}

	return (raft::proceed);
}

/* Flow Duration */
float starflow::kernels::BiData::flow_duration(const std::list<types::Packet>& packets) const {
	return std::chrono::duration_cast<std::chrono::microseconds>(packets.back().ts - packets.front().ts).count();
}

/* Min. Interarrival Time */
float starflow::kernels::BiData::min_interarrival_time(const std::list<types::Packet>& packets) const {
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
float starflow::kernels::BiData::mean_interarrival_time(const std::list<types::Packet>& packets) const {
	if(packets.size() == 1)
		return 0;
	std::chrono::microseconds prev;
	float total_diff = 0;
	bool flag = true;
	for(auto const& itr : packets) {
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
float starflow::kernels::BiData::max_interarrival_time(const std::list<types::Packet>& packets) const {
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


float starflow::kernels::BiData::stddev_interarrival_time(const std::list<types::Packet>& packets) const {
	if(packets.size() == 1)
		return 0;	
	float diff;
	float mean = starflow::kernels::BiData::mean_interarrival_time(packets);
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

unsigned long starflow::kernels::BiData::min_packet_length(const std::list<types::Packet>& packets) const {
	unsigned long min = ULONG_MAX;
	bool flag = false;
	unsigned long tmp;
	for(auto const& itr : packets) {
		//std::cout << "len: " << itr.len << std::endl;
		if(itr.len <= 60) {
			flag = true;
			tmp = itr.len;
			continue;
		}
		if(itr.len < min)
			min = itr.len;
	}
	if(min == ULONG_MAX && flag) {
		min = tmp;
	}
	return min;
}

/* Mean Packet Length */
unsigned long starflow::kernels::BiData::mean_packet_length(const std::list<types::Packet>& packets) const {
	unsigned long total_len = 0;
	for(auto const& itr : packets)
		total_len += itr.len;
	return total_len/packets.size();
}

unsigned long starflow::kernels::BiData::max_packet_length(const std::list<types::Packet>& packets) const {
	unsigned long max = 0;
	for(auto const& itr : packets) {
		if(itr.len > max)
			max = itr.len;
	}
	return max;
}

unsigned long starflow::kernels::BiData::stddev_packet_length(const std::list<types::Packet>& packets) const {
	unsigned long mean;
	unsigned long stddev = 0;
	mean = starflow::kernels::BiData::mean_packet_length(packets);
	for(auto const& itr : packets)
		stddev += pow((long)(itr.len - mean), 2);
	return sqrt(stddev/packets.size());
}

void starflow::kernels::BiData::merge_packets(const std::list<std::pair<types::Packet, bool>>& i_pairs, const std::list<std::pair<types::Packet, bool>>& o_pairs,
	const unsigned long i_num_packs, const unsigned long o_num_packs, std::list<std::pair<types::Packet, bool>>& for_sort) const {
	for(auto const& i : i_pairs) {
		for_sort.push_back(i);
	}
	for(auto const& i : o_pairs) {
		for_sort.push_back(i);
	}
}

bool cmp_by_ts(const std::pair<starflow::types::Packet, bool>& i_pair, const std::pair<starflow::types::Packet, bool>& o_pair) {
	return i_pair.first.ts.count() < o_pair.first.ts.count();
}

void starflow::kernels::BiData::sort_packets(const std::list<std::pair<types::Packet, bool>>& i_pairs, const std::list<std::pair<types::Packet, bool>>& o_pairs,
	const unsigned long i_num_packs, const unsigned long o_num_packs, std::list<std::pair<types::Packet, bool>>& to_sort) const {
	starflow::kernels::BiData::merge_packets(i_pairs, o_pairs, i_num_packs, o_num_packs, to_sort);
	to_sort.sort(cmp_by_ts);
}

std::array<float, 3> starflow::kernels::BiData::min_mean_max_burst_length(const std::list<std::pair<types::Packet, bool>>& i_pairs, const std::list<std::pair<types::Packet, bool>>& o_pairs,
	const unsigned long i_num_packs, const unsigned long o_num_packs, std::array<float, 3> data_arr, const bool toggle) const {
	std::list<std::pair<types::Packet, bool>> sort_list;
	starflow::kernels::BiData::sort_packets(i_pairs, o_pairs, i_num_packs, o_num_packs, sort_list);
	float min_burst = FLT_MAX;
	float max_burst = 0;
	float out_count = 0;
	float in_count = 0;
	std::list<float> burst_lens;
	for(auto const& itr : sort_list) {
		if(itr.second == !toggle) { //1 is in flow
			if(out_count == 0)
				continue;
			in_count++;
			if(in_count == 2) { //back to back in flows, end burst
				if(out_count < min_burst)
					min_burst = out_count;
				if(out_count > max_burst)
					max_burst = out_count;
				burst_lens.push_back(out_count);
				out_count = 0;
				in_count = 0;
			}
		}
		out_count++;
	}
	float mean_burst = 0;
	if(min_burst == FLT_MAX)
		min_burst = 0;
	if(burst_lens.size() != 0) {
		float sum_bursts = 0;
		for(auto const& itr : burst_lens) {
			sum_bursts += itr;
		}
		mean_burst = sum_bursts/burst_lens.size();
	}
	data_arr[0] = min_burst;
	data_arr[1] = mean_burst;
	data_arr[2] = max_burst;
	return data_arr;
}



/*for(auto const& itr : sort_list) {
	std::cout << "timestamp: " << itr.first.ts.count() << std::endl;
}
std::cout << "-------------------------------------------------" << std::endl;
*/







