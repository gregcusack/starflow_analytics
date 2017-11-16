
#ifndef STARFLOW_ANALYTICS_FLOW_TABLE_H
#define STARFLOW_ANALYTICS_FLOW_TABLE_H

#include <raft>

#include "../types/key.h"
#include "../types/clfr.h"

namespace starflow {
	namespace kernels {

		class CLFRTable : public raft::kernel
		{
			enum class incomplete_evict_policy { none, to, pkt_count };
			using flow_table_t = std::map<types::Key, types::CLFR>;

		public:
			CLFRTable();
			raft::kstatus run() override;

		private:
			flow_table_t _flows;

			std::chrono::microseconds _to_check_interval    = std::chrono::seconds(10);
			std::chrono::microseconds _udp_to               = std::chrono::seconds(30);
			std::chrono::microseconds _last_to_check        = std::chrono::seconds(0);

			incomplete_evict_policy _evict_policy           = incomplete_evict_policy::none;
			std::chrono::microseconds _incomplete_evict_to  = std::chrono::seconds(10);
			unsigned long _incomplete_evict_pkt_count       = 50;

			unsigned long long _n_packets_processed         = 0;
			unsigned long long _n_flows_processed           = 0;
			unsigned long long _n_packets                   = 0;
			unsigned long long _n_flows                     = 0;

			void _check_timeouts(std::chrono::microseconds trigger_ts);

			flow_table_t::iterator _evict_flow(const flow_table_t::iterator& i,
				std::chrono::microseconds evict_ts, bool complete = true);
		};
	}
}

#endif
