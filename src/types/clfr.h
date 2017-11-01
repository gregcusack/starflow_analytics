
#ifndef STARFLOW_ANALYTICS_CLFR_H
#define STARFLOW_ANALYTICS_CLFR_H

#include <list>

#include "packet.h"
#include "key.h"

namespace starflow {
	namespace types {
		class CLFR
		{
		public:
			CLFR()                       = default;
			CLFR(const CLFR&)            = default;
			CLFR& operator=(const CLFR&) = default;
			CLFR(CLFR&&)                 = default;
			CLFR& operator=(CLFR&&)      = default;

			void add_packet(Packet p);

			std::string str_desc() const;

			unsigned long n_packets() const;
			unsigned long n_bytes() const;

			virtual ~CLFR() = default;

		private:
			std::list<types::Packet> _packets;
		};
	}
}

#endif
