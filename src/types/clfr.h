
#ifndef STARFLOW_ANALYTICS_CLFR_H
#define STARFLOW_ANALYTICS_CLFR_H

#include <list>

#include "packet.h"
#include "key.h"


namespace starflow {

	// forward declare kernels::CLFRTable to allow friend
	// specification w/o circular dependency
	namespace kernels { class CLFRTable; }
	namespace modules { class FlowTable; }

	namespace types {
		class CLFR
		{
			friend class starflow::kernels::CLFRTable;
			friend class starflow::modules::FlowTable;

		public:
			CLFR();
			CLFR(const Key& k);
			CLFR(const CLFR&)            = default;
			CLFR& operator=(const CLFR&) = default;
			CLFR(CLFR&&)                 = default;
			CLFR& operator=(CLFR&&)      = default;

			const Key& key() const;

			void add_packet(Packet p);
			const Packet& last_packet() const;

			const std::list<Packet>& packets() const;

			std::string str_desc() const;

			bool complete() const;

			unsigned long n_packets() const;
			unsigned long n_bytes() const;
			
			virtual ~CLFR() = default;

		private:
			Key _key;
			bool _complete = false;
			std::chrono::microseconds _evict_ts = std::chrono::microseconds(0);
			std::list<types::Packet> _packets = {};
		};
	}
}

#endif
