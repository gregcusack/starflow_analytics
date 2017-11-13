
#ifndef STARFLOW_KERNELS_SINK
#define STARFLOW_KERNELS_SINK

#include "../types/key.h"
#include "../types/clfr.h"
#include <string>
#include <iostream>
#include <fstream>

namespace starflow {
	namespace kernels {
		template<typename T>
		class Sink : public raft::kernel
		{
		public:
			explicit Sink()
				: raft::kernel()
			{
				input.template add_port<T>("in");
			}

			raft::kstatus run() override
			{
				//int count = 0;
				//T t{};
				//input["in"].pop(t);
				std::pair<types::Key, types::CLFR> k_c_pair;
				input["in"].pop(k_c_pair);
				types::Key key = k_c_pair.first;
				types::CLFR clfr = k_c_pair.second;

				unsigned long num_packs = clfr.n_packets();
				unsigned long num_bytes = clfr.n_bytes();
				std::string ip = key.str_desc();	
				//std::cout << "Flow: " << ip << ", # packs: " << num_packs << ", # bytes: " << num_bytes << std::endl;
				
				//so pretty sure this returns an address rather than dublicating the whole clfr
				//std::list<types::Packet> clfr_pkts = clfr.get_pkt_list();
				unsigned long avg_ts = clfr.mean_interarrival_time();
				unsigned long avg_pkt_len = clfr.mean_packet_length();
				unsigned long flow_duration = clfr.flow_duration();	
				unsigned long min_ia_time  = clfr.min_interarrival_time();
				
				std::ofstream data_file;
				data_file.open("data_file.csv",std::ios_base::app);
				data_file << key.str_desc_for_df() << "," << num_packs;
			    data_file << "," << num_bytes << "," << avg_ts << ",";
			    data_file << avg_pkt_len << "," << flow_duration;
			    data_file << "," << min_ia_time <<  std::endl;
				
				//how do i access list of packets in the CLFR?
				//_packets is a private variable of CLFR and there is no getter...


				return (raft::proceed);
			}
		};
	}
}

#endif
