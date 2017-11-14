
#ifndef STARFLOW_KERNELS_SINK
#define STARFLOW_KERNELS_SINK

#include "../types/key.h"
#include "../types/clfr.h"
#include <string>
#include <iostream>
#include <fstream>
#include <raft>

namespace starflow {
	namespace kernels {
		class Data : public raft::kernel {
		public:
			explicit Data();
			raft::kstatus run() override;
		private:
			/* Feature Extraction */
			unsigned long flow_duration(const std::list<types::Packet>& packets) const;				//Done
			unsigned long min_interarrival_time(const std::list<types::Packet>& packets) const;		//Done
			unsigned long mean_interarrival_time(const std::list<types::Packet>& packets) const;	//Done
			unsigned long max_interarrival_time(const std::list<types::Packet>& packets) const;		//Done
			unsigned long stddev_interarrival_time(const std::list<types::Packet>& packets) const;
			unsigned long min_packet_length(const std::list<types::Packet>& packets) const;
			unsigned long mean_packet_length(const std::list<types::Packet>& packets) const;		//Done
			unsigned long max_packet_length(const std::list<types::Packet>& packets) const;
			unsigned long stddev_packet_length(const std::list<types::Packet>& packets) const;
		
		};
	}
}

#endif
