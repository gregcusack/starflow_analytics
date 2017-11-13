
#ifndef STARFLOW_ANALYTICS_CLFR_H
#define STARFLOW_ANALYTICS_CLFR_H

#include <list>

#include "packet.h"
#include "key.h"


namespace starflow {

	// forward declare kernels::CLFRTable to allow friend
	// specification w/o circular dependency
	namespace kernels { class CLFRTable; }

	namespace types {
		class CLFR
		{
			friend class starflow::kernels::CLFRTable;

		public:
			CLFR()                       = default;
			CLFR(const CLFR&)            = default;
			CLFR& operator=(const CLFR&) = default;
			CLFR(CLFR&&)                 = default;
			CLFR& operator=(CLFR&&)      = default;

			void add_packet(Packet p);
			const Packet& last_packet() const;

			std::string str_desc() const;

			bool complete() const;

			unsigned long n_packets() const;
			unsigned long n_bytes() const;

			std::list<types::Packet>& get_pkt_list();
			
			/* Feature Extraction */
			unsigned long flow_duration() const;			//Done
			unsigned long min_interarrival_time() const;	//Done
			unsigned long mean_interarrival_time() const;	//Done
			unsigned long max_interarrival_time() const;
			unsigned long stddev_interarrival_time() const;
			unsigned long min_packet_length() const;
			unsigned long mean_packet_length() const;		//Done
			unsigned long max_packet_length() const;
			unsigned long stddev_packet_length() const;
						
			
			virtual ~CLFR() = default;

		private:
			bool _complete = false;
			std::chrono::microseconds _evict_ts = std::chrono::microseconds(0);
			std::list<types::Packet> _packets = {};
		};
	}
}

#endif
