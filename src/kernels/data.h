
#ifndef STARFLOW_KERNELS_DATA
#define STARFLOW_KERNELS_DATA

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
			float flow_duration(const std::list<types::Packet>& packets) const;				//Done
			float min_interarrival_time(const std::list<types::Packet>& packets) const;		//Done
			float mean_interarrival_time(const std::list<types::Packet>& packets) const;	//Done
			float max_interarrival_time(const std::list<types::Packet>& packets) const;		//Done
			float stddev_interarrival_time(const std::list<types::Packet>& packets) const;	//Done
			unsigned long min_packet_length(const std::list<types::Packet>& packets) const;			//Done
			unsigned long mean_packet_length(const std::list<types::Packet>& packets) const;		//Done
			unsigned long max_packet_length(const std::list<types::Packet>& packets) const;			//Done
			unsigned long stddev_packet_length(const std::list<types::Packet>& packets) const;		//Done
		
		};
	}
}

#endif
