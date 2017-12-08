
#ifndef STARFLOW_KERNELS_BI_DATA
#define STARFLOW_KERNELS_BI_DATA

#include "../types/key.h"
#include "../types/clfr.h"
#include <string>
#include <iostream>
#include <fstream>
#include <raft>

namespace starflow {
	namespace kernels {
		class BiData : public raft::kernel {
		public:
			explicit BiData();
			raft::kstatus run() override;
		private:
			/* Feature Extraction */
			float flow_duration(const std::list<types::Packet>& packets) const;				//Done
			float min_interarrival_time(const std::list<types::Packet>& packets) const;		//Done
			float mean_interarrival_time(const std::list<types::Packet>& packets) const;	//Done
			float max_interarrival_time(const std::list<types::Packet>& packets) const;		//Done
			float stddev_interarrival_time(const std::list<types::Packet>& packets) const;	//Done
			unsigned long min_packet_length(const std::list<types::Packet>& packets) const;			//Done
			unsigned long mean_packet_length(const std::list<types::Packet>& packets) const;		//Done
			unsigned long max_packet_length(const std::list<types::Packet>& packets) const;			//Done
			unsigned long stddev_packet_length(const std::list<types::Packet>& packets) const;		//Done
			//ratio, burst
			void merge_packets(const std::list<std::pair<types::Packet, bool>>& i_pairs, const std::list<std::pair<types::Packet, bool>>& o_pairs,
				const unsigned long i_num_packs, const unsigned long o_num_packs, std::list<std::pair<types::Packet, bool>>& for_sort) const;
			void sort_packets(const std::list<std::pair<types::Packet, bool>>& i_pairs, const std::list<std::pair<types::Packet, bool>>& o_pairs,
				const unsigned long i_num_packs, const unsigned long o_num_packs, std::list<std::pair<types::Packet, bool>>& to_sort) const;
			//TODO: in_burst and out_burst
			std::array<float, 3> min_mean_max_burst_length(const std::list<std::pair<types::Packet, bool>>& i_pairs, const std::list<std::pair<types::Packet, bool>>& o_pairs,
				const unsigned long i_num_packs, const unsigned long o_num_packs, std::array<float, 3> data_arr, const bool toggle) const;
		};
	}
}

#endif
